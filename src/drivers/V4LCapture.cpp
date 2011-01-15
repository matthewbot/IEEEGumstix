#include "ieee/drivers/V4LCapture.h"
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <sys/mman.h>
#include <errno.h>
#include <cstring>
#include <string>
#include <sstream>
#include <stdexcept>

using namespace ieee;
using namespace cv;
using namespace std;

struct YUVPixel {
	uchar y, u, v;
};

struct BGRPixel {
	uchar b, g, r;
};

struct YUYV { // raw image format from camera, each corresponds to two YUV pixels
	uchar y, u, y2, v;
};

static BGRPixel yuv2bgr(const YUVPixel &in);
static void convertFrame(BGRPixel *frame, const YUYV *yuyv, int length);

V4LCapture::V4LCapture(int width, int height, const string &filename, int exposure) : width(width), height(height) {
	std::pair<bool, std::string> result;
	if (filename.size())
		result = openDevice(filename);
	else
		result = scanDevices();

	if (!result.first)
		throw runtime_error(result.second);

	if (exposure == -1)
		setAutoExposure(true);
	else
		setExposure(exposure);

	v4l2_format fmt; // set up video format
	memset(&fmt, 0, sizeof(fmt));
    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = width;
    fmt.fmt.pix.height      = height;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;

    if (ioctl(fd, VIDIOC_S_FMT, &fmt) == -1)
    	throw runtime_error("Can't set video format");

    v4l2_requestbuffers reqbufs; // request only single buffer, we don't care about dropped frames and in fact always want the most recent image available
    memset(&reqbufs, 0, sizeof(reqbufs));
    reqbufs.count  = 1;
    reqbufs.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqbufs.memory = V4L2_MEMORY_MMAP;

    if (ioctl(fd, VIDIOC_REQBUFS, &reqbufs) == -1)
    	throw runtime_error("Failed to request buffers");
    if (reqbufs.count != 1)
    	throw runtime_error("Got multiple buffers");

	v4l2_buffer buf; // queue the buffer so it can be written to by the driver
	memset(&buf, 0, sizeof(buf));
	buf.index = 0;
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;

	if (ioctl(fd, VIDIOC_QBUF, &buf) == -1)
		throw runtime_error("Failed to queue buffer");

    v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE; // tell the driver to begin streaming
    if (ioctl(fd, VIDIOC_STREAMON, &type) == -1)
    	throw runtime_error("Failed to start stream");
}

pair<bool, string> V4LCapture::openDevice(const string &filename) {
	fd = open(filename.c_str(), O_RDWR, 0);
	if (fd == -1)
		return make_pair(false, "Can't open " + filename + ": " + strerror(errno));

	v4l2_capability cap; // query the device's capabilities to verify its actually a webcam/capture card
	ioctl(fd, VIDIOC_QUERYCAP, &cap);
	if ((cap.capabilities & V4L2_CAP_STREAMING) == 0)
		return make_pair(false, "Video device " + filename + " doesn't support streaming");

	return make_pair(true, "");
}

std::pair<bool, std::string> V4LCapture::scanDevices() {
	stringstream errbuf;
	for (int i=5; i>=0; i--) {
		stringstream devicebuf;
		devicebuf << "/dev/video" << i;
		pair<bool, string> out = openDevice(devicebuf.str());

		if (out.first)
			return make_pair(true, "");
		else
			errbuf << out.second << endl;
	}

	return make_pair(false, "Failed to scan for video device\n" + errbuf.str());
}

V4LCapture::~V4LCapture() {
	v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ioctl(fd, VIDIOC_STREAMOFF, &type); // stop streaming
	close(fd); // close filehandle
}

void V4LCapture::readFrame(Mat &mat) {
	v4l2_buffer buf; // dequeue our buffer (will block until the driver has actually written data to it)
	memset(&buf, 0, sizeof(buf));
	buf.index = 0;
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;

	if (ioctl(fd, VIDIOC_DQBUF, &buf) == -1)
		throw runtime_error("Failed to dequeue buffer");
	if (buf.length != width*height*2) { // YUYV has 2 pixels every 4 bytes, so 2 bytes per pixel
		stringstream errbuf;
		errbuf << "Bad number of bytes " << buf.length;
		throw runtime_error(errbuf.str());
	}

	// after we've dequeued the buffer, its safe to mmap in to our process so we can look at it
	YUYV *rawframe = (YUYV *)mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
	if (rawframe == NULL)
		throw runtime_error("Failed to mmap buffer");

	mat.create(height, width, CV_8UC3); // tell OpenCV to allocate memory for the Mat
	convertFrame((BGRPixel *)mat.data, rawframe, width*height*2); // do our conversion directly from the mmap()ed buffer to OpenCV's memory

	munmap(rawframe, buf.length); // unmap the buffer

	if (ioctl(fd, VIDIOC_QBUF, &buf) == -1) // requeue it with the driver
		throw runtime_error("Failed to re-queue buffer");
}

static void convertFrame(BGRPixel *frame, const YUYV *rawframe, int length) {
	while (length) {
		YUVPixel pix1 = { rawframe->y, rawframe->u, rawframe->v}; // every 4-byte YUYV has two YUV pixels
		YUVPixel pix2 = { rawframe->y2, rawframe->u, rawframe->v};
		rawframe++;
		length -= sizeof(YUYV);

		*frame++ = yuv2bgr(pix1); // so output the two as RGB
		*frame++ = yuv2bgr(pix2);
	}
}

// based on http://social.msdn.microsoft.com/Forums/en/windowsdirectshowdevelopment/thread/1071301e-74a2-4de4-be72-81c34604cde9
static BGRPixel yuv2bgr(const YUVPixel &in) {
	int c = (int)in.y-16;
	int d = (int)in.u-128;
	int e = (int)in.v-128;

	BGRPixel out;
	out.r = saturate_cast<uchar>((298 * c           + 409 * e + 128) >> 8);
	out.g = saturate_cast<uchar>((298 * c - 100 * d - 208 * e + 128) >> 8);
	out.b = saturate_cast<uchar>((298 * c + 516 * d           + 128) >> 8);

	return out;
}

void V4LCapture::setAutoExposure(bool on) {
	if (!setControl(V4L2_CID_EXPOSURE_AUTO, on ? V4L2_EXPOSURE_APERTURE_PRIORITY : V4L2_EXPOSURE_MANUAL)) {
		throw runtime_error(string("Failed to change auto exposure setting: ") + strerror(errno));
	}
}

void V4LCapture::setExposure(int exposure) {
	setAutoExposure(false);
	if (!setControl(V4L2_CID_EXPOSURE_ABSOLUTE, exposure)) {
		throw runtime_error(string("Failed to set exposure: ") + strerror(errno));
	}
}

bool V4LCapture::setControl(int id, int val) {
	struct v4l2_control control;
	memset(&control, 0, sizeof(control));
	control.id = id;
	control.value = val;

	return ioctl(fd, VIDIOC_S_CTRL, &control) != -1;
}


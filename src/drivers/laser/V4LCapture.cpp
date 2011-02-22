#include "ieee/drivers/laser/V4LCapture.h"
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <sys/mman.h>
#include <errno.h>
#include <cstring>
#include <string>
#include <sstream>
#include <stdexcept>
#include <algorithm>

using namespace ieee;
using namespace std;

V4LCapture::V4LCapture(int width, int height, const string &filename, int exposure, int bufs) : width(width), height(height) {
	std::pair<bool, std::string> result;
	if (filename.size())
		result = openDevice(filename);
	else
		result = scanDevices();

	if (!result.first)
		throw runtime_error(result.second);

	if (exposure == -1) {
		try {
			setAutoExposure(true);
		} catch (runtime_error &err) { }
	} else {
		setExposure(exposure);
	}

	v4l2_format fmt; // set up video format
	memset(&fmt, 0, sizeof(fmt));
    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = width;
    fmt.fmt.pix.height      = height;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;

    if (ioctl(fd, VIDIOC_S_FMT, &fmt) == -1)
    	throw runtime_error("Can't set video format");

    v4l2_requestbuffers reqbufs; // request buffers
    memset(&reqbufs, 0, sizeof(reqbufs));
    reqbufs.count  = bufs;
    reqbufs.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqbufs.memory = V4L2_MEMORY_MMAP;

    if (ioctl(fd, VIDIOC_REQBUFS, &reqbufs) == -1)
    	throw runtime_error("Failed to request buffers");
    if (reqbufs.count != bufs)
    	throw runtime_error("Got wrong number of buffers");

	for (int i=0; i<bufs; i++) { // queue the buffers so it can be written to by the driver
		v4l2_buffer buf;
		memset(&buf, 0, sizeof(buf));
		buf.index = i;
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;

		if (ioctl(fd, VIDIOC_QBUF, &buf) == -1)
			throw runtime_error("Failed to queue buffer");
	}

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

void V4LCapture::readFrame(Image &out) {
	out.reformat(height, width, Image::YUYV);

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
	uint8_t *data = (uint8_t *)mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
	if (data == NULL)
		throw runtime_error("Failed to mmap buffer");

	memcpy(out.getData(), data, buf.length);
	munmap(data, buf.length); // unmap the buffer

	if (ioctl(fd, VIDIOC_QBUF, &buf) == -1) // requeue it with the driver
		throw runtime_error("Failed to re-queue buffer");
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


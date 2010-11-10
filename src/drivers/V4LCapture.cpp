#include "ieeepath/drivers/V4LCapture.h"
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <sys/mman.h>
#include <cstring>
#include <string>
#include <sstream>
#include <stdexcept>

using namespace ieeepath;
using namespace cv;
using namespace std;

struct YUVPixel {
	uint8_t y, u, v;
} __attribute__((__packed__));

struct BGRPixel {
	uint8_t b, g, r;
} __attribute__((__packed__));

struct YUYV {
	uint8_t y;
	uint8_t u;
	uint8_t y2;
	uint8_t v;
} __attribute__((__packed__));

static BGRPixel yuv2bgr(const YUVPixel &in);
static void convertFrame(BGRPixel *frame, const YUYV *yuyv, int length);

V4LCapture::V4LCapture(const string &filename, int width, int height) : width(width), height(height) {
	fd = open(filename.c_str(), O_RDWR, 0);
	if (fd == -1)
		throw runtime_error("Can't open " + filename);
	
	struct v4l2_capability cap;
	ioctl(fd, VIDIOC_QUERYCAP, &cap);
	
	if ((cap.capabilities & V4L2_CAP_STREAMING) == 0)
		throw runtime_error("Video device doesn't support streaming");
	
	struct v4l2_format fmt;
	memset(&fmt, 0, sizeof(fmt));
    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = width; 
    fmt.fmt.pix.height      = height;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    
    if (ioctl(fd, VIDIOC_S_FMT, &fmt) == -1)
    	throw runtime_error("Can't set video format");
    	
    struct v4l2_requestbuffers reqbufs;
    memset(&reqbufs, 0, sizeof(reqbufs));
    reqbufs.count = 1;
    reqbufs.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqbufs.memory = V4L2_MEMORY_MMAP;
    
    if (ioctl(fd, VIDIOC_REQBUFS, &reqbufs) == -1)
    	throw runtime_error("Failed to request buffers");
    if (reqbufs.count != 1)
    	throw runtime_error("Got multiple buffers");
    
	struct v4l2_buffer buf;
	memset(&buf, 0, sizeof(buf));
	buf.index = 0;
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	
	if (ioctl(fd, VIDIOC_QBUF, &buf) == -1)
		throw runtime_error("Failed to queue buffer");
    
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(fd, VIDIOC_STREAMON, &type) == -1)
    	throw runtime_error("Failed to start stream");
}
		
V4LCapture::~V4LCapture() {
	ioctl(fd, VIDIOC_STREAMOFF);
	close(fd);
}

		
void V4LCapture::readFrame(Mat &mat) {
	struct v4l2_buffer buf;
	memset(&buf, 0, sizeof(buf));
	buf.index = 0;
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	
	if (ioctl(fd, VIDIOC_DQBUF, &buf) == -1)
		throw runtime_error("Failed to dequeue buffer");
	if (buf.length != width*height*2) {
		stringstream errbuf;
		errbuf << "Bad number of bytes " << buf.length;
		throw runtime_error(errbuf.str());
	}
	
	YUYV *rawframe = (YUYV *)mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
	if (rawframe == NULL)
		throw runtime_error("Failed to mmap buffer");
	
	mat.create(height, width, CV_8UC3);
	convertFrame((BGRPixel *)mat.data, rawframe, width*height*2);
	
	munmap(rawframe, buf.length);
	
	if (ioctl(fd, VIDIOC_QBUF, &buf) == -1)
		throw runtime_error("Failed to re-queue buffer");
}
		
static void convertFrame(BGRPixel *frame, const YUYV *rawframe, int length) {
	while (length) {
		YUVPixel pix1 = { rawframe->y, rawframe->u, rawframe->v};
		YUVPixel pix2 = { rawframe->y2, rawframe->u, rawframe->v};
		rawframe++;
		length -= sizeof(YUYV);
		
		*frame++ = yuv2bgr(pix1);
		*frame++ = yuv2bgr(pix2);
	}
}

// based on http://social.msdn.microsoft.com/Forums/en/windowsdirectshowdevelopment/thread/1071301e-74a2-4de4-be72-81c34604cde9
static BGRPixel yuv2bgr(const YUVPixel &in) {
	int c = (int)in.y-16;
	int d = (int)in.u-128;
	int e = (int)in.v-128;       

	BGRPixel out;
	out.r = saturate_cast<uint8_t>((298 * c           + 409 * e + 128) >> 8);
	out.g = saturate_cast<uint8_t>((298 * c - 100 * d - 208 * e + 128) >> 8);
	out.b = saturate_cast<uint8_t>((298 * c + 516 * d           + 128) >> 8);
	
	return out;
}


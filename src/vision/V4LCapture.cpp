#include "ieeepath/vision/V4LCapture.h"
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <sys/mman.h>
#include <cstring>
#include <string>
#include <sstream>
#include <stdexcept>

using namespace ieeepath;
using namespace std;

struct YUVPixel {
	uint8_t y, u, v;
};

struct YUYV {
	uint8_t y;
	uint8_t u;
	uint8_t y2;
	uint8_t v;
} __attribute__((__packed__));

typedef YUYV RawFrame[320*240/2];

static uint8_t clamp_u8(int val);
static V4LCapture::RGBPixel yuv2rgb(const YUVPixel &in);
static void convertFromRaw(const RawFrame &rawframe, V4LCapture::Frame &frame);

V4LCapture::V4LCapture(const string &filename) {
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
    fmt.fmt.pix.width       = 320; 
    fmt.fmt.pix.height      = 240;
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

		
void V4LCapture::readFrame(Frame &frame) {
	struct v4l2_buffer buf;
	memset(&buf, 0, sizeof(buf));
	buf.index = 0;
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	
	if (ioctl(fd, VIDIOC_DQBUF, &buf) == -1)
		throw runtime_error("Failed to dequeue buffer");
	if (buf.length != sizeof(RawFrame)) {
		stringstream errbuf;
		errbuf << "Bad number of bytes " << buf.length;
		throw runtime_error(errbuf.str());
	}
	
	
	RawFrame *rawframe = (RawFrame *)mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
	if (rawframe == NULL)
		throw runtime_error("Failed to mmap buffer");
	
	convertFromRaw(*rawframe, frame);
	
	munmap(rawframe, buf.length);
	
	if (ioctl(fd, VIDIOC_QBUF, &buf) == -1)
		throw runtime_error("Failed to re-queue buffer");
}
		
static void convertFromRaw(const RawFrame &rawframe, V4LCapture::Frame &frame) {
	struct V4LCapture::RGBPixel *rgb = frame;
	const struct YUYV *yuyv = rawframe;
	
	for (int i=0; i<320*240/2; i++) {
		YUVPixel pix1 = { yuyv->y, yuyv->u, yuyv->v};
		YUVPixel pix2 = { yuyv->y2, yuyv->u, yuyv->v};
		yuyv++;
		
		*rgb++ = yuv2rgb(pix1);
		*rgb++ = yuv2rgb(pix2);
	}
}

static uint8_t clamp_u8(int val) {
	if (val > 255)
		return 255;
	else if (val < 0)
		return 0;
	else
		return (uint8_t)val;
}

// based on http://social.msdn.microsoft.com/Forums/en/windowsdirectshowdevelopment/thread/1071301e-74a2-4de4-be72-81c34604cde9
static V4LCapture::RGBPixel yuv2rgb(const YUVPixel &in) {
	V4LCapture::RGBPixel out;
	
	int c = (int)in.y-16;
	int d = (int)in.u-128;
	int e = (int)in.v-128;       

	out.r = clamp_u8((298 * c           + 409 * e + 128) >> 8);
	out.g = clamp_u8((298 * c - 100 * d - 208 * e + 128) >> 8);
	out.b = clamp_u8((298 * c + 516 * d           + 128) >> 8);
	
	return out;
}


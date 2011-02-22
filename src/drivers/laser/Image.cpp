#include "ieee/drivers/laser/Image.h"
#include <cassert>
#include <stdexcept>

using namespace ieee;
using namespace std;

Image::Image(Format format)
: rows(0), cols(0), format(format) { }
Image::Image(int rows, int cols, Format format)
: rows(rows), cols(cols), format(format), data(rows*cols*bytesPerPixel(format)) { }
Image::Image(const Image &orig)
: rows(orig.rows), cols(orig.cols), format(format), data(orig.data) { }
Image::Image(const Image &orig, Format format)
: rows(orig.rows), cols(orig.cols), format(format), data(rows*cols*bytesPerPixel(format)) {
	if (orig.format == format)
		copy(orig.data.begin(), orig.data.end(), data.begin());
	else if (orig.format == YUYV && format == RGB)
		yuyv2rgb(orig.data.begin(), orig.data.end(), data.begin());
	else if (orig.format == GRAYSCALE && format == RGB)
		grayscale2rgb(orig.data.begin(), orig.data.end(), data.begin());
	else
		throw runtime_error("Attempted image conversion between unsupported formats");
}

void Image::resize(int rows, int cols) {
	this->rows = rows;
	this->cols = cols;

	int bytes = rows*cols*bytesPerPixel(format);
	if (data.size() < bytes) {
		DataVec newvec(bytes);
		swap(data, newvec); // this simply swaps the internal pointers, and avoids copying the old data over
	}
}

void Image::reformat(int rows, int cols, Format format) {
	this->format = format;

	resize(rows, cols);
}

bool Image::inBounds(int row, int col) const {
	return row >= 0 && col >= 0 && row < rows && col < cols;
}

void Image::clear() {
	fill(data.begin(), data.end(), 0);
}

static uint8_t u8sat(int i) {
	if ((i & ~0xFF) == 0)
		return (uint8_t)i;
	else if (i < 0) // check sign bit (assumes 32 bit 2's signed complement)
		return 0;
	else
		return 255;
}

void Image::yuyv2rgb(DataVec::const_iterator iter, DataVec::const_iterator end, DataVec::iterator out) {
	for (; iter != end; iter+=4) {
		int c  = ((int)iter[0]-16)*298;
		int c2 = ((int)iter[2]-16)*298;
		int d  = (int)iter[1]-128;
		int e  = (int)iter[3]-128;

		int rtmp = 409 * e + 128;
		int gtmp = -100 * d - 208 * e + 128;
		int btmp = 516 * d + 128;

		*out++ = u8sat((c + rtmp) >> 8); // RGB1
		*out++ = u8sat((c + gtmp) >> 8);
		*out++ = u8sat((c + btmp) >> 8);
		*out++ = u8sat((c2 + rtmp) >> 8); // RGB2
		*out++ = u8sat((c2 + gtmp) >> 8);
		*out++ = u8sat((c2 + btmp) >> 8);
	}
}

void Image::grayscale2rgb(DataVec::const_iterator iter, DataVec::const_iterator end, DataVec::iterator out) {
	for (; iter != end; iter++) {
		*out++ = *iter;
		*out++ = *iter;
		*out++ = *iter;
	}
}


#include "ieee/sim/shared/ImagePanel.h"
#include <opencv/cv.h>

using namespace ieee;
using namespace cv;
using namespace std;

BEGIN_EVENT_TABLE(ImagePanel, wxPanel)
	EVT_PAINT(ImagePanel::OnPaint)
END_EVENT_TABLE()

ImagePanel::ImagePanel(wxWindow *parent) : wxPanel(parent) { }

void ImagePanel::update(const Mat &frame) {
	this->frame = frame;
	bitmap_dirty = true;
}

void ImagePanel::OnPaint(wxPaintEvent &ev) {
	if (bitmap_dirty)
		regenBitmap();

	wxPaintDC dc(this);

	if (bitmap.IsOk()) {
		wxSize size = dc.GetSize();
		int x = size.x/2 - bitmap.GetWidth()/2;
		int y = size.y/2 - bitmap.GetHeight()/2;
		dc.DrawBitmap(bitmap, x, y, false);
	}
}

void ImagePanel::regenBitmap() const {
	if (frame.data) {
		wxImage image(frame.cols, frame.rows, false);
		if (frame.channels() == 3)
			frameConv3C(image, frame);
		else if (frame.channels() == 1)
			frameConv1C(image, frame);
		else
			throw runtime_error("Bad number of channels in ImagePanel's frame Mat");

		bitmap = image;
	} else {
		bitmap = wxBitmap();
	}

	bitmap_dirty = false;
}

void ImagePanel::frameConv3C(wxImage &out, const cv::Mat &frame) {
	uchar *imagedata = out.GetData();
	const uchar *framedata = frame.data;
	const uchar *framedata_end = framedata + frame.rows*frame.cols*3;
	while (framedata != framedata_end) {
		imagedata[0] = framedata[2]; // BGR -> RGB
		imagedata[1] = framedata[1];
		imagedata[2] = framedata[0];
		imagedata += 3;
		framedata += 3;
	}
}

void ImagePanel::frameConv1C(wxImage &out, const cv::Mat &frame) {
	uchar *imagedata = out.GetData();
	const uchar *framedata = frame.data;
	const uchar *framedata_end = framedata + frame.rows*frame.cols;
	while (framedata != framedata_end) {
		imagedata[0] = *framedata;
		imagedata[1] = *framedata;
		imagedata[2] = *framedata;
		imagedata += 3;
		framedata++;
	}
}


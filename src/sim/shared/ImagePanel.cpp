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
	if (bitmap.IsOk())
		dc.DrawBitmap(bitmap, 0, 0, false);
}

void ImagePanel::regenBitmap() const {
	if (frame.data) {
		wxImage image(frame.cols, frame.rows, false);
		unsigned char *imagedata = image.GetData();
		const uchar *framedata = frame.data;
		for (int ctr=0; ctr<frame.rows*frame.cols; ctr++) {
			imagedata[0] = framedata[2];
			imagedata[1] = framedata[1];
			imagedata[2] = framedata[0];
			imagedata += 3;
			framedata += 3;
		}

		bitmap = image;
	} else {
		bitmap = wxBitmap();
	}

	bitmap_dirty = false;
}


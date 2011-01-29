#include "ieee/sim/shared/ImagePanel.h"

using namespace ieee;
using namespace std;

BEGIN_EVENT_TABLE(ImagePanel, wxPanel)
	EVT_PAINT(ImagePanel::OnPaint)
END_EVENT_TABLE()

ImagePanel::ImagePanel(wxWindow *parent) : wxPanel(parent) { }

void ImagePanel::update(const Image &frame) {
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
	if (frame.getRows() && frame.getCols()) {
		wxImage image(frame.getCols(), frame.getRows(), false);
		if (frame.getFormat() == Image::RGB)
			frameConvRGB(image, frame);
		else if (frame.getFormat() == Image::GRAYSCALE)
			frameConvGray(image, frame);
		else if (frame.getFormat() == Image::YUYV)
			frameConvRGB(image, Image(frame, Image::RGB));
		else
			throw runtime_error("Bad format image in ImagePanel");

		bitmap = image;
	} else {
		bitmap = wxBitmap();
	}

	bitmap_dirty = false;
}

void ImagePanel::frameConvRGB(wxImage &out, const Image &frame) {
	unsigned char *imagedata = out.GetData();
	const unsigned char *framedata = frame.getData();
	const unsigned char *framedata_end = framedata + frame.getRows()*frame.getCols()*3;
	copy(framedata, framedata_end, imagedata);
}

void ImagePanel::frameConvGray(wxImage &out, const Image &frame) {
	unsigned char *imagedata = out.GetData();
	const unsigned char *framedata = frame.getData();
	const unsigned char *framedata_end = framedata + frame.getRows()*frame.getCols();
	while (framedata != framedata_end) {
		imagedata[0] = *framedata;
		imagedata[1] = *framedata;
		imagedata[2] = *framedata;
		imagedata += 3;
		framedata++;
	}
}


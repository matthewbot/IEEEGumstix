#include "ieee/sim/laser/LaserImagePanel.h"

using namespace ieee;
using namespace cv;
using namespace std;

BEGIN_EVENT_TABLE(LaserImagePanel, ImagePanel)
	EVT_PAINT(LaserImagePanel::OnPaint)
END_EVENT_TABLE()

LaserImagePanel::LaserImagePanel(wxWindow *parent) : ImagePanel(parent) { }

void LaserImagePanel::update(const Mat &frame, const LaserSensor::RawReadings &readings) {
	ImagePanel::update(frame);

	this->readings = readings;
}

void LaserImagePanel::OnPaint(wxPaintEvent &event) {
	ImagePanel::OnPaint(event);

	wxPaintDC dc(this);

	wxSize size = dc.GetSize();
	int basex = size.x/2 - getFrameWidth()/2;
	int basey = size.y/2 - getFrameHeight()/2;

	for (int laser=0; laser<readings.size(); laser++) {
		static wxPen pens[] = {
			wxPen(*wxBLUE),
			wxPen(*wxGREEN),
			wxPen(*wxRED)
		};
		dc.SetPen(pens[laser]);

		const vector<int> &vals = readings[laser];
		for (int x=0; x<vals.size(); x++) {
			const int y = vals[x];
			if (y == -1)
				continue;

			dc.DrawPoint(basex+x, basey+y);
		}
	}

}

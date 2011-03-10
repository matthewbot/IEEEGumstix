#include "ieee/sim/laser/LaserPanelLayer.h"

using namespace ieee;
using namespace std;

LaserPanelLayer::LaserPanelLayer(const LaserSensor::Readings &readings) : readings(readings) { }

int LaserPanelLayer::getWeight() const { return WEIGHT; }

void LaserPanelLayer::render(wxPaintDC &dc, const CoordScale &drawscale) const {
	static wxPen pens[] = {
		wxPen(*wxBLUE),
		wxPen(*wxGREEN),
		wxPen(*wxRED)
	};

	static wxBrush brushes[] = {
		wxBrush(*wxBLUE),
		wxBrush(*wxGREEN),
		wxBrush(*wxRED)
	};

	for (int laser=0; laser < readings.size(); laser++) {
		const LaserSensor::DistAngleVec &distangles = readings[laser];
		for (LaserSensor::DistAngleVec::const_iterator i = distangles.begin(); i != distangles.end(); ++i) {
			Coord coord = i->toCoord(M_PI/2);

			Pos pos = drawscale.coordToPos(coord.x + 50, 100 - coord.y);

			int num = min(laser, 3);
			dc.SetPen(pens[num]);
			dc.SetBrush(brushes[num]);
			dc.DrawCircle(pos.x, pos.y, 1);
		}
	}
}


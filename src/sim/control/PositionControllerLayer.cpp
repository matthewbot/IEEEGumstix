#include "ieee/sim/control/PositionControllerLayer.h"

using namespace ieee;

PositionControllerLayer::PositionControllerLayer(const PositionController &poscontrol)
: poscontrol(poscontrol) { }

int PositionControllerLayer::getWeight() const { return WEIGHT; }

void PositionControllerLayer::render(wxPaintDC &dc, const CoordScale &drawscale) const {
	renderRobot(dc, drawscale);
	renderDragCommand(dc, drawscale);
}

void PositionControllerLayer::renderRobot(wxPaintDC &dc, const CoordScale &drawscale) const {
	const PositionFilter::Output &posout = poscontrol.getPositionFilter().getOutput();

	Pos pos = drawscale.coordToPos(posout.pos);
	const float thickness = 15;
	const float dir = posout.dir;

	wxPoint points[3];
	points[0].x = (int)(pos.x + thickness*cos(dir));
	points[0].y = (int)(pos.y - thickness*sin(dir));
	points[1].x = (int)(pos.x + thickness*cos(dir + 5*M_PI/4));
	points[1].y = (int)(pos.y - thickness*sin(dir + 5*M_PI/4));
	points[2].x = (int)(pos.x + thickness*cos(dir - 5*M_PI/4));
	points[2].y = (int)(pos.y - thickness*sin(dir - 5*M_PI/4));
	dc.DrawPolygon(3, points);
}

void PositionControllerLayer::renderDragCommand(wxPaintDC &dc, const CoordScale &drawscale) const {
	if (!dragging)
		return;

	Pos startpos = drawscale.coordToPos(startcoord);
	Pos endpos = drawscale.coordToPos(endcoord);
	dc.DrawLine(startpos.x, startpos.y, endpos.x, endpos.y);
}

bool PositionControllerLayer::leftDown(const Coord &coord) {
	dragging = true;
	startcoord = endcoord = coord;
	return true;
}

bool PositionControllerLayer::mouseMotion(const Coord &coord) {
	if (!dragging)
		return false;

	endcoord = coord;
	return true;
}

bool PositionControllerLayer::leftUp(const Coord &coord) {
	dragging = false;
	return true;
}


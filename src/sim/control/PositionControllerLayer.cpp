#include "ieee/sim/control/PositionControllerLayer.h"
#include <cmath>

using namespace ieee;
using namespace std;

PositionControllerLayer::PositionControllerLayer(Callbacks &callbacks, const PositionController &poscontrol)
: callbacks(callbacks), poscontrol(poscontrol), dragging(false), lastangle(0) { }

int PositionControllerLayer::getWeight() const { return WEIGHT; }

void PositionControllerLayer::render(wxPaintDC &dc, const CoordScale &drawscale) const {
	renderRobot(dc, drawscale);
	renderControllerCommand(dc, drawscale);
	renderDragCommand(dc, drawscale);
}

void PositionControllerLayer::renderRobot(wxPaintDC &dc, const CoordScale &drawscale) const {
	Vec2D vecpos = poscontrol.getPositionFilter().getPosition();
	Pos pos = drawscale.coordToPos(Vec2D(vecpos.x, 100 - vecpos.y)); // TODO don't encode room height here
	const float thickness = 15;
	const float dir = poscontrol.getPositionFilter().getHeading().getRad();

	wxPoint points[3];
	points[0].x = (int)(pos.x + thickness*cos(dir));
	points[0].y = (int)(pos.y - thickness*sin(dir));
	points[1].x = (int)(pos.x + thickness*cos(dir + 5*M_PI/4));
	points[1].y = (int)(pos.y - thickness*sin(dir + 5*M_PI/4));
	points[2].x = (int)(pos.x + thickness*cos(dir - 5*M_PI/4));
	points[2].y = (int)(pos.y - thickness*sin(dir - 5*M_PI/4));
	dc.DrawPolygon(3, points);
}

void PositionControllerLayer::renderControllerCommand(wxPaintDC &dc, const CoordScale &drawscale) const {
	if (poscontrol.getState() == PositionController::STOPPED)
		return;
	const PositionController::Command &command = poscontrol.getCommand();
	Vec2D vec = Vec2D(command.destpos.x, 100 - command.destpos.y);

	Pos pos = drawscale.coordToPos(vec);
	const float dir = command.destheading.getRad();

	dc.DrawCircle(pos.x, pos.y, 4);

	const float len = 8;
	const float endx = pos.x+len*cos(dir);
	const float endy = pos.y-len*sin(dir);
	dc.DrawLine(pos.x, pos.y, endx, endy);
	dc.DrawCircle(endx, endy, 1);
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
	float angle;
	if ((Vec2D(startcoord) - Vec2D(endcoord)).magnitude() < 1)
		angle = lastangle;
	else
		angle = atan2(-(endcoord.y - startcoord.y), endcoord.x - startcoord.x);

	callbacks.onCommand(Coord(startcoord.x, 100 - startcoord.y), angle);

	lastangle = angle;
	dragging = false;
	return true;
}


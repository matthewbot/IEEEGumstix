#include "ieee/sim/RobotPanelLayer.h"
#include <algorithm>

using namespace ieee;
using namespace std;

RobotPanelLayer::RobotPanelLayer(const Robot &robot, const CoordScale &gridscale)
: robot(robot),
  gridscale(gridscale) { }

int RobotPanelLayer::getWeight() const { return WEIGHT; }

void RobotPanelLayer::render(wxPaintDC &dc, const CoordScale &drawscale) const {
	const float minsize = min(drawscale.sx/gridscale.sx, drawscale.sy/gridscale.sy);

	Pos pos = drawscale.coordToPos(robot.getPosition());
	const float thickness = minsize * .3;
	const float dir = robot.getDirection();

	wxPoint points[3];
	points[0].x = (int)(pos.x + thickness*cos(dir));
	points[0].y = (int)(pos.y - thickness*sin(dir));
	points[1].x = (int)(pos.x + thickness*cos(dir + 5*M_PI/4));
	points[1].y = (int)(pos.y - thickness*sin(dir + 5*M_PI/4));
	points[2].x = (int)(pos.x + thickness*cos(dir - 5*M_PI/4));
	points[2].y = (int)(pos.y - thickness*sin(dir - 5*M_PI/4));
	dc.DrawPolygon(3, points);

	wxBrush victimbrush(wxColour(150, 200, 130));
	const RoomPlanner::Plan &plan = robot.getPlan();
	if (plan.identifyvictim)
		dc.SetBrush(victimbrush);

	const float radius = max(minsize*.1f, 3.0f);
	for (int i = 0; i != plan.coords.size(); ++i) {
		Pos p = drawscale.coordToPos(plan.coords[i]);
		float dirrad = dirToRad(plan.facedirs[i]);

		dc.DrawCircle(p.x, p.y, radius);

		const float len = minsize*0.3;
		const float endx = p.x+len*cos(dirrad);
		const float endy = p.y-len*sin(dirrad);
		dc.DrawLine(p.x, p.y, endx, endy);
		dc.DrawCircle(endx, endy, 1);
	}
}


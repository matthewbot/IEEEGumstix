#include "ieee/sim/shared/ObjectPanelLayer.h"
#include "ieee/sim/shared/VictimWorldObject.h"
#include "ieee/sim/shared/ObstacleWorldObject.h"
#include <algorithm>

using namespace ieee;
using namespace std;

ObjectPanelLayer::ObjectPanelLayer(const World &world, const CoordScale &gridscale, Callbacks &callbacks)
: world(world),
  gridscale(gridscale),
  callbacks(callbacks),
  dragging(false) { }

int ObjectPanelLayer::getWeight() const { return WEIGHT; }

void ObjectPanelLayer::render(wxPaintDC &dc, const CoordScale &drawscale) const {
	const float minsize = min(drawscale.sx/gridscale.sx, drawscale.sy/gridscale.sy);

	for (World::const_iterator i = world.begin(); i != world.end(); ++i) {
		if (const VictimWorldObject *victim = dynamic_cast<const VictimWorldObject *>(&*i)) {
			Pos pos = drawscale.coordToPos(gridscale.posToCoord(victim->getPos()));
			const float radius = minsize*0.4f;
			dc.DrawCircle(pos.x, pos.y, radius);
		} else if (const ObstacleWorldObject *obstacle = dynamic_cast<const ObstacleWorldObject *>(&*i)) {
			Pos startpos = drawscale.coordToPos(gridscale.posToCoord(obstacle->getStartPos()));
			Pos endpos = drawscale.coordToPos(gridscale.posToCoord(obstacle->getEndPos()));
			const float dir = atan2(endpos.y-startpos.y, endpos.x - startpos.x);

			const float thickness = (obstacle->isLarge() ? 0.3 : 0.1) * minsize;
			wxPoint points[4]; // points are counter-clockwise from top left of starting position
			points[0].x = (int)(startpos.x + thickness*cos(dir + 3*M_PI/4));
			points[0].y = (int)(startpos.y + thickness*sin(dir + 3*M_PI/4));
			points[1].x = (int)(startpos.x + thickness*cos(dir - 3*M_PI/4));
			points[1].y = (int)(startpos.y + thickness*sin(dir - 3*M_PI/4));
			points[2].x = (int)(endpos.x + thickness*cos(dir - M_PI/4));
			points[2].y = (int)(endpos.y + thickness*sin(dir - M_PI/4));
			points[3].x = (int)(endpos.x + thickness*cos(dir + M_PI/4));
			points[3].y = (int)(endpos.y + thickness*sin(dir + M_PI/4));
			dc.DrawPolygon(4, points);
		}
	}
}

bool ObjectPanelLayer::leftDown(const Coord &coord) {
	Pos pos = gridscale.coordToPos(coord);

	dragging = callbacks.onWorldClicked(pos);
	if (dragging)
		lastdragpos = pos;
	return dragging;
}

void ObjectPanelLayer::leftUp() {
	dragging = false;
}

void ObjectPanelLayer::mouseMotion(const Coord &coord) {
	if (!dragging)
		return;

	Pos pos = gridscale.coordToPos(coord);

	const WorldGrid &grid = world.getGrid();
	if (pos.x < 0)
		pos.x = 0;
	else if (pos.x >= grid.getWidth())
		pos.x = grid.getWidth()-1;

	if (pos.y < 0)
		pos.y = 0;
	else if (pos.y >= grid.getHeight())
		pos.y = grid.getHeight()-1;

	if (lastdragpos != pos) {
		callbacks.onWorldDragged(pos);
		lastdragpos = pos;
	}
}


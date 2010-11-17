#include "ieeepath/sim/WorldPanel.h"
#include "ieeepath/shared/VictimWorldObject.h"
#include "ieeepath/shared/ObstacleWorldObject.h"
#include <algorithm>

using namespace ieee;
using namespace std;

BEGIN_EVENT_TABLE(WorldPanel, wxPanel)
	EVT_LEFT_DOWN(WorldPanel::OnLeftDown)
	EVT_LEFT_UP(WorldPanel::OnLeftUp)
	EVT_MOTION(WorldPanel::OnMotion)
	EVT_PAINT(WorldPanel::OnPaint)
END_EVENT_TABLE()

WorldPanel::WorldPanel(wxWindow *parent, Callbacks &callbacks, const World &world, const Robot &robot)
: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE), 
  callbacks(callbacks), 
  world(world),
  robot(robot) { }

void WorldPanel::OnPaint(wxPaintEvent &event) {
	wxPaintDC dc(this);
	
	paintGrid(dc);
	paintObjects(dc);
	paintRobot(dc);
}

void WorldPanel::paintGrid(wxPaintDC &dc) {
	const WorldGrid &grid = world.getGrid();

	wxCoord dispw, disph;
	dc.GetSize(&dispw, &disph);
	
	const float squarew = (float)dispw/grid.getWidth();
	const float squareh = (float)disph/grid.getHeight();
	
	wxBrush brush;
	
	for (int x=0; x<grid.getWidth(); x++) {
		for (int y=0; y<grid.getHeight(); y++) {
			Pos pos(x, y);
			float squarex = x * squarew;
			float squarey = y * squareh;
		
			switch (grid[pos]) {
				case WorldGrid::EMPTY:
					brush.SetColour(255, 255, 255);
					break;
			
				case WorldGrid::SMALL_OBSTACLE:
					brush.SetColour(240, 240, 80);
					break;
					
				case WorldGrid::VICTIM:
					brush.SetColour(150, 200, 130);
					break;
					
				case WorldGrid::LARGE_OBSTACLE:
					brush.SetColour(250, 100, 100);
					break;
			}
			
			if (robot.getMap()[pos] == WorldGrid::UNKNOWN) {
				wxColour color = brush.GetColour();
				brush.SetColour(color.Red()/2, color.Green()/2, color.Blue()/2);
			}
			
			dc.SetBrush(brush);
			dc.DrawRectangle(floor(squarex), floor(squarey), ceil(squarew)+1, ceil(squareh)+1);
		}
	}
	
	dc.SetBrush(*wxWHITE_BRUSH);
}

void WorldPanel::paintObjects(wxPaintDC &dc) {
	const WorldGrid &grid = world.getGrid();
	
	wxCoord dispw, disph;
	dc.GetSize(&dispw, &disph);
	
	const float squarew = (float)dispw/grid.getWidth();
	const float squareh = (float)disph/grid.getHeight();
	
	for (World::const_iterator i = world.begin(); i != world.end(); ++i) {
		if (const VictimWorldObject *victim = dynamic_cast<const VictimWorldObject *>(&*i)) {
			const float centerx = (victim->getPos().x+0.5f)*squarew;
			const float centery = (victim->getPos().y+0.5)*squareh;
			const float radius = min(squarew, squareh)*0.4f;
			dc.DrawCircle(centerx, centery, radius);
			
			if (robot.isVictimIdentified(victim->getPos())) {
				const float crossdelta = radius*0.3f;
				dc.DrawLine(centerx-crossdelta, centery-crossdelta, centerx+crossdelta, centery+crossdelta);
				dc.DrawLine(centerx-crossdelta, centery+crossdelta, centerx+crossdelta, centery-crossdelta);
			}
			
		} else if (const ObstacleWorldObject *obstacle = dynamic_cast<const ObstacleWorldObject *>(&*i)) {
			const float startx = (obstacle->getStartPos().x+0.5f)*squarew;
			const float starty = (obstacle->getStartPos().y+0.5f)*squareh;
			const float endx = (obstacle->getEndPos().x+0.5f)*squarew;
			const float endy = (obstacle->getEndPos().y+0.5f)*squareh;
			const float dir = atan2(endy-starty, endx - startx);
			
			const float thickness = (obstacle->isLarge() ? 0.3 : 0.1) * min(squarew, squareh);
			wxPoint points[4]; // points are counter-clockwise from top left of starting position
			points[0].x = (int)(startx + thickness*cos(dir + 3*M_PI/4));
			points[0].y = (int)(starty + thickness*sin(dir + 3*M_PI/4));
			points[1].x = (int)(startx + thickness*cos(dir - 3*M_PI/4));
			points[1].y = (int)(starty + thickness*sin(dir - 3*M_PI/4));
			points[2].x = (int)(endx + thickness*cos(dir - M_PI/4));
			points[2].y = (int)(endy + thickness*sin(dir - M_PI/4));
			points[3].x = (int)(endx + thickness*cos(dir + M_PI/4));
			points[3].y = (int)(endy + thickness*sin(dir + M_PI/4));
			dc.DrawPolygon(4, points);
		}
	}
}

void WorldPanel::paintRobot(wxPaintDC &dc) {
	const WorldGrid &grid = world.getGrid();
	
	wxCoord dispw, disph;
	dc.GetSize(&dispw, &disph);
	
	const float squarew = (float)dispw/grid.getWidth();
	const float squareh = (float)disph/grid.getHeight();
	
	const float startx = squarew*(robot.getPosition().x + 1);
	const float starty = squareh*(robot.getPosition().y + 1);
	const float thickness = min(squarew, squareh) * .3;
	const float dir = robot.getDirection();
	
	wxPoint points[3];
	points[0].x = (int)(startx + thickness*cos(dir));
	points[0].y = (int)(starty - thickness*sin(dir));
	points[1].x = (int)(startx + thickness*cos(dir + 5*M_PI/4));
	points[1].y = (int)(starty - thickness*sin(dir + 5*M_PI/4));
	points[2].x = (int)(startx + thickness*cos(dir - 5*M_PI/4));
	points[2].y = (int)(starty - thickness*sin(dir - 5*M_PI/4));
	dc.DrawPolygon(3, points);
	
	const float radius = max(min(squarew, squareh)*.1f, 3.0f);
	const RoomPlanner::Plan &plan = robot.getPlan();
	
	for (int i = 0; i != plan.coords.size(); ++i) {
		const Coord &c = plan.coords[i];
		Dir dir = plan.facedirs[i];
		float dirrad = dirToRad(dir);
		
		const float centerx = squarew*(c.x + 1);
		const float centery = squareh*(c.y + 1);
		dc.DrawCircle(centerx, centery, radius);
		
		const float len = min(squarew, squareh)*0.3;
		const float finalx = centerx+len*cos(dirrad);
		const float finaly = centery-len*sin(dirrad);
		dc.DrawLine(centerx, centery, finalx, finaly);
		dc.DrawCircle(finalx, finaly, 1);
	}
}

void WorldPanel::OnLeftDown(wxMouseEvent &event) {
	const WorldGrid &grid = world.getGrid();
	wxSize size = GetSize();
	const int gridx = event.GetX()*grid.getWidth()/size.GetWidth();
	const int gridy = event.GetY()*grid.getHeight()/size.GetHeight();

	Pos pos(gridx, gridy);
	dragging = callbacks.onWorldClicked(pos);
	if (dragging)
		lastdragpos = pos;
}

void WorldPanel::OnLeftUp(wxMouseEvent &event) {
	dragging = false;
}

void WorldPanel::OnMotion(wxMouseEvent &event) {
	if (!dragging)
		return;
		
	const WorldGrid &grid = world.getGrid();
	wxSize size = GetSize();
	int gridx = event.GetX()*grid.getWidth()/size.GetWidth();
	int gridy = event.GetY()*grid.getHeight()/size.GetHeight();

	if (gridx < 0)
		gridx = 0;
	else if (gridx >= grid.getWidth())
		gridx = grid.getWidth()-1;
		
	if (gridy < 0)
		gridy = 0;
	else if (gridy >= grid.getHeight())
		gridy = grid.getHeight()-1;

	Pos pos(gridx, gridy);
	if (lastdragpos != pos) {
		callbacks.onWorldDragged(pos);
		lastdragpos = pos;
	}
}


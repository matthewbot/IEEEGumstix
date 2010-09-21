#include "WorldPanel.h"
#include "VictimWorldObject.h"
#include "ObstacleWorldObject.h"
#include <algorithm>

using namespace pathsim;
using namespace std;

BEGIN_EVENT_TABLE(WorldPanel, wxPanel)
	EVT_PAINT(WorldPanel::OnPaint)
END_EVENT_TABLE()

WorldPanel::WorldPanel(wxWindow *parent, const World &world, const Robot &robot)
: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE), 
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
			float squarex = x * squarew;
			float squarey = y * squareh;
		
			switch (grid(x, y)) {
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
			
			if (robot.getMap().get(x, y) == WorldGrid::UNKNOWN) {
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
			dc.DrawCircle((victim->getX()+0.5f)*squarew, (victim->getY()+0.5)*squareh, min(squarew, squareh)*0.4f);
		} else if (const ObstacleWorldObject *obstacle = dynamic_cast<const ObstacleWorldObject *>(&*i)) {
			const float startx = (obstacle->getStartX()+0.5f)*squarew;
			const float starty = (obstacle->getStartY()+0.5f)*squareh;
			const float endx = (obstacle->getEndX()+0.5f)*squarew;
			const float endy = (obstacle->getEndY()+0.5f)*squareh;
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
	
	const float startx = squarew*(robot.getPosition().x + 0.5f);
	const float starty = squareh*(robot.getPosition().y + 0.5f);
	const float thickness = min(squarew, squareh) * .3;
	
	wxPoint points[3];
	points[0].x = (int)startx;
	points[0].y = (int)(starty - thickness);
	points[1].x = (int)(startx + thickness*cos(7*M_PI/6));
	points[1].y = (int)(starty - thickness*sin(7*M_PI/6));
	points[2].x = (int)(startx + thickness*cos(11*M_PI/6));
	points[2].y = (int)(starty - thickness*sin(11*M_PI/6));
	dc.DrawPolygon(3, points);
	
	const float radius = max(min(squarew, squareh)*.1f, 3.0f);
	for (AStarSearch::Route::const_iterator i = robot.getRoute().begin(); i != robot.getRoute().end(); ++i) {
		dc.DrawCircle(squarew*(i->x + 0.5f), squareh*(i->y + 0.5f), radius);
	}
}


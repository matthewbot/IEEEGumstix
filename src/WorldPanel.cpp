#include "WorldPanel.h"
#include "VictimWorldObject.h"
#include "ObstacleWorldObject.h"
#include <algorithm>

using namespace pathsim;
using namespace std;

BEGIN_EVENT_TABLE(WorldPanel, wxPanel)
	EVT_PAINT(WorldPanel::OnPaint)
END_EVENT_TABLE()

WorldPanel::WorldPanel(wxWindow *parent, const World &world)
: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE), 
  world(world) { }

void WorldPanel::OnPaint(wxPaintEvent &event) {
	wxPaintDC dc(this);
	
	paintGrid(dc);
	paintObjects(dc);
}

void WorldPanel::paintGrid(wxPaintDC &dc) {
	const WorldGrid &grid = world.getGrid();

	wxCoord dispw, disph;
	dc.GetSize(&dispw, &disph);
	
	const float squarew = (float)dispw/grid.getWidth();
	const float squareh = (float)disph/grid.getHeight();
	
	wxBrush victimbrush(wxColor(150, 200, 130));
	wxBrush smallobstaclebrush(wxColor(240, 240, 80));
	wxBrush largeobstaclebrush(wxColor(250, 100, 100));
	
	for (int x=0; x<grid.getWidth(); x++) {
		for (int y=0; y<grid.getHeight(); y++) {
			float squarex = x * squarew;
			float squarey = y * squareh;
		
			switch (grid(x, y)) {
				case WorldGrid::EMPTY:
					dc.SetBrush(*wxWHITE_BRUSH);
					break;
			
				case WorldGrid::SMALL_OBSTACLE:
					dc.SetBrush(smallobstaclebrush);
					break;
					
				case WorldGrid::VICTIM:
					dc.SetBrush(victimbrush);
					break;
					
				case WorldGrid::LARGE_OBSTACLE:
					dc.SetBrush(largeobstaclebrush);
					break;
			}
			
			dc.DrawRectangle(floor(squarex), floor(squarey), ceil(squarew)+1, ceil(squareh)+1);
		}
	}
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



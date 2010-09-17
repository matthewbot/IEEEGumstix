#include "WorldPanel.h"

using namespace pathsim;

BEGIN_EVENT_TABLE(WorldPanel, wxPanel)
	EVT_PAINT(WorldPanel::OnPaint)
END_EVENT_TABLE()

WorldPanel::WorldPanel(wxWindow *parent, const World &world)
: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE), 
  world(world) { }

void WorldPanel::OnPaint(wxPaintEvent &event) {
	const WorldGrid &grid = world.getGrid();
	const int gridw = grid.getWidth();
	const int gridh = grid.getHeight();
		
	wxPaintDC dc(this);
	wxCoord dispw, disph;
	dc.GetSize(&dispw, &disph);
	
	float squarew = (float)dispw/gridw;
	float squareh = (float)disph/gridh;
	
	wxBrush victimbrush(wxColor(150, 200, 130));
	wxBrush smallobstaclebrush(wxColor(240, 240, 80));
	wxBrush largeobstaclebrush(wxColor(250, 100, 100));
	
	for (int x=0; x<gridw; x++) {
		for (int y=0; y<gridh; y++) {
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


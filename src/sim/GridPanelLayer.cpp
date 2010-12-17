#include "ieee/sim/GridPanelLayer.h"

using namespace ieee;

GridPanelLayer::GridPanelLayer(const WorldGrid &grid, const CoordScale &gridscale)
: grid(grid), gridscale(gridscale) { }

int GridPanelLayer::getWeight() const { return WEIGHT; }

void GridPanelLayer::render(wxPaintDC &dc, const CoordScale &drawscale) const {
	wxBrush brush;

	for (int x=0; x<grid.getWidth(); x++) {
		for (int y=0; y<grid.getHeight(); y++) {
			Pos pos(x, y);
			WorldGrid::GridSquare square = grid[pos];

			static const wxColour squarecolors[] = {
				wxColour(255, 255, 255), // EMPTY
				wxColour(240, 240, 80), // SMALL_OBSTACLE
				wxColour(150, 200, 130), // VICTIM
				wxColour(250, 100, 100), // LARGE_OBSTACLE
				wxColour(120, 120, 120) // UNKNOWN
			};

			brush.SetColour(squarecolors[square]);
			dc.SetBrush(brush);

			Pos drawpos = drawscale.coordToPos(gridscale.posToCoord(pos));
			float width = drawscale.sx / gridscale.sx;
			float height = drawscale.sy / gridscale.sy;
			dc.DrawRectangle(drawpos.x - floor(width/2), drawpos.y - floor(height/2), ceil(width)+1, ceil(height)+1);
		}
	}
}


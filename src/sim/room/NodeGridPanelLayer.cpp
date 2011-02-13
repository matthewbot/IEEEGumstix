#include "ieee/sim/room/NodeGridPanelLayer.h"
#include <algorithm>

using namespace ieee;
using namespace std;

NodeGridPanelLayer::NodeGridPanelLayer(const NodeGrid &nodegrid, const CoordScale &nodescale)
: nodegrid(nodegrid),
  nodescale(nodescale) { }

int NodeGridPanelLayer::getWeight() const { return WEIGHT; }

void NodeGridPanelLayer::render(wxPaintDC &dc, const CoordScale &drawscale) const {
	const float radius = min(drawscale.sx, drawscale.sy)*2;

	wxBrush brush;
	for (int x=0; x<nodegrid.getWidth(); x++) {
		for (int y=0; y<nodegrid.getHeight(); y++) {
			Pos pos(x, y);
			Node::Type type = nodegrid[pos].getType();

			static const wxColour typecolors[] = {
				wxColour(255, 255, 255), // OPEN
				wxColour(240, 240, 80), // CLIMB
				wxColour(50, 50, 50), // IMPASSABLE
				wxColour(160, 160, 160) // UNKNOWN
			};

			brush.SetColour(typecolors[type]);
			dc.SetBrush(brush);

			Pos drawpos = drawscale.coordToPos(nodescale.posToCoord(pos));
			dc.DrawCircle(drawpos.x, drawpos.y, radius);
		}
	}

	dc.SetBrush(*wxWHITE_BRUSH);
}


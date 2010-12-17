#include "ieee/sim/WorldPanel.h"
#include "ieee/sim/VictimWorldObject.h"
#include "ieee/sim/ObstacleWorldObject.h"
#include <algorithm>

using namespace ieee;
using namespace std;

BEGIN_EVENT_TABLE(WorldPanel, wxPanel)
	EVT_LEFT_DOWN(WorldPanel::OnLeftDown)
	EVT_LEFT_UP(WorldPanel::OnLeftUp)
	EVT_MOTION(WorldPanel::OnMotion)
	EVT_PAINT(WorldPanel::OnPaint)
END_EVENT_TABLE()

WorldPanel::WorldPanel(wxWindow *parent)
: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE) { }

void WorldPanel::addLayer(WorldPanelLayer *layer) {
	int newweight = layer->getWeight();

	LayerVec::iterator i;
	for (i = layers.begin(); i != layers.end(); ++i) {
		if ((*i)->getWeight() < newweight)
			break;
	}

	layers.insert(i, layer);
}

void WorldPanel::removeLayer(WorldPanelLayer *layer) {
	LayerVec::iterator i = find(layers.begin(), layers.end(), layer);
	if (i != layers.end())
		layers.erase(i);
}

void WorldPanel::OnPaint(wxPaintEvent &event) {
	wxPaintDC dc(this);

	wxCoord w, h;
	GetSize(&w, &h);

	CoordScale drawscale(w/100.0f, h/100.0f);

	for (LayerVec::iterator i = layers.begin(); i != layers.end(); ++i) {
		(*i)->render(dc, drawscale);
		dc.SetBrush(*wxWHITE_BRUSH);
	}
}

void WorldPanel::OnLeftDown(wxMouseEvent &event) {
	wxCoord w, h;
	GetSize(&w, &h);

	CoordScale drawscale(w/100.0f, h/100.0f);

	Coord coord = drawscale.posToCoord(event.GetX(), event.GetY());

	for (LayerVec::iterator i = layers.begin(); i != layers.end(); ++i) {
		if ((*i)->leftDown(coord))
			break;
	}
}

void WorldPanel::OnLeftUp(wxMouseEvent &event) {
	for (LayerVec::iterator i = layers.begin(); i != layers.end(); ++i) {
		(*i)->leftUp();
	}
}

void WorldPanel::OnMotion(wxMouseEvent &event) {
	wxCoord w, h;
	GetSize(&w, &h);

	CoordScale drawscale(w/100.0f, h/100.0f);

	Coord coord = drawscale.posToCoord(event.GetX(), event.GetY());

	for (LayerVec::iterator i = layers.begin(); i != layers.end(); ++i) {
		(*i)->mouseMotion(coord);
	}
}


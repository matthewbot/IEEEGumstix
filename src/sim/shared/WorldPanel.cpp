#include "ieee/sim/shared/WorldPanel.h"
#include <algorithm>

using namespace ieee;
using namespace std;

BEGIN_EVENT_TABLE(WorldPanel, wxPanel)
	EVT_LEFT_DOWN(WorldPanel::OnLeftDown)
	EVT_LEFT_UP(WorldPanel::OnLeftUp)
	EVT_MOTION(WorldPanel::OnMotion)
	EVT_PAINT(WorldPanel::OnPaint)
END_EVENT_TABLE()

WorldPanel::WorldPanel(wxWindow *parent, float roomwidth, float roomheight)
: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE),
  roomwidth(roomwidth),
  roomheight(roomheight) { }

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

bool WorldPanel::hasLayer(WorldPanelLayer *layer) {
	LayerVec::iterator i = find(layers.begin(), layers.end(), layer);
	return i != layers.end();
}

void WorldPanel::OnPaint(wxPaintEvent &event) {
	wxPaintDC dc(this);
	CoordScale drawscale = getDrawScale();

	for (LayerVec::iterator i = layers.begin(); i != layers.end(); ++i) {
		(*i)->render(dc, drawscale);
		dc.SetBrush(*wxWHITE_BRUSH);
	}
}

void WorldPanel::OnLeftDown(wxMouseEvent &event) {
	Coord coord = getDrawScale().posToCoord(event.GetX(), event.GetY());
	bool redraw = false;
	for (LayerVec::iterator i = layers.begin(); i != layers.end(); ++i) {
		if ((*i)->leftDown(coord)) {
			redraw = true;
			break;
		}
	}

	if (redraw)
		Refresh();
}

void WorldPanel::OnLeftUp(wxMouseEvent &event) {
	Coord coord = getDrawScale().posToCoord(event.GetX(), event.GetY());
	bool redraw = false;
	for (LayerVec::iterator i = layers.begin(); i != layers.end(); ++i) {
		if ((*i)->leftUp(coord))
			redraw = true;
	}

	if (redraw)
		Refresh();
}

void WorldPanel::OnMotion(wxMouseEvent &event) {
	Coord coord = getDrawScale().posToCoord(event.GetX(), event.GetY());
	bool redraw = false;
	for (LayerVec::iterator i = layers.begin(); i != layers.end(); ++i) {
		if ((*i)->mouseMotion(coord))
			redraw = true;
	}

	if (redraw)
		Refresh();
}

CoordScale WorldPanel::getDrawScale() const {
	wxCoord w, h;
	GetSize(&w, &h);
	return CoordScale(w/roomwidth, h/roomheight);
}


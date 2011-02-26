#ifndef WORLDPANELLAYER_H
#define WORLDPANELLAYER_H

#include <ieee/shared/types.h>
#include <wx/wx.h>

namespace ieee {
	class WorldPanelLayer {
		public:
			virtual int getWeight() const = 0;
			virtual void render(wxPaintDC &dc, const CoordScale &drawscale) const = 0;

			// all of these return true to request a redraw
			virtual bool leftDown(const Coord &coord); // returning true will also cause the event to not propogate to remaining layers
			virtual bool leftUp(const Coord &coord);
			virtual bool mouseMotion(const Coord &coord);
	};
}

#endif

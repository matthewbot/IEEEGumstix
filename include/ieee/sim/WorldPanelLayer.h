#ifndef WORLDPANELLAYER_H
#define WORLDPANELLAYER_H

#include <ieee/shared/types.h>
#include <wx/wx.h>

namespace ieee {
	class WorldPanelLayer {
		public:
			virtual int getWeight() const = 0;
			virtual void render(wxPaintDC &dc, const CoordScale &drawscale) const = 0;

			virtual bool leftDown(const Coord &coord); // default empty implementations
			virtual void leftUp();
			virtual void mouseMotion(const Coord &coord);
	};
}

#endif

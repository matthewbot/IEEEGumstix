#ifndef GRIDPANELLAYER_H
#define GRIDPANELLAYER_H

#include "ieee/sim/WorldPanelLayer.h"
#include "ieee/shared/WorldGrid.h"

namespace ieee {
	class GridPanelLayer : public WorldPanelLayer {
		public:
			GridPanelLayer(const WorldGrid &grid, const CoordScale &gridscale);

			static const int WEIGHT = 10;
			virtual int getWeight() const;
			virtual void render(wxPaintDC &dc, const CoordScale &drawscale) const;

		private:
			const WorldGrid &grid;
			const CoordScale &gridscale;

	};
}

#endif

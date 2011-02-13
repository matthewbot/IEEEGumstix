#ifndef NODEGRIDPANELLAYER_H
#define NODEGRIDPANELLAYER_H

#include "ieee/planner/NodeGrid.h"
#include "ieee/sim/shared/WorldPanelLayer.h"

namespace ieee {
	class NodeGridPanelLayer : public WorldPanelLayer {
		public:
			NodeGridPanelLayer(const NodeGrid &nodegrid, const CoordScale &nodescale);

			static const int WEIGHT = 10;
			virtual int getWeight() const;
			virtual void render(wxPaintDC &dc, const CoordScale &drawscale) const;

		private:
			const NodeGrid &nodegrid;
			const CoordScale &nodescale;

	};
}

#endif

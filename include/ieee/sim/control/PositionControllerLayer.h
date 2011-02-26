#ifndef IEEE_POSITIONCONTROLLERLAYER_H
#define IEEE_POSITIONCONTROLLERLAYER_H

#include "ieee/sim/shared/WorldPanelLayer.h"
#include "ieee/controls/PositionController.h"

namespace ieee {
	class PositionControllerLayer : public WorldPanelLayer {
		public:
			PositionControllerLayer(const PositionController &poscontrol);

			static const int WEIGHT = 0;
			virtual int getWeight() const; // WorldPanelLayer
			virtual void render(wxPaintDC &dc, const CoordScale &drawscale) const;

		private:
			void renderRobot(wxPaintDC &dc, const CoordScale &drawscale) const;

			const PositionController &poscontrol;
	};
}

#endif


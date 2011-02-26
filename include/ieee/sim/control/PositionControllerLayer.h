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
			virtual bool leftDown(const Coord &pos);
			virtual bool mouseMotion(const Coord &pos);
			virtual bool leftUp(const Coord &pos);

		private:
			void renderRobot(wxPaintDC &dc, const CoordScale &drawscale) const;
			void renderDragCommand(wxPaintDC &dc, const CoordScale &drawscale) const;

			const PositionController &poscontrol;

			bool dragging;
			Coord startcoord;
			Coord endcoord;
	};
}

#endif


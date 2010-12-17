#ifndef ROBOTPANELLAYER_H
#define ROBOTPANELLAYER_H

#include "ieee/sim/WorldPanelLayer.h"
#include "ieee/sim/Robot.h"

namespace ieee {
	class RobotPanelLayer : public WorldPanelLayer {
		public:
			RobotPanelLayer(const Robot &robot, const CoordScale &gridscale);

			static const int WEIGHT = 0;
			virtual int getWeight() const;
			virtual void render(wxPaintDC &dc, const CoordScale &drawscale) const;

		private:
			const Robot &robot;
			const CoordScale &gridscale;
	};
}

#endif

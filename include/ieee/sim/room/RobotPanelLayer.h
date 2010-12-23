#ifndef ROBOTPANELLAYER_H
#define ROBOTPANELLAYER_H

#include "ieee/sim/room/Robot.h"
#include "ieee/sim/shared/WorldPanelLayer.h"

namespace ieee {
	class RobotPanelLayer : public WorldPanelLayer {
		public:
			RobotPanelLayer(const Robot &robot, const CoordScale &gridscale, const CoordScale &victimscale);

			static const int WEIGHT = 0;
			virtual int getWeight() const;
			virtual void render(wxPaintDC &dc, const CoordScale &drawscale) const;

		private:
			const Robot &robot;
			const CoordScale &gridscale;
			const CoordScale &victimscale;
	};
}

#endif

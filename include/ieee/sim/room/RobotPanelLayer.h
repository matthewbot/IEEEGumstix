#ifndef ROBOTPANELLAYER_H
#define ROBOTPANELLAYER_H

#include "ieee/sim/room/Robot.h"
#include "ieee/sim/shared/WorldPanelLayer.h"

namespace ieee {
	class RobotPanelLayer : public WorldPanelLayer {
		public:
			RobotPanelLayer(const Robot &robot, const CoordScale &victimscale, const CoordScale &nodescale);

			static const int WEIGHT = 0;
			virtual int getWeight() const;
			virtual void render(wxPaintDC &dc, const CoordScale &drawscale) const;

		private:
			void drawRobot(wxPaintDC &dc, const CoordScale &drawscale) const;
			void drawPlan(wxPaintDC &dc, const CoordScale &drawscale) const;
			void drawIdentifiedVictims(wxPaintDC &dc, const CoordScale &drawscale) const;
			void drawNodeGrid(wxPaintDC &dc, const CoordScale &drawscale) const;

			const Robot &robot;
			const CoordScale &victimscale;
			const CoordScale &nodescale;
	};
}

#endif

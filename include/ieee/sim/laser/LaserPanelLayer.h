#ifndef LASERPANELLAYER_H
#define LASERPANELLAYER_H

#include "ieee/drivers/LaserSensor.h"
#include "ieee/sim/shared/WorldPanelLayer.h"

namespace ieee {
	class LaserPanelLayer : public WorldPanelLayer {
		public:
			LaserPanelLayer(const LaserSensor::Readings &readings);

			static const int WEIGHT = 5;
			virtual int getWeight() const;
			virtual void render(wxPaintDC &dc, const CoordScale &drawscale) const;

		private:
			const LaserSensor::Readings &readings;
	};
}

#endif


#ifndef CAMERASENSORPREDICTOR_H
#define CAMERASENSORPREDICTOR_H

#include "SensorPredictor.h"

namespace ieee {
	class CameraSensorPredictor : public SensorPredictor {
		public:
			CameraSensorPredictor(int maxdistance, float fieldofview, float fovstep);
			
			virtual PosSet predictVision(const Coord &curpos, float curdir, const WorldGrid &grid, const CoordScale &scale) const;
		private:
			float maxdistance;
			float fovstep;
			float fieldofview;
	};
}

#endif


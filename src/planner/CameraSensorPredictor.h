#ifndef CAMERASENSORPREDICTOR_H
#define CAMERASENSORPREDICTOR_H

#include "SensorPredictor.h"

namespace ieeepath {
	class CameraSensorPredictor : public SensorPredictor {
		public:
			CameraSensorPredictor(int maxdistance, float fieldofview, float fovstep);
			
			virtual PosSet predictVision(const Pos &curpos, Dir curdir, const WorldGrid &grid) const;
		private:
			float maxdistance;
			float fovstep;
			float fieldofview;
	};
}

#endif


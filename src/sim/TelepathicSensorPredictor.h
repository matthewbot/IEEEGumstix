#ifndef TELEPATHICSENSORPREDICTOR_H
#define TELEPATHICSENSORPREDICTOR_H

#include "SensorPredictor.h"

namespace ieeepath {
	class TelepathicSensorPredictor : public SensorPredictor {
		public:
			TelepathicSensorPredictor(int range);
			
			virtual PosSet predictVision(const Pos &curpos, Dir curdir, const WorldGrid &grid) const;
		private:
			int range;
	};
}

#endif

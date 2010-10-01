#ifndef TELEPATHICSENSORPREDICTOR_H
#define TELEPATHICSENSORPREDICTOR_H

#include "SensorPredictor.h"

namespace pathsim {
	class TelepathicSensorPredictor : public SensorPredictor {
		public:
			TelepathicSensorPredictor(int range);
			
			virtual PosSet predictVision(const Pos &curpos, const WorldGrid &grid) const;
		private:
			int range;
	};
}

#endif

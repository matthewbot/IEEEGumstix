#ifndef DRIVEEQUATION_H
#define DRIVEEQUATION_H

#include "ieee/controls/Vec2D.h"
#include "ieee/drivers/WheelsDriver.h"

namespace ieee {
	class DriveEquation {
		public:
			struct WheelConfig {
				Vec2D relpos;

				float outscale;
				float outoffset;
			};

			struct Config {
				WheelConfig left, right, back;

				float rotationoffset; // wheels are rotated this much relative to the frame
				float minspeed; // speeds below this the wheel will be given 0 effort
			};

			struct Motion {
				Vec2D vel;

				float curangle; // orientation of robot
				float angvel;
			};

			DriveEquation(const Config &config);

			WheelsDriver::Output compute(const Motion &motion) const;

		private:
			const Config &config;

			WheelsDriver::WheelOutput computeWheel(const WheelConfig &config, const Motion &motion) const;
	};
}

#endif

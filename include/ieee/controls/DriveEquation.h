#ifndef DRIVEEQUATION_H
#define DRIVEEQUATION_H

#include "ieee/controls/Vec2D.h"
#include "ieee/controls/Angle.h"
#include "ieee/drivers/avr/WheelsControl.h"
#include <boost/property_tree/ptree.hpp>

namespace ieee {
	class DriveEquation {
		public:
			struct WheelConfig {
				Vec2D framepos;

				float effortscale;
				float effortoffset;

				void readTree(const boost::property_tree::ptree &pt);
			};

			struct Config {
				WheelConfig left, right, back;

				Angle wheelangleoffset; // wheels are rotated this much relative to the frame
				float minspeed; // speeds below this the wheel will be disabled

				void readTree(const boost::property_tree::ptree &pt);
			};

			struct Motion {
				Vec2D vel;
				Angle curdir; // orientation of robot
				float angvel;

				static const Motion stop;
			};

			DriveEquation(const Config &config);

			WheelsControl::Output compute(const Motion &motion) const;

		private:
			const Config &config;

			WheelsControl::WheelOutput computeWheel(const WheelConfig &config, const Motion &motion) const;
	};
}

#endif

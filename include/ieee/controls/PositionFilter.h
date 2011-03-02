#ifndef IEEE_POSITIONFILTER_H
#define IEEE_POSITIONFILTER_H

#include "ieee/controls/Vec2D.h"
#include "ieee/drivers/avr/SonarSensor.h"

namespace ieee {
	class PositionFilter {
		public:
			struct Config {
				float roomwidth;
				float roomheight;

				Vec2D sonaroffset; // sonar position relative to the robot
				float sonarstepperrad; // sonar distance from center of stepper

				float sonarmindist; // sonars are always kept pointing farther than this from the wall
			};

			PositionFilter(const Config &config);

			enum SonarDir {
				SONARDIR_EAST,
				SONARDIR_NORTH,
				SONARDIR_SOUTH,
				SONARDIR_WEST,
				SONARDIR_INDETERMINATE // sonars are too far from the desired position
			};

			static float sonarDirToRad(SonarDir dir);
			static SonarDir radToSonarDir(float rad);

			struct Input {
				SonarDir cursonardir;
				SonarSensor::Reading sonar1;
				SonarSensor::Reading sonar2;

				float compassdir;
			};

			struct Output {
				Vec2D pos;
				float dir;
				bool ok;

				SonarDir sonardir;
			};

			const Output &update(const Input &input);
			inline const Output &getOutput() const { return output; }

		private:
			Output output;
			const Config &config;

			Vec2D lastsonarpos;
			Vec2D updateSonarPos(const Input &input);
			SonarDir computeSonarDir(const Vec2D &pos, SonarDir curdir) const;
	};
}

#endif

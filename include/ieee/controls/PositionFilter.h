#ifndef IEEE_POSITIONFILTER_H
#define IEEE_POSITIONFILTER_H

#include "ieee/controls/Vec2D.h"
#include "ieee/controls/Angle.h"
#include "ieee/drivers/avr/SonarSensor.h"
#include <boost/property_tree/ptree.hpp>
#include <vector>

namespace ieee {
	class PositionFilter {
		public:
			struct Config {
				float roomwidth;
				float roomheight;

				Vec2D sonarposoffset; // sonar position relative to the robot
				float sonarmountradius; // sonar distance from center of mount
				float sonarturnmindist; // sonars are always kept pointing farther than this from the wall

				int posbufsize; // this many position samples are averaged to produce our current output

				void readTree(const boost::property_tree::ptree &pt);
			};

			PositionFilter(const Config &config);

			enum SonarDir {
				SONARDIR_EAST,
				SONARDIR_NORTH,
				SONARDIR_WEST,
				SONARDIR_SOUTH,
				SONARDIR_INDETERMINATE // sonars are too far from the desired position
			};

			static Angle sonarDirToAngle(SonarDir dir);
			static SonarDir angleToSonarDir(Angle angle);

			struct Input {
				SonarDir cursonardir;
				float sonar1;
				float sonar2;

				Angle compassheading;
			};

			void update(const Input &input);

			inline const Vec2D &getPosition() const { return position; }
			inline bool getPositionOk() const { return positionok; }
			inline Angle getHeading() const { return heading; }
			inline SonarDir getDesiredSonarDir() const { return desiredsonardir; }
			inline Angle getDesiredSonarAngle() const { return sonarDirToAngle(desiredsonardir); }

		private:
			const Config &config;

			Vec2D position;
			bool positionok;
			Angle heading;
			SonarDir desiredsonardir;

			std::vector<Vec2D> posbuf;

			void updateHeading(const Input &input);
			void updatePos(const Input &input);
			void updateDesiredSonarDir(const Input &input);

			Vec2D computeSonarPos(const Input &input) const;
	};
}

#endif

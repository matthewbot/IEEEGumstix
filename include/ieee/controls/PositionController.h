#ifndef POSITIONCONTROLLER_H
#define POSITIONCONTROLLER_H

#include "ieee/controls/PositionFilter.h"
#include "ieee/controls/DriveEquation.h"
#include "ieee/drivers/avr/AVRRobot.h"
#include <boost/property_tree/ptree.hpp>

namespace ieee {
	class PositionController {
		public:
			struct Config {
				PositionFilter::Config posfilter;
				DriveEquation::Config driveequ;

				float stopdist; // stop within this distance

				float turnminangdiff; // turn in place when we're this many radians off
				float turnangvel; // speed of turn in place radians/sec

				float lockdist; // the desired desired velocity vector won't be recomputed as long as we're closer than this distance
				float lockmaxangdiff; // the maximum difference between the desired velocity and the locked velocity before we're temporarily "unlocked"
				float lockspeed; // speed we use when in lock mode

				float driveangvelfactor; // angular error * factor = desired angular velocity
				float drivemaxangvel; // angular velocity never exceeds this

				void readTree(const boost::property_tree::ptree &pt);
			};

			PositionController(const Config &config);

			inline PositionFilter &getPositionFilter() { return posfilter; }
			inline const PositionFilter &getPositionFilter() const { return posfilter; }

			struct Command {
				Vec2D destpos;
				Angle destheading;
				float speed;

				inline Command() { }
				inline Command(const Vec2D &destpos, float speed) : destpos(destpos), speed(speed) { }
			};

			void setCommand(const Command &command);
			inline const Command &getCommand() const { return command; }

			enum State {
				STOPPED,
				DRIVE,
				LOCK,
				TURN,
				DONE
			};

			static const char *stateToString(State state);

			inline void stop() { state = STOPPED; }
			inline State getState() const { return state; }

			void update(AVRRobot &robot);
			inline const DriveEquation::Motion &getMotion() const { return motion; }

		private:
			void updatePositionFilter(AVRRobot &robot);
			void updateMotion();

			const Config &config;
			Command command;

			DriveEquation driveequ;
			PositionFilter posfilter;

			State state;
			Vec2D lockvec;
			DriveEquation::Motion motion;
	};
}

#endif


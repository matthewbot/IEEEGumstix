#ifndef POSITIONCONTROLLER_H
#define POSITIONCONTROLLER_H

#include "ieee/controls/PositionFilter.h"
#include "ieee/controls/DriveEquation.h"
#include "ieee/drivers/avr/AVRRobot.h"

namespace ieee {
	class PositionController {
		public:
			struct Config {
				PositionFilter::Config posfilter;
				DriveEquation::Config driveequ;

				float lockdist; // the desired desired velocity vector won't be recomputed as long as we're closer than this distance
				float lockangdiff; // the maximum difference between the desired velocity and the locked velocity before we're temporarily "unlocked"
				float stopdist;
			};

			PositionController(const Config &config);

			struct Command {
				Vec2D destpos;
				float speed;

				inline Command() { }
				inline Command(const Vec2D &destpos, float speed) : destpos(destpos), speed(speed) { }
			};

			void setCommand(const Command &command);
			inline const Command &getCommand() const { return command; }

			inline bool getStarted() const { return start; }
			void stop();

			inline const PositionFilter &getPositionFilter() const { return posfilter; }

			void update(AVRRobot &robot);

		private:
			const Config &config;
			Command command;

			DriveEquation driveequ;
			PositionFilter posfilter;

			bool start;
			Vec2D lastvelvec;

			void updatePositionFilter(AVRRobot &robot);
			Vec2D computeVelocityVector();
	};
}

#endif


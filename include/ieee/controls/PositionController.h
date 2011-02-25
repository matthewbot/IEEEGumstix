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
	};
}

#endif


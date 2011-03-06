#ifndef STEPPERCONTROL_H
#define STEPPERCONTROL_H

#include "ieee/drivers/avr/SerialPackets.h"

namespace ieee {
	class StepperControl {
		public:
			struct Config {
				float stepsize; // size of a step in radians
			};

			StepperControl(const Config &config);

			void setAngle(GumstixPacket &gp, float angle) const;
			float getCurAngle(const AVRPacket &ap) const;
			void setEnabled(GumstixPacket &gp, bool enabled) const;

		private:
			const Config &config;
	};
}

#endif

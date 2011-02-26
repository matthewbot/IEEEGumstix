#ifndef IEEE_COMPASSSENSOR_H
#define IEEE_COMAPSSSENSOR_H

#include "ieee/drivers/avr/SerialPackets.h"

namespace ieee {
	class CompassSensor {
		public:
			struct Config {
				int16_t centerx;
				int16_t centery;
				float yscale; // scale of y axis with respect to x axis
			};

			CompassSensor(const Config &config);

			float getAngle(const AVRPacket &ap) const;

			void calibrateOffset(const AVRPacket &ap, float desiredangle);

		private:
			const Config &config;
			float offset;
	};
}

#endif
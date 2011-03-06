#ifndef IEEE_SONARDRIVER_H
#define IEEE_SONARDRIVER_H

#include "ieee/drivers/avr/SerialPackets.h"
#include <vector>
#include <utility>
#include <stdint.h>

namespace ieee {
	class SonarSensor {
		public:
			struct Config {
				float alpha, beta; // distance = reading*alpha + beta
			};

			enum ReadingStatus {
				READING_SHORT,
				READING_GOOD,
				READING_FAR
			};

			SonarSensor(const Config &config, int num); // num for sonar 1 or sonar 2

			float getReading(const AVRPacket &ap) const;

		private:
			const Config &config;
			int num;
	};
}

#endif

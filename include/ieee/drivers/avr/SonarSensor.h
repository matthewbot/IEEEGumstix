#ifndef IEEE_SONARDRIVER_H
#define IEEE_SONARDRIVER_H

#include "ieee/drivers/avr/SerialPackets.h"
#include <vector>
#include <utility>
#include <stdint.h>

namespace ieee {
	class SonarSensor {
		public:
			struct CalPoint {
				uint16_t reading;
				float dist;
			};

			struct Config {
				std::vector<CalPoint> calpoints;
			};

			enum ReadingStatus {
				READING_SHORT,
				READING_GOOD,
				READING_FAR
			};

			SonarSensor(const Config &config, int num); // num for sonar 1 or sonar 2

			std::pair<float, ReadingStatus> getReading(const AVRPacket &ap) const;

		private:
			const Config &config;
			int num;
	};
}

#endif

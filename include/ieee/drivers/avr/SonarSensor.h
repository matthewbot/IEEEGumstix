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

				int16_t short_reading;
				int16_t far_reading;
			};

			enum ReadingStatus {
				READING_SHORT,
				READING_GOOD,
				READING_FAR
			};

			SonarSensor(const Config &config, int num); // num for sonar 1 or sonar 2

			struct Reading {
				float dist;
				ReadingStatus status;

				inline Reading() { }
				inline Reading(float dist, ReadingStatus status) : dist(dist), status(status) { }
			};

			Reading getReading(const AVRPacket &ap) const;

		private:
			const Config &config;
			int num;
	};
}

#endif

#include "ieee/drivers/avr/SonarSensor.h"

using namespace ieee;
using namespace std;

SonarSensor::SonarSensor(const Config &config, int num) : config(config), num(num) { }

SonarSensor::DistanceStatus SonarSensor::getReading(const AVRPacket &ap) const {
	const std::vector<CalPoint> &calpoints = config.calpoints;
	uint16_t reading = (num == 1) ? ap.sonar1_reading : ap.sonar2_reading;

	if (reading < calpoints.front().reading) {
		return make_pair(calpoints.front().dist, READING_SHORT);
	} else if (reading > calpoints.back().reading) {
		return make_pair(calpoints.back().dist, READING_FAR);
	}

	std::vector<CalPoint>::const_iterator calentry;
	for (calentry = calpoints.begin(); calentry != calpoints.end()-1; ++calentry) {
		if (reading < calentry[1].reading)
			break;
	}

	const CalPoint &first = calentry[0];
	const CalPoint &second = calentry[1];

	float ratio = (float)(reading - first.reading) / (second.reading - first.reading);
	float dist = first.dist + ratio*(second.dist - first.dist);

	return make_pair(dist, READING_GOOD);
}


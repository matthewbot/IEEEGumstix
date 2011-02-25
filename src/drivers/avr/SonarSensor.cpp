#include "ieee/drivers/avr/SonarSensor.h"

using namespace ieee;
using namespace std;

SonarSensor::SonarSensor(const Config &config, int num) : config(config), num(num) { }

SonarSensor::Reading SonarSensor::getReading(const AVRPacket &ap) const {
	uint16_t reading = (num == 1) ? ap.sonar1_reading : ap.sonar2_reading;
	float dist = config.alpha * reading + config.beta;

	ReadingStatus status;
	if (reading < config.short_reading)
		status = READING_SHORT;
	else if (reading > config.far_reading)
		status = READING_FAR;
	else
		status = READING_GOOD;

	return Reading(dist, status);
}


#include "ieee/drivers/avr/SonarSensor.h"

using namespace ieee;
using namespace std;

SonarSensor::SonarSensor(const Config &config, int num) : config(config), num(num) { }

float SonarSensor::getReading(const AVRPacket &ap) const {
	uint16_t reading = (num == 1) ? ap.sonar1_reading : ap.sonar2_reading;
	return config.alpha * reading + config.beta;
}


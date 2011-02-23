#include "ieee/drivers/avr/CompassSensor.h"
#include <cmath>

using namespace ieee;
using namespace std;

CompassSensor::CompassSensor(const Config &config) : config(config), offset(0) { }

float CompassSensor::getAngle(const AVRPacket &ap) const {
	float xaxis = ap.mag_x - config.centerx;
	float yaxis = (ap.mag_y - config.centery)*config.yscale;
	return atan2(yaxis, xaxis) + offset;
}

void CompassSensor::calibrateOffset(const AVRPacket &ap, float desiredangle) {
	offset = 0;
	float rawangle = getAngle(ap);
	offset = desiredangle - rawangle;
}


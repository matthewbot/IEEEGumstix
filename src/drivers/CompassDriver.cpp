#include "ieee/drivers/CompassDriver.h"
#include <cmath>

using namespace ieee;
using namespace std;

CompassDriver::CompassDriver(const Config &config) : config(config), offset(0) { }

float CompassDriver::getAngle(const AVRPacket &ap) const {
	float xaxis = ap.mag_x - config.centerx;
	float yaxis = (ap.mag_y - config.centery)*config.yscale;
	return atan2(yaxis, xaxis) + offset;
}

void CompassDriver::calibrateOffset(const AVRPacket &ap, float desiredangle) {
	offset = 0;
	float rawangle = getAngle(ap);
	offset = desiredangle - rawangle;
}


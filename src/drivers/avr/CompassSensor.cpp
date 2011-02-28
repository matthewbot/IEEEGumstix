#include "ieee/drivers/avr/CompassSensor.h"
#include <cmath>

using namespace ieee;
using namespace std;

CompassSensor::CompassSensor(const Config &config) : config(config), offset(0) { }

float CompassSensor::getAngle(const AVRPacket &ap, const WheelsControl &wheels) const {
	WheelsControl::Output wheelsout = wheels.readOutput(ap);

	float magx = ap.mag_x;
	float magy = ap.mag_y;

	magx -= polyval(config.leftwheel_offset.magx, angleWrap(wheelsout.left.angle));
	magx -= polyval(config.rightwheel_offset.magx, angleWrap(wheelsout.right.angle));
	magx -= polyval(config.backwheel_offset.magx, angleWrap(wheelsout.back.angle));

	magy -= polyval(config.leftwheel_offset.magy, angleWrap(wheelsout.left.angle));
	magy -= polyval(config.rightwheel_offset.magy, angleWrap(wheelsout.right.angle));
	magy -= polyval(config.backwheel_offset.magy, angleWrap(wheelsout.back.angle));

	float xaxis = magx - config.centerx;
	float yaxis = (magy - config.centery)*config.yscale;
	return atan2(yaxis, xaxis) + offset;
}

void CompassSensor::calibrateOffset(const AVRPacket &ap, const WheelsControl &wheels, float desiredangle) {
	offset = 0;
	float rawangle = getAngle(ap, wheels);
	offset = desiredangle - rawangle;
}

float CompassSensor::polyval(const std::vector<float> &coefs, float val) {
	float out = 0;
	for (int i=0; i<coefs.size(); i++) {
		out += coefs[i] * pow(val, coefs.size()-i-1);
	}

	return out;
}

float CompassSensor::angleWrap(float angle) {
	if (angle > M_PI)
		return angle - M_PI;
	else
		return angle;
}

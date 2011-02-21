#include "ieee/drivers/WheelsDriver.h"
#include <cmath>

using namespace ieee;
using namespace std;

WheelsDriver::WheelsDriver(const Config &config) : config(config) { }

void WheelsDriver::writeOutput(const Output &output, GumstixPacket &gp) const {
	gp.leftwheel_angle = toRawAngle(output.left.angle);
	gp.rightwheel_angle = toRawAngle(output.right.angle);
	gp.backwheel_angle = toRawAngle(output.back.angle);

	gp.leftwheel_speed = toRawSpeed(output.left.effort, output.left.angle);
	gp.rightwheel_speed = toRawSpeed(output.right.effort, output.right.angle);
	gp.backwheel_speed = toRawSpeed(output.back.effort, output.back.angle);
}

int16_t WheelsDriver::toRawAngle(float angle) {
	int val = (int)(angle/M_PI*1800);
	if (val < 0)
		val += 1800;
	else if (val > 1800)
		val -= 1800;
	return (int16_t)val;
}

int16_t WheelsDriver::toRawSpeed(float speed, float angle) {
	if (angle < 0 || angle > M_PI)
		speed = -speed;
	return (int16_t)(speed*1000);
}


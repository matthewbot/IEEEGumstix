#include "ieee/drivers/avr/WheelsControl.h"
#include <cmath>

using namespace ieee;
using namespace std;

WheelsControl::WheelsControl(const Config &config) : config(config) { }

void WheelsControl::writeOutput(const Output &output, GumstixPacket &gp) const {
	gp.servos_enabled = true;

	gp.leftwheel_angle = toRawAngle(output.left.angle, config.left);
	gp.rightwheel_angle = toRawAngle(output.right.angle, config.right);
	gp.backwheel_angle = toRawAngle(output.back.angle, config.back);

	gp.leftwheel_speed = toRawSpeed(output.left.effort, output.left.angle);
	gp.rightwheel_speed = toRawSpeed(output.right.effort, output.right.angle);
	gp.backwheel_speed = toRawSpeed(output.back.effort, output.back.angle);
}

int16_t WheelsControl::toRawAngle(float angle, const WheelConfig &wconf) {
	int val = (int)(angle/M_PI*1800);
	if (val < 0)
		val += 1800;
	else if (val > 1800)
		val -= 1800;

	if (val < wconf.minstop)
		return wconf.minstop;
	else if (val > wconf.maxstop)
		return wconf.maxstop;
	else
		return (int16_t)val;
}

int16_t WheelsControl::toRawSpeed(float speed, float angle) {
	if (angle < 0 || angle > M_PI)
		speed = -speed;
	return (int16_t)(speed*1000);
}


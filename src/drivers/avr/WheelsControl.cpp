#include "ieee/drivers/avr/WheelsControl.h"
#include <cmath>

using namespace ieee;
using namespace std;

WheelsControl::WheelsControl(const Config &config) : config(config) { }

void WheelsControl::writeOutput(const Output &output, GumstixPacket &gp) const {
	gp.enable_bits |= ENABLE_SERVOS;

	gp.leftwheel_angle = toRawAngle(output.left.angle, config.left);
	gp.rightwheel_angle = toRawAngle(output.right.angle, config.right);
	gp.backwheel_angle = toRawAngle(output.back.angle, config.back);

	gp.leftwheel_effort = toRawEffort(output.left.effort, output.left.angle);
	gp.rightwheel_effort = toRawEffort(output.right.effort, output.right.angle);
	gp.backwheel_effort = toRawEffort(output.back.effort, output.back.angle);
}

WheelsControl::Output WheelsControl::readOutput(const AVRPacket &ap) const {
	Output out;
	out.left.angle = fromRawAngle(ap.leftwheel_angle, ap.leftwheel_effort, config.left);
	out.right.angle = fromRawAngle(ap.rightwheel_angle, ap.rightwheel_effort, config.right);
	out.back.angle = fromRawAngle(ap.backwheel_angle, ap.backwheel_effort, config.back);

	out.left.effort = fromRawEffort(ap.leftwheel_effort);
	out.right.effort = fromRawEffort(ap.rightwheel_effort);
	out.back.effort = fromRawEffort(ap.backwheel_effort);

	return out;
}

int16_t WheelsControl::toRawAngle(float angle, const WheelConfig &wconf) {
	int val = (int)(angle/M_PI*1800);
	if (val < 0)
		val += 1800;
	else if (val >= 1800)
		val -= 1800;

	val += wconf.offset;

	if (val < wconf.minstop)
		return wconf.minstop;
	else if (val > wconf.maxstop)
		return wconf.maxstop;
	else
		return (int16_t)val;
}

int16_t WheelsControl::toRawEffort(float speed, float angle) {
	if (angle < 0 || angle > M_PI)
		speed = -speed;
	return (int16_t)(speed*1000);
}

float WheelsControl::fromRawAngle(int16_t rawangle, int16_t raweffort, const WheelConfig &wconf) {
	float angle = (rawangle - wconf.offset) / 1800.0 * M_PI;
	if (raweffort < 0)
		return angle + M_PI;
	else
		return angle;
}

float WheelsControl::fromRawEffort(int16_t raweffort) {
	return abs(raweffort*1000.0f);
}

WheelsControl::WheelOutput &WheelsControl::Output::getWheel(int num) {
	switch (num) {
		case 0:
			return left;

		case 1:
			return right;

		case 2:
			return back;
	}
}


#include "ieee/drivers/avr/WheelsControl.h"
#include <cmath>

using namespace ieee;
using namespace std;

WheelsControl::WheelsControl(const Config &config) : config(config) { }

void WheelsControl::writeOutput(const Output &output, GumstixPacket &gp) const {
	bool reverse;
	gp.enable_bits |= ENABLE_SERVOS;

	if (output.left.enabled) {
		gp.leftwheel_angle = toRawAngle(output.left.angle, gp.leftwheel_angle, config.left, reverse);
		gp.leftwheel_effort = toRawEffort(output.left.effort, reverse);
	} else
		gp.leftwheel_effort = 0;

	if (output.right.enabled) {
		gp.rightwheel_angle = toRawAngle(output.right.angle, gp.rightwheel_angle, config.right, reverse);
		gp.rightwheel_effort = toRawEffort(output.right.effort, reverse);
	} else
		gp.rightwheel_effort = 0;

	if (output.back.enabled) {
		gp.backwheel_angle = toRawAngle(output.back.angle, gp.backwheel_angle, config.back, reverse);
		gp.backwheel_effort = toRawEffort(output.back.effort, reverse);
	} else
		gp.backwheel_effort = 0;
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

int16_t WheelsControl::toRawAngle(float angle, int16_t curpos, const WheelConfig &wconf, bool &reverse) const {
	reverse = false;

	int pos = (int)(angle/M_PI*1800);
	if (pos < 0) {
		pos += 1800;
		reverse = true;
	} else if (pos > 1800) {
		pos -= 1800;
		reverse = true;
	}

	const int th_low = config.turnhysteresis;
	const int th_high = 1800 - config.turnhysteresis;

	if (pos < th_low && curpos > th_high) {
		pos = 1800;
		reverse = !reverse;
	} else if (pos > th_high && curpos < th_low) {
		pos = 0;
		reverse = !reverse;
	}

	pos += wconf.offset;

	if (pos < wconf.minstop)
		return wconf.minstop;
	else if (pos > wconf.maxstop)
		return wconf.maxstop;
	else
		return (int16_t)pos;
}

int16_t WheelsControl::toRawEffort(float eff, bool reverse) {
	int16_t effort = (int16_t)(eff*1000);
	if (reverse)
		return -effort;
	else
		return effort;
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


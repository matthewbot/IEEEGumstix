#include "ieee/controls/DriveEquation.h"
#include <algorithm>

using namespace ieee;
using namespace std;

DriveEquation::DriveEquation(const Config &config) : config(config) { }

WheelsControl::Output DriveEquation::compute(const Motion &motion) const {
	WheelsControl::Output out;
	out.left = computeWheel(config.left, motion);
	out.right = computeWheel(config.right, motion);
	out.back = computeWheel(config.back, motion);

	float maxeffort = max(max(out.left.effort, out.right.effort), out.back.effort);
	if (maxeffort > 1) {
		out.left.effort /= maxeffort;
		out.right.effort /= maxeffort;
		out.back.effort /= maxeffort;
	}

	return out;
}

WheelsControl::WheelOutput DriveEquation::computeWheel(const WheelConfig &wconfig, const Motion &motion) const {
	Vec2D relvel = motion.vel.rotate(motion.curdir);

	Vec2D wheelout = relvel + (wconfig.relpos*motion.angvel).rotate(-M_PI/2);
	float mag = wheelout.magnitude();

	WheelsControl::WheelOutput out;
	if (mag < config.minspeed) {
		out.angle = 0;
		out.effort = 0;
		out.enabled = false;
		return out;
	}

	out.angle = (wheelout.angle()+config.rotationoffset).getRad();
	out.effort = wconfig.outscale*mag;
	if (out.effort > 0)
		out.effort += wconfig.outoffset;
	else
		out.effort -= wconfig.outoffset;
	return out;
}

const DriveEquation::Motion DriveEquation::Motion::stop = { Vec2D(0, 0), Angle(0), 0 };


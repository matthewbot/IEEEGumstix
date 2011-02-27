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
	Vec2D relvel = motion.vel.rotate(-motion.curangle);

	Vec2D wheelout;
	wheelout.x = relvel.x + motion.angvel*(-wconfig.relpos.x*sin(motion.curangle) - wconfig.relpos.y*cos(motion.curangle));
	wheelout.y = -relvel.y + motion.angvel*(wconfig.relpos.x*cos(motion.curangle) - wconfig.relpos.y*sin(motion.curangle));

	WheelsControl::WheelOutput out;
	out.angle = wheelout.angle()+config.rotationoffset;
	if (out.angle < 0)
		out.angle += 2*M_PI;
	else if (out.angle >= 2*M_PI)
		out.angle -= 2*M_PI;

	float mag = wheelout.magnitude();
	if (abs(mag) < config.minspeed) {
		out.effort = 0;
	} else {
		out.effort = wconfig.outscale*mag;
		if (out.effort > 0)
			out.effort += wconfig.outoffset;
		else
			out.effort -= wconfig.outoffset;
	}

	return out;
}


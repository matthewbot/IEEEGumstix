#include "ieee/drivers/avr/StepperControl.h"
#include <cmath>

using namespace ieee;
using namespace boost::property_tree;

StepperControl::StepperControl(const Config &config) : config(config) { }

void StepperControl::setAngle(GumstixPacket &gp, float angle) const {
	float curangle = gp.stepper_pos * config.stepsize;
	while (angle - curangle >= M_PI)
		angle -= 2*M_PI;
	while (angle - curangle <= -M_PI)
		angle += 2*M_PI;
	gp.enable_bits |= ENABLE_STEPPER;
	gp.stepper_pos = angle / config.stepsize;
}

float StepperControl::getCurAngle(const AVRPacket &ap) const {
	float angle = ap.stepper_pos * config.stepsize;
	return angle;
}

void StepperControl::setEnabled(GumstixPacket &gp, bool enabled) const {
	if (enabled)
		gp.enable_bits |= ENABLE_STEPPER;
	else
		gp.enable_bits &= ~ENABLE_STEPPER;
}

void StepperControl::Config::readTree(const ptree &pt) {
	stepsize = pt.get<float>("stepsize_deg") / 180 * M_PI;
}



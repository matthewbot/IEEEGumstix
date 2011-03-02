#include "ieee/drivers/avr/StepperControl.h"
#include <cmath>

using namespace ieee;

StepperControl::StepperControl(const Config &config) : config(config) { }

void StepperControl::setAngle(GumstixPacket &gp, float angle) const {
	if (config.wrapangle > 0 && angle >= config.wrapangle)
		angle -= 2*M_PI;
	gp.stepper_pos = (int16_t)(angle / config.stepsize);
	gp.enable_bits |= ENABLE_STEPPER;
}

float StepperControl::getCurAngle(const AVRPacket &ap) const {
	float angle = ap.stepper_pos * config.stepsize;
	if (angle < 0)
		angle += 2*M_PI;
	return angle;
}

void StepperControl::setEnabled(GumstixPacket &gp, bool enabled) const {
	if (enabled)
		gp.enable_bits |= ENABLE_STEPPER;
	else
		gp.enable_bits &= ~ENABLE_STEPPER;
}



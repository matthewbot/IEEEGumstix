#include "ieee/drivers/avr/AVRRobot.h"
#include <cstring>

using namespace ieee;

AVRRobot::AVRRobot(const Config &config)
: comm(),
  compass(config.compass),
  sonar1(config.sonar1, 1),
  sonar2(config.sonar2, 2),
  wheelscontrol(config.wheels) {
	memset(&gp, 0, sizeof(gp));
	memset(&ap, 0, sizeof(ap));
}

bool AVRRobot::syncIn() {
	return comm.syncIn(ap);
}

void AVRRobot::syncOut() {
	return comm.syncOut(gp);
}

float AVRRobot::getCompassAngle() const {
	return compass.getAngle(ap);
}

AVRRobot::SonarDistStatus AVRRobot::getSonar1() const {
	return sonar1.getReading(ap);
}

AVRRobot::SonarDistStatus AVRRobot::getSonar2() const {
	return sonar2.getReading(ap);
}

void AVRRobot::setWheels(const WheelsOutput &wheels) {
	wheelscontrol.writeOutput(wheels, gp);
}

void AVRRobot::setSonarAngle(float angle) {
	gp.sonar_angle = angle/M_PI*1800; // so simple we can just put this in here directly
}

void AVRRobot::setUpDown(bool updown) {
	gp.retract = updown; // TODO.. stuff in serial packets needs to be renamed for better consistency
}


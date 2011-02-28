#include "ieee/drivers/avr/AVRRobot.h"
#include <boost/date_time.hpp>
#include <boost/thread.hpp>
#include <cstring>

using namespace ieee;
using namespace boost;
using namespace boost::posix_time;
using namespace std;

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

void AVRRobot::syncIOWait(int count) {
	for (int i=0; i<count; i++) {
		ptime start = microsec_clock::local_time();
		while (true) {
			comm.syncOut(gp);
			if (comm.syncIn(ap))
				break;

			if (microsec_clock::local_time() - start > milliseconds(500))
				throw runtime_error("AVRRobot failed to sync");

			this_thread::sleep(milliseconds(50));
		}

		while (comm.syncIn(ap)) { }
	}
}

void AVRRobot::syncOut() {
	return comm.syncOut(gp);
}

void AVRRobot::disableAll() {
	gp.enable_bits = 0;
	gp.leftwheel_effort = gp.rightwheel_effort = gp.backwheel_effort = 0;
}

float AVRRobot::getCompassAngle() const {
	return compass.getAngle(ap, wheelscontrol);
}

void AVRRobot::calibrateCompassOffset(float angle) {
	compass.calibrateOffset(ap, wheelscontrol, angle);
}

AVRRobot::SonarReading AVRRobot::getSonar1() const {
	return sonar1.getReading(ap);
}

AVRRobot::SonarReading AVRRobot::getSonar2() const {
	return sonar2.getReading(ap);
}

void AVRRobot::setWheels(const WheelsOutput &wheels) {
	wheelscontrol.writeOutput(wheels, gp);
}

AVRRobot::WheelsOutput AVRRobot::getCurrentWheels() const {
	return wheelscontrol.readOutput(ap);
}

void AVRRobot::setSonarAngle(float angle) {
	gp.sonar_angle = angle/M_PI*1800; // so simple we can just put this in here directly
	gp.enable_bits |= ENABLE_STEPPER;
}

void AVRRobot::setUpDown(bool updown) {
	gp.updown_lift = updown;
}

void AVRRobot::setStepperEnabled(bool enabled) {
	if (enabled)
		gp.enable_bits |= ENABLE_STEPPER;
	else
		gp.enable_bits &= ~ENABLE_STEPPER;
}


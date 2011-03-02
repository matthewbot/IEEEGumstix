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
  wheelscontrol(config.wheels),
  steppercontrol(config.stepper) {
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
	steppercontrol.setAngle(gp, angle);
}

float AVRRobot::getCurSonarAngle() const {
	return steppercontrol.getCurAngle(ap);
}

void AVRRobot::setStepperEnabled(bool enabled) {
	steppercontrol.setEnabled(gp, enabled);
}

void AVRRobot::setUpDown(bool updown) {
	gp.updown_lift = updown;
}


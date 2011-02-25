#include "ieee/controls/PositionController.h"

using namespace ieee;

PositionController::PositionController(const Config &config)
: config(config),
  command(Vec2D(0, 0), 0),
  driveequ(config.driveequ),
  posfilter(config.posfilter),
  start(false) { }

void PositionController::setCommand(const Command &command) {
	this->command = command;
	start = true;
}

void PositionController::stop() {
	start = false;
}

void PositionController::update(AVRRobot &robot) {
	// update the position filter
	PositionFilter::Input posin;
	posin.cursonardir = PositionFilter::SONARDIR_EAST;
	posin.sonar1 = robot.getSonar1();
	posin.sonar2 = robot.getSonar2();
	posin.compassdir = robot.getCompassAngle();
	const PositionFilter::Output &posout = posfilter.update(posin);

	// calculate the stepper motor position using the robot's orientation
	robot.setSonarAngle(PositionFilter::sonarDirToRad(posin.cursonardir) - posout.dir);

	// if we haven't been told to start, we stop here
	if (!start)
		return;

	// compute the motion
	DriveEquation::Motion motion;
	motion.curangle = posout.dir;

	// start from the motion vector
	Vec2D motionvec = command.destpos - posout.pos;
	if (motionvec.magnitude() < config.stopdist) { // if we're close enough already
		motion.vel = Vec2D(0, 0); // stop
		motion.angvel = 0;
	} else {
		motion.vel = motionvec.unit() * command.speed; // set up motion
		motion.angvel = 0;
	}

	robot.setWheels(driveequ.compute(motion)); // compute the wheel outputs and set them
}


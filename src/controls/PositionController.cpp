#include "ieee/controls/PositionController.h"

using namespace ieee;

static float angdiff(float anglea, float angleb); // TODO put this somewhere else

PositionController::PositionController(const Config &config)
: config(config),
  command(Vec2D(0, 0), 0),
  driveequ(config.driveequ),
  posfilter(config.posfilter),
  start(false) {
	lastmotion = DriveEquation::Motion::stop;
}

void PositionController::setCommand(const Command &command) {
	this->command = command;
	start = true;
}

void PositionController::stop() {
	start = false;
}

void PositionController::update(AVRRobot &robot) {
	const PositionFilter::Output &posout = updatePositionFilter(robot);

	DriveEquation::Motion motion;
	if (posout.ok && start)
		motion = computeMotion();
	else
		motion = DriveEquation::Motion::stop;

	robot.setWheels(driveequ.compute(motion));
};

const PositionFilter::Output &PositionController::updatePositionFilter(AVRRobot &robot) {
	// update position filter
	PositionFilter::Input posin;
	posin.cursonardir = PositionFilter::radToSonarDir(robot.getCurSonarAngle() + posfilter.getOutput().dir);
	posin.sonar1 = robot.getSonar1();
	posin.sonar2 = robot.getSonar2();
	posin.compassdir = robot.getCompassAngle();
	const PositionFilter::Output &out = posfilter.update(posin);

	// keep the stepper following the position filter's desired sonar orientation
	robot.setSonarAngle(PositionFilter::sonarDirToRad(out.sonardir) - out.dir);
	return out;
}

#include <iostream>

using namespace std;

DriveEquation::Motion PositionController::computeMotion() {
	Vec2D distvec = command.destpos - posfilter.getOutput().pos;
	float dist = distvec.magnitude();

	DriveEquation::Motion motion;
	if (dist < config.stopdist) {
		motion.vel = Vec2D(0, 0);
		motion.angvel = 0;
		motion.curdir = 0;
	} else if (dist < config.lockdist && abs(angdiff(distvec.angle(), lastmotion.vel.angle())) < config.lockangdiff) {
		motion = lastmotion;
	} else {
		motion.curdir = posfilter.getOutput().dir;
		motion.vel = distvec.unit()*command.speed;
		motion.angvel = angdiff(command.destdir, motion.curdir)*config.angvelfactor;
		if (motion.angvel > config.maxangvel)
			motion.angvel = config.maxangvel;
		else if (motion.angvel < -config.maxangvel)
			motion.angvel = -config.maxangvel;

		cout << "angvel " << motion.angvel << endl;
	}

	lastmotion = motion;
	return motion;
}

static float angdiff(float anglea, float angleb) {
	float diff = anglea - angleb;
	while (diff <= -M_PI)
		diff += M_PI*2;
	while (diff > M_PI)
		diff -= M_PI*2;
	return diff;
}


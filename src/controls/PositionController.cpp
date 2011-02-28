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
	updatePositionFilter(robot);

	if (!start)
		return;

	Vec2D velvec = computeVelocityVector();

	// set up the motion
	DriveEquation::Motion motion;
	motion.curangle = posfilter.getOutput().dir;
	motion.vel = velvec;
	motion.angvel = 0;
	robot.setWheels(driveequ.compute(motion)); // compute the wheel outputs and set them
}

void PositionController::updatePositionFilter(AVRRobot &robot) {
	// update position filter
	PositionFilter::Input posin;
	posin.cursonardir = PositionFilter::SONARDIR_EAST;
	posin.sonar1 = robot.getSonar1();
	posin.sonar2 = robot.getSonar2();
	posin.compassdir = robot.getCompassAngle();
	const PositionFilter::Output &out = posfilter.update(posin);

	// keep the stepper following the position filter's desired sonar orientation
	robot.setSonarAngle(PositionFilter::sonarDirToRad(out.sonardir) - out.dir);
}

static float angdiff(float anglea, float angleb) {
	float diff = anglea - angleb;
	while (diff < 0)
		diff += 2*M_PI;
	while (diff >= 2*M_PI)
		diff -= 2*M_PI;
	return diff;
}

Vec2D PositionController::computeVelocityVector() {
	// compute the distance
	Vec2D distvec = command.destpos - posfilter.getOutput().pos;
	float dist = distvec.magnitude();

	// compute the desired velocity vector
	Vec2D velvec;
	if (dist < config.stopdist) {
		velvec = Vec2D(0, 0);
	} else if (dist < config.lockdist && abs(angdiff(distvec.angle(), lastvelvec.angle())) < config.lockangdiff) {
		velvec = lastvelvec;
	} else {
		velvec = distvec.unit() * command.speed;
	}

	lastvelvec = velvec;
	return velvec;
}


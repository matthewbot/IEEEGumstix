#include "ieee/controls/PositionController.h"

using namespace ieee;

PositionController::PositionController(const Config &config)
: config(config),
  command(Vec2D(0, 0), 0),
  driveequ(config.driveequ),
  posfilter(config.posfilter),
  state(STOPPED),
  motion(DriveEquation::Motion::stop) { }

void PositionController::setCommand(const Command &command) {
	this->command = command;
	state = DRIVE;
}

void PositionController::update(AVRRobot &robot) {
	updatePositionFilter(robot);

	if (!posfilter.getPositionOk() || state == STOPPED)
		robot.offWheels();
	else {
		updateMotion();
		robot.setWheels(driveequ.compute(motion));
	}
};

void PositionController::updatePositionFilter(AVRRobot &robot) {
	Angle sonarangle = Angle(robot.getCurSonarAngle()) + posfilter.getHeading();

	PositionFilter::Input posin;
	posin.cursonardir = PositionFilter::angleToSonarDir(sonarangle);
	posin.sonar1 = robot.getSonar1();
	posin.sonar2 = robot.getSonar2();
	posin.compassheading = Angle(robot.getCompassAngle());
	posfilter.update(posin);

	// keep the stepper following the position filter's desired sonar orientation
	robot.setSonarAngle((posfilter.getDesiredSonarAngle() - posfilter.getHeading()).getRad());
}

void PositionController::updateMotion() {
	Vec2D distvec = command.destpos - posfilter.getPosition(); // compute a vector giving the distance and direction to travel
	float dist = distvec.magnitude(); // magnitude gives the distance as a scalar
	float turndist = (command.destheading - posfilter.getHeading()).getDistFromZero(); // compute the amount the robot needs to turn

	// first, handle state transitions
	switch (state) {
		case DRIVE:
			if (dist < config.lockdist) { // DRIVE -> LOCK when within lockdist
				lockvec = distvec.unit(); // save our distvec's direction as the lockvec
				state = LOCK;
			}
			break;

		case LOCK:
			if (dist < config.stopdist) { // LOCK ends when we enter the stopdist
				if (abs(turndist) > config.maxturndist) // if we still need to turn
					state = TURN; // enter TURN
				else
					state = DONE; // don't need to turn, skip directly to DONE
			} else if (abs((distvec.angle() - lockvec.angle()).getDistFromZero()) < config.lockangdiff) { // LOCK -> DRIVE if we missed / overshot
				state = DRIVE;
			}
			break;

		case TURN:
			if (abs(turndist) < config.maxturndist) // TURN -> DONE if we no longer need to turn
				state = DONE;
			break;

		case DONE:
			break;
	}

	switch (state) {
		case DRIVE:
			motion.vel = distvec.unit()*command.speed; // DRIVE just follows the direction of the distance vector
			motion.angvel = (command.destheading - motion.curdir).getDistFromZero()*config.angvelfactor; // throw some angvel in so we turn while moving and look fancy
			break;

		case LOCK:
			motion.vel = lockvec*config.lockspeed; // LOCK ignores the distvec and follows the lockvec
			motion.angvel = 0; // no more angvel, makes it easier to hit the destination point
			break;

		case TURN:
			motion.vel = Vec2D(0, 0); // TURN doesn't move
			motion.angvel = turndist > 0 ? config.turnspeed : -config.turnspeed; // turn at a constant speed in the correct direction
			break;

		case DONE:
			motion.vel = Vec2D(0, 0);
			motion.angvel = 0;
			break;
	}

	// lastly, some housekeeping on the motion data

	motion.curdir = posfilter.getHeading(); // always update curdir from the position filter
	if (motion.angvel > config.maxangvel) // limit angvel
		motion.angvel = config.maxangvel;
	else if (motion.angvel < -config.maxangvel)
		motion.angvel = -config.maxangvel;
}


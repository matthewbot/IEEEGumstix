#include "ieee/controls/PositionFilter.h"
#include <cassert>
#include <numeric>

using namespace ieee;
using namespace boost::property_tree;
using namespace std;

PositionFilter::PositionFilter(const Config &config)
: config(config),
  position(0, 0),
  positionok(false),
  heading(0),
  desiredsonardir(SONARDIR_EAST) { }

Angle PositionFilter::sonarDirToAngle(SonarDir dir) {
	assert(dir != SONARDIR_INDETERMINATE);
	return Angle(dir * M_PI/2);
}

PositionFilter::SonarDir PositionFilter::angleToSonarDir(Angle angle) {
	for (int i=0; i<4; i++) {
		Angle sonarangle = Angle(i*M_PI/2);
		Angle delta = angle - sonarangle;

		if (abs(delta.getDistFromZero()) < .1)
			return (SonarDir)i;
	}

	return SONARDIR_INDETERMINATE;
}

void PositionFilter::update(const Input &input) {
	updateHeading(input);
	updatePos(input);
	updateDesiredSonarDir(input);
}

void PositionFilter::updateHeading(const Input &input) {
	heading = input.compassheading;
}

void PositionFilter::updatePos(const Input &input) {
	positionok = false;

	if (input.cursonardir == SONARDIR_INDETERMINATE)
		return;

	posbuf.push_back(computeSonarPos(input));
	if (posbuf.size() > config.posbufsize)
		posbuf.erase(posbuf.begin());

	position = accumulate(posbuf.begin(), posbuf.end(), Vec2D(0, 0)) / posbuf.size(); // put the average in the output

	if (posbuf.size() < config.posbufsize)
		return;
	else if (position.x < 0 || position.x > config.roomwidth || position.y < 0 || position.y > config.roomheight)
		return;

	positionok = true;
}

void PositionFilter::updateDesiredSonarDir(const Input &input) {
	if (input.cursonardir == SONARDIR_INDETERMINATE)
		return;

	bool up = (input.cursonardir == SONARDIR_EAST || input.cursonardir == SONARDIR_NORTH);
	bool right = (input.cursonardir == SONARDIR_EAST || input.cursonardir == SONARDIR_SOUTH);

	if (position.x < config.sonarturnmindist)
		right = true;
	else if (position.x > config.roomwidth - config.sonarturnmindist)
		right = false;

	if (position.y < config.sonarturnmindist)
		up = true;
	else if (position.y > config.roomheight - config.sonarturnmindist)
		up = false;

	if (up) {
		if (right)
			desiredsonardir = SONARDIR_EAST;
		else
			desiredsonardir = SONARDIR_NORTH;
	} else {
		if (right)
			desiredsonardir = SONARDIR_SOUTH;
		else
			desiredsonardir = SONARDIR_WEST;
	}
}

Vec2D PositionFilter::computeSonarPos(const Input &input) const {
	assert(input.cursonardir != SONARDIR_INDETERMINATE);

	Vec2D sonarpos;
	const float s1dist = input.sonar1 + config.sonarmountradius;
	const float s2dist = input.sonar2 + config.sonarmountradius;

	switch (input.cursonardir) {
		case SONARDIR_EAST:
			sonarpos.x = config.roomwidth - s1dist;
			sonarpos.y = config.roomheight - s2dist;
			break;

		case SONARDIR_NORTH:
			sonarpos.y = config.roomheight - s1dist;
			sonarpos.x = s2dist;
			break;

		case SONARDIR_WEST:
			sonarpos.x = s1dist;
			sonarpos.y = s2dist;
			break;

		case SONARDIR_SOUTH:
			sonarpos.y = s1dist;
			sonarpos.x = config.roomwidth - s2dist;
			break;
	}

	sonarpos -= config.sonarposoffset.rotate(heading);
	return sonarpos;
}

void PositionFilter::Config::readTree(const ptree &pt) {
	roomwidth = pt.get<float>("roomwidth");
	roomheight = pt.get<float>("roomheight");

	sonarposoffset.x = pt.get<float>("sonarposoffset_x");
	sonarposoffset.y = pt.get<float>("sonarposoffset_y");
	sonarmountradius = pt.get<float>("sonarmountradius");
	sonarturnmindist = pt.get<float>("sonarturnmindist");

	posbufsize = pt.get<int>("posbufsize");
}


#include "ieee/controls/PositionFilter.h"
#include <cassert>

using namespace ieee;

PositionFilter::PositionFilter(const Config &config) : config(config), lastsonarpos(0, 0) {
	output.pos = Vec2D(0, 0);
	output.dir = 0;
	output.sonardir = SONARDIR_EAST;
}

float PositionFilter::sonarDirToRad(SonarDir dir) {
	assert(dir != SONARDIR_INDETERMINATE);
	return dir * M_PI/2;
}

PositionFilter::SonarDir PositionFilter::radToSonarDir(float rad) {
	for (int i=0; i<4; i++) {
		float actualrad = i*M_PI/2;
		float delta = rad - actualrad;
		while (delta >= M_PI)
			delta -= 2*M_PI;
		while (delta < -M_PI)
			delta += 2*M_PI;

		if (abs(delta) < .05)
			return (SonarDir)i;
	}

	return SONARDIR_INDETERMINATE;
}

const PositionFilter::Output &PositionFilter::update(const Input &input) {
	output.dir = input.compassdir;
	output.pos = updateSonarPos(input);
	output.sonardir = computeSonarDir(output.pos, input.cursonardir);
	output.ok = (input.cursonardir == output.sonardir); // for now, our "ok" status entirely depends on the sonars being alligned

	return output;
}

Vec2D PositionFilter::updateSonarPos(const Input &input) {
	if (input.cursonardir == SONARDIR_INDETERMINATE)
		return lastsonarpos;

	Vec2D sonarpos;
	const float s1dist = input.sonar1.dist + config.sonarstepperrad;
	const float s2dist = input.sonar2.dist + config.sonarstepperrad;

	switch (input.cursonardir) {
		case SONARDIR_EAST:
			sonarpos.x = config.roomwidth - s1dist;
			sonarpos.y = s2dist;
			break;

		case SONARDIR_NORTH:
			sonarpos.y = s1dist;
			sonarpos.x = s2dist;
			break;

		case SONARDIR_WEST:
			sonarpos.x = s1dist;
			sonarpos.y = config.roomheight - s2dist;
			break;

		case SONARDIR_SOUTH:
			sonarpos.y = config.roomheight - s1dist;
			sonarpos.x = config.roomwidth - s2dist;
			break;
	}

	sonarpos -= config.sonaroffset.rotate(output.dir);

	lastsonarpos = sonarpos;
	return sonarpos;
}

PositionFilter::SonarDir PositionFilter::computeSonarDir(const Vec2D &pos, SonarDir curdir) const {
	if (curdir == SONARDIR_INDETERMINATE)
		return output.sonardir;

	bool up = (curdir == SONARDIR_EAST || curdir == SONARDIR_NORTH);
	bool right = (curdir == SONARDIR_EAST || curdir == SONARDIR_SOUTH);

	if (pos.x < config.sonarmindist)
		right = true;
	else if (pos.x > config.roomwidth - config.sonarmindist)
		right = false;

	if (pos.y < config.sonarmindist)
		up = false;
	else if (pos.y > config.roomheight - config.sonarmindist)
		up = true;

	if (up) {
		if (right)
			return SONARDIR_EAST;
		else
			return SONARDIR_NORTH;
	} else {
		if (right)
			return SONARDIR_SOUTH;
		else
			return SONARDIR_WEST;
	}
}


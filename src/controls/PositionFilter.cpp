#include "ieee/controls/PositionFilter.h"
#include <cassert>

using namespace ieee;

PositionFilter::PositionFilter(const Config &config) : config(config) {
	output.pos = Vec2D(0, 0);
	output.dir = 0;
	output.sonardir = SONARDIR_INDETERMINATE;
}

float PositionFilter::sonarDirToRad(SonarDir dir) {
	assert(dir != SONARDIR_INDETERMINATE);
	return dir * M_PI/2;
}

const PositionFilter::Output &PositionFilter::update(const Input &input) {
	output.dir = input.compassdir;

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

		case SONARDIR_INDETERMINATE:
			sonarpos = output.pos;
			break;
	}
	output.pos = sonarpos - config.sonaroffset.rotate(output.dir);

	return output;
}


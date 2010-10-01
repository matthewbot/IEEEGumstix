#include "types.h"
#include <cmath>

using namespace pathsim;
using namespace std;

ostream &pathsim::operator<<(ostream &out, const Pos &pos) {
	out << "(" << pos.x << ", " << pos.y << ")";
}

Dir pathsim::oppositeDir(Dir dir) {
	int newdir = dir + 4;
	if (newdir >= MAX_DIR)
		newdir -= MAX_DIR - 1; // -1 to skip DIR_NONE
	return (Dir)newdir;
}

bool pathsim::isDirDiagonal(Dir dir) {
	switch (dir) {
		case DIR_NE:
		case DIR_NW:
		case DIR_SW:
		case DIR_SE:
			return true;
			
		default:
			return false;
	}
}

Pos pathsim::advancePos(Pos pos, Dir dir) {
	if (dir == DIR_NE || dir == DIR_E || dir == DIR_SE)
		pos.x += 1;
	else if (dir == DIR_NW || dir == DIR_W || dir == DIR_SW)
		pos.x -= 1;
	
	if (dir == DIR_NW || dir == DIR_N || dir == DIR_NE)
		pos.y -= 1;
	else if (dir == DIR_SW || dir == DIR_S || dir == DIR_SE)
		pos.y += 1;
		
	return pos;
}

Dir pathsim::getDirFromPoses(const Pos &start, const Pos &end) {
	int dx = end.x - start.x;
	int dy = end.y - start.y;
	if (dx > 0) {
		if (dy > 0)
			return DIR_SE;
		else if (dy < 0)
			return DIR_NE;
		else
			return DIR_E;
	} else if (dx < 0) {
		if (dy > 0)
			return DIR_SW;
		else if (dy < 0)
			return DIR_NW;
		else
			return DIR_W;
	} else {
		if (dy > 0)
			return DIR_S;
		else if (dy < 0)
			return DIR_N;
		else
			return DIR_NONE;	
	}
}

float pathsim::dirToRad(Dir dir) {
	if (dir == DIR_NONE)
		return 0;
	else {
		return (dir-1)*M_PI/4;
	}
}



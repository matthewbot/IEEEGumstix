#include "ieee/shared/types.h"
#include <cmath>

using namespace ieee;
using namespace std;

bool Pos::operator<(const Pos &pos) const {
	if (x < pos.x)
		return true;
	else if (x > pos.x)
		return false;
	else
		return y < pos.y;
}

ostream &ieee::operator<<(ostream &out, const Pos &pos) {
	out << "(" << pos.x << ", " << pos.y << ")";
}

ostream &ieee::operator<<(ostream &out, Dir dir) {
	static const char *dirstr[] = {"none", "E", "NE", "N", "NW", "W", "SW", "S", "SE"};

	if (dir < MAX_DIR && dir >= 0)
		out << dirstr[dir];
	else
		out << "(BAD DIR)";

	return out;
}

Dir ieee::oppositeDir(Dir dir) {
	int newdir = dir + 4;
	if (newdir >= MAX_DIR)
		newdir -= MAX_DIR - 1; // -1 to skip DIR_NONE
	return (Dir)newdir;
}

bool ieee::isDirDiagonal(Dir dir) {
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

Pos ieee::advancePos(Pos pos, Dir dir) {
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

Dir ieee::getDirFromPoses(const Pos &start, const Pos &end) {
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

int ieee::getDirDelta(Dir cur, Dir dest) {
	int delta = dest - cur;
	if (delta >= 4)
		delta -= 8;
	else if (delta < -4)
		delta += 8;
	return delta;
}

Dir ieee::getIntermediateDir(Dir cur, Dir dest) {
	int newdir;

	int delta = getDirDelta(cur, dest);
	if (delta > 0)
		newdir = cur + 1;
	else if (delta < 0)
		newdir = cur - 1;

	if (newdir >= MAX_DIR)
		newdir -= MAX_DIR - 1;
	else if (newdir <= DIR_NONE)
		newdir += MAX_DIR - 1;

	return (Dir)newdir;
}

float ieee::dirToRad(Dir dir) {
	if (dir == DIR_NONE)
		return 0;
	else {
		return (dir-1)*M_PI/4;
	}
}

Dir ieee::radToNearestDir(float rad) {
	int i = (int)round(rad / (M_PI/4));
	while (i < 0)
		i += 8;
	while (i >= 8)
		i -= 8;

	return (Dir)(i + DIR_E);
}

Pos CoordScale::coordToPos(float x, float y) const {
    return Pos((int)round(x*sx + xoff), (int)round(y*sy + yoff));
}

Pos CoordScale::coordToPos(float x, float y, float &errx, float &erry) const {
    Pos p = coordToPos(x, y);
    Coord c = posToCoord(p);
    errx = x - c.x;
    erry = y - c.y;
    return p;
}

Coord CoordScale::posToCoord(int x, int y) const {
    return Coord(((float)x - xoff) / sx, ((float)y - yoff) / sy);
}


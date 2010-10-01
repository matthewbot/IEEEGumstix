#ifndef UTIL_H
#define UTIL_H

#include <ostream>
#include <vector>

namespace pathsim {
	struct Pos {
		int x, y;
		
		inline bool operator==(const Pos &pos) const { return pos.x == x && pos.y == y; }
		inline bool operator!=(const Pos &pos) const { return !(*this == pos); }
		
		inline Pos() { }
		inline Pos(int x, int y) : x(x), y(y) { }
	};
	
	typedef std::vector<Pos> Path;
	
	std::ostream &operator<<(std::ostream &stream, const Pos &pos);

	enum Dir {
		DIR_NONE,
		DIR_E,
		DIR_NE,
		DIR_N,
		DIR_NW,
		DIR_W,
		DIR_SW,
		DIR_S,
		DIR_SE,
		MAX_DIR
	};

	Dir oppositeDir(Dir dir);
	bool isDirDiagonal(Dir dir);
	Pos advancePos(Pos pos, Dir dir);
	Dir getDirFromPoses(const Pos &start, const Pos &end);
	float dirToRad(Dir dir);
}

#endif


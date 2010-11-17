#ifndef TYPES_H
#define TYPES_H

#include <boost/unordered_set.hpp>
#include <ostream>
#include <vector>
#include <set>

namespace ieee {
	struct Pos {
		int x, y;
		
		inline bool operator==(const Pos &pos) const { return pos.x == x && pos.y == y; }
		inline bool operator!=(const Pos &pos) const { return !(*this == pos); }
		bool operator<(const Pos &pos) const;
		
		inline Pos() { }
		inline Pos(int x, int y) : x(x), y(y) { }
	};
	
	inline size_t hash_value(const Pos &pos) { return (pos.x << 8) | pos.y; }
	
	typedef std::vector<Pos> Path;
	typedef boost::unordered_set<Pos> PosSet;
	
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
	
	std::ostream &operator<<(std::ostream &stream, Dir dir);
	
	typedef std::vector<Dir> DirVec;

	Dir oppositeDir(Dir dir);
	bool isDirDiagonal(Dir dir);
	Pos advancePos(Pos pos, Dir dir);
	Dir getDirFromPoses(const Pos &start, const Pos &end);
	int getDirDelta(Dir cur, Dir dest);
	Dir getIntermediateDir(Dir cur, Dir dest);
	float dirToRad(Dir dir);
	Dir radToNearestDir(float rad);
	
	struct Coord {
		float x, y;
		
		inline Coord() { }
		inline Coord(float x, float y) : x(x), y(y) { }
		explicit inline Coord(const Pos &pos) : x(pos.x), y(pos.y) { }
		
		inline bool operator<(const Pos &pos) const;
	};
}

#endif


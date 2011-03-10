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

	typedef std::vector<Pos> PosList;
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

	typedef std::vector<Coord> CoordList;

	struct CoordRoute {
		CoordList coords;
		DirVec facedirs;
	};

	struct CoordScale {
	    float sx, sy;
	    float xoff, yoff;

	    CoordScale() { }
	    CoordScale(float sx, float sy, float xoff=0, float yoff=0) : sx(sx), sy(sy), xoff(xoff), yoff(yoff) { }

        inline static CoordScale identity() { return CoordScale(1, 1); }

		inline Pos coordToPos(const Coord &coord) const { return coordToPos(coord.x, coord.y); }
		inline Coord posToCoord(const Pos &pos) const { return posToCoord(pos.x, pos.y); }

		Pos coordToPos(float x, float y) const;
		Pos coordToPos(float x, float y, float &errx, float &erry) const;
		Coord posToCoord(int x, int y) const;
    };
}

#endif


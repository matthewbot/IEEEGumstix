#ifndef ASTARSEARCH_H
#define ASTARSEARCH_H

#include "WorldGrid.h"
#include <vector>
#include <set>
#include <boost/scoped_array.hpp>

namespace pathsim {
	class AStarSearch {
		public:
			struct Pos {
				int x, y;
				
				bool operator==(const Pos &pos) const;
				inline bool operator!=(const Pos &pos) const { return !(*this == pos); }
			};
		
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
			
			static Dir oppositeDir(Dir dir);
			static bool isDirDiagonal(Dir dir);
			static Pos advancePos(Pos pos, Dir dir);
		
			struct Square {
				Dir parentdir;
				int gscore; // best movement cost
				int hscore; // heuristic score
				bool closed;
			};

			typedef std::vector<Pos> Route;
			
			AStarSearch(const WorldGrid &grid, const Pos &start, const Pos &end);
			
			inline const Route &getRoute() const { return route; }
			inline int getRouteLength() const { return route.size(); }
			inline bool foundRoute() const { return route.size() > 0; }
			
			const Square &getSquare(int x, int y) const { return squares[x + y*width]; }
			const Square &getSquare(const Pos &pos) const { return getSquare(pos.x, pos.y); }
			
		private:
			inline Square &getSquare(const Pos &pos) { return squares[pos.x + pos.y*width]; }
		
			int width, height;
			boost::scoped_array<Square> squares;
			Route route;
	};
}

#endif


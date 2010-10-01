#ifndef ASTARSEARCH_H
#define ASTARSEARCH_H

#include "WorldGrid.h"
#include "util.h"
#include <vector>
#include <list>
#include <boost/scoped_array.hpp>

namespace pathsim {
	class AStarSearch {
		public:
			struct Square {
				Dir parentdir;
				int cost;
				int heuristic; 
				bool closed;
				
				Square();
			};
			
			AStarSearch(const WorldGrid &grid, const Pos &start, const Pos &end);
			
			inline const Path &getPath() const { return path; }
			inline int getPathLength() const { return path.size(); }
			inline bool foundPath() const { return path.size() > 0; }
			inline int getPathCost() const { return getSquare(path.back()).cost; }
			
			const Square &getSquare(int x, int y) const { return squares[x + y*width]; }
			const Square &getSquare(const Pos &pos) const { return getSquare(pos.x, pos.y); }
			
		private:
			inline Square &getSquare(const Pos &pos) { return squares[pos.x + pos.y*width]; }
		
			typedef std::list<Pos> OpenList;
			void insertPosToOpenList(OpenList &openlist, const Pos &pos, int fscore);
			int pathCost(Dir dir, const Pos &pos, const WorldGrid &grid);
			int positionHeuristic(const Pos &pos, const Pos &end);
			bool doSearch(const WorldGrid &grid, const Pos &start, const Pos &end);
		
			int width, height;
			boost::scoped_array<Square> squares;
			Path path;
	};
}

#endif


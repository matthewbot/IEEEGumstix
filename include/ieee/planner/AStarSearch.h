#ifndef ASTARSEARCH_H
#define ASTARSEARCH_H

#include "ieee/planner/NodeGrid.h"
#include "ieee/shared/types.h"
#include <vector>
#include <list>
#include <boost/scoped_array.hpp>

namespace ieee {
	class AStarSearch {
		public:
			struct Square {
				Dir parentdir;
				int cost;
				int heuristic; 
				bool closed;
				
				Square();
			};
			
			AStarSearch(const NodeGrid &grid, const Pos &start, const Pos &end);
			
			inline const PosList &getPath() const { return path; }
			inline int getPathLength() const { return path.size(); }
			inline bool foundPath() const { return path.size() > 0; }
			inline int getPathCost() const { return getSquare(path.back()).cost; }
			
			const Square &getSquare(int x, int y) const { return squares[x + y*width]; }
			const Square &getSquare(const Pos &pos) const { return getSquare(pos.x, pos.y); }
			
		private:
			inline Square &getSquare(const Pos &pos) { return squares[pos.x + pos.y*width]; }
		
			typedef std::list<Pos> OpenList;
			void insertPosToOpenList(OpenList &openlist, const Pos &pos, int fscore);
			int pathCost(Dir dir, const Pos &pos, const NodeGrid &grid);
			int positionHeuristic(const Pos &pos, const Pos &end);
			bool doSearch(const NodeGrid &grid, const Pos &start, const Pos &end);
		
			int width, height;
			boost::scoped_array<Square> squares;
			PosList path;
	};
}

#endif


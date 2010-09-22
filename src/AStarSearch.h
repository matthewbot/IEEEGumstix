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

			typedef std::vector<Pos> Route;
			
			AStarSearch(const WorldGrid &grid, const Pos &start, const Pos &end);
			
			inline const Route &getRoute() const { return route; }
			inline int getRouteLength() const { return route.size(); }
			inline bool foundRoute() const { return route.size() > 0; }
			inline int getRouteCost() const { return getSquare(route.back()).cost; }
			
			const Square &getSquare(int x, int y) const { return squares[x + y*width]; }
			const Square &getSquare(const Pos &pos) const { return getSquare(pos.x, pos.y); }
			
		private:
			inline Square &getSquare(const Pos &pos) { return squares[pos.x + pos.y*width]; }
		
			typedef std::list<Pos> OpenList;
			bool isPosValid(const Pos &pos);
			void insertPosToOpenList(OpenList &openlist, const Pos &pos, int fscore);
			int pathCost(Dir dir, const Pos &pos, const WorldGrid &grid);
			int positionHeuristic(const Pos &pos, const Pos &end);
			bool doSearch(const WorldGrid &grid, const Pos &start, const Pos &end);
		
			int width, height;
			boost::scoped_array<Square> squares;
			Route route;
	};
}

#endif


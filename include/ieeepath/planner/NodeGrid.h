#ifndef NODEGRID_H
#define NODEGRID_H

#include "ieeepath/planner/Node.h"
#include "ieeepath/shared/WorldGrid.h"
#include <boost/multi_array.hpp>
#include <ostream>

namespace ieee {
	class NodeGrid {
		public:
			NodeGrid(int width, int height);
			
			inline int getWidth() const { return array.shape()[0]; }
			inline int getHeight() const { return array.shape()[1]; }
			
			inline bool inBounds(Pos pos) const { return pos.x >= 0 && pos.y >= 0 && pos.x < getWidth() && pos.y < getHeight(); }
			
			inline const Node &operator[](const Pos &pos) const { assert(inBounds(pos)); return array[pos.x][pos.y]; }
			inline Node &operator[](const Pos &pos) { assert(inBounds(pos)); return array[pos.x][pos.y]; }	
			
			static NodeGrid fromWorldGrid(const WorldGrid &grid, const CoordScale &gridscale, const CoordScale &nodescale);
			
		private:
			boost::multi_array<Node, 2> array;
			
			static bool passableRect(const WorldGrid &grid, const Pos &start, const Pos &end);
			static bool unknownRect(const WorldGrid &grid, const Pos &pos, const Pos &end);
			static bool passable(WorldGrid::GridSquare square);
	
	};
	
	std::ostream &operator<<(std::ostream &out, const NodeGrid &grid);
}

#endif

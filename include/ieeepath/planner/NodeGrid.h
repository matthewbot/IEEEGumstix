#ifndef NODEGRID_H
#define NODEGRID_H

#include "ieeepath/planner/Node.h"
#include "ieeepath/shared/WorldGrid.h"
#include <boost/multi_array.hpp>

namespace ieeepath {
	class NodeGrid {
		public:
			NodeGrid(int width, int height);
			
			inline int getWidth() const { return array.shape()[0]; }
			inline int getHeight() const { return array.shape()[1]; }
			
			inline const Node &operator[](const Pos &pos) const { return (*this)(pos.x, pos.y); }
			inline Node &operator[](const Pos &pos) { return (*this)(pos.x, pos.y); }
			inline const Node &operator()(int x, int y) const { return array[x][y]; }
			inline Node &operator()(int x, int y) { return array[x][y]; }
			
			static NodeGrid fromWorldGrid(const WorldGrid &grid);
			
		private:
			boost::multi_array<Node, 2> array;
	
	};
}

#endif

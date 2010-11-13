#ifndef NODE_H
#define NODE_H

#include "ieeepath/shared/types.h"
#include "ieeepath/shared/WorldGrid.h"
#include <ostream>

namespace ieeepath {
	class Node {
		public:
			enum Type {
				OPEN,
				VICTIM_ID,
				CLIMB,
				IMPASSABLE,
				UNKNOWN
			};
			
			Node(Type type=OPEN, Dir dir=DIR_NONE);
			
			inline Type getType() const { return type; }
			inline Dir getDir() const { return dir; }
			
			bool traverseableFrom(Dir travdir) const;
			
			inline bool operator==(const Node &node) const { return type == node.type && dir == node.dir; }
			inline bool operator!=(const Node &node) const { return !(*this == node); }
			
		private:
			Type type;
			Dir dir;
	};
	
	Dir getVictimIDDir(const Pos &nodepos, const WorldGrid &grid);
	bool getVictimPos(const Pos &nodepos, const WorldGrid &grid, Dir dir, Pos &out);
	
	std::ostream &operator<<(std::ostream &out, const Node &node);
}

#endif


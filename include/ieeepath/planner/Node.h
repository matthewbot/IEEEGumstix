#ifndef NODE_H
#define NODE_H

#include "ieeepath/shared/types.h"

namespace ieeepath {
	class Node {
		public:
			enum Type {
				OPEN,
				VICTIM_ID,
				CLIMB,
				CLOSED
			};
			
			Node(Type type=OPEN, Dir dir=DIR_NONE);
			
			inline Type getType() const { return type; }
			inline Dir getDir() const { return dir; }
			
			bool traverseableFrom(Dir travdir) const;
			
		private:
			Type type;
			Dir dir;
	};
}

#endif


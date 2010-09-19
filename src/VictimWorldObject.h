#ifndef VICTIMWORLDOBJECT_H
#define VICTIMWORLDOBJECT_H

#include "WorldObject.h"

namespace pathsim {
	class VictimWorldObject : public WorldObject {
		public:
			VictimWorldObject(int x, int y);
			virtual ~VictimWorldObject();
			
			virtual void fillWorldGrid(WorldGrid &grid) const;
			
			inline int getX() const { return x; }
			inline int getY() const { return y; }
			
			inline void setPos(int x, int y) { this->x = x; this->y = y; }
			
		private:
			int x, y;
	};
}

#endif

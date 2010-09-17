#ifndef VICTIMWORLDOBJECT_H
#define VICTIMWORLDOBJECT_H

#include "WorldObject.h"

namespace roomsim {
	class VictimWorldObject : public WorldObject {
		public:
			VictimWorldObject(int x, int y);
			virtual ~VictimWorldObject();
			
			virtual void fillWorldGrid(WorldGrid &grid) const = 0;
			
		private:
			int x, y;
	};
}

#endif

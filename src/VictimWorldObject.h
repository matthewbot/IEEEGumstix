#ifndef VICTIMWORLDOBJECT_H
#define VICTIMWORLDOBJECT_H

#include "WorldObject.h"

namespace pathsim {
	class VictimWorldObject : public WorldObject {
		public:
			VictimWorldObject(int x, int y);
			virtual ~VictimWorldObject();
			
			virtual void fillWorldGrid(WorldGrid &grid) const;
			
		private:
			int x, y;
	};
}

#endif

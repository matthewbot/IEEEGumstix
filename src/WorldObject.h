#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include "WorldGrid.h"

namespace roomsim {
	class WorldObject {
		public:
			inline virtual ~WorldObject() { }
			virtual void fillWorldGrid(WorldGrid &grid) const = 0;
	};
}

#endif

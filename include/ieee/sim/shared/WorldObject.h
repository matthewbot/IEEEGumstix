#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include "ieee/shared/WorldGrid.h"

namespace ieee {
	class WorldObject {
		public:
			inline virtual ~WorldObject() { }
			virtual void fillWorldGrid(WorldGrid &grid, const CoordScale &gridscale) const =0;

			virtual int selectionTest(const Coord &selectcoord) const =0; // returns an ID if this object has a selectable point at pos or -1
			virtual void selectionMoved(int id, const Coord &newcoord) =0; // moves an object's selectable point to the pos
	};
}

#endif

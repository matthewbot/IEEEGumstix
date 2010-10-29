#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include "WorldGrid.h"

namespace pathsim {
	class WorldObject {
		public:
			inline virtual ~WorldObject() { }
			virtual void fillWorldGrid(WorldGrid &grid) const =0;
			
			virtual int selectionTest(const Pos &selectpos) const =0; // returns an ID if this object has a selectable point at pos or -1
			virtual void selectionMoved(int id, const Pos &newpos) =0; // moves an object's selectable point to the pos
	};
}

#endif

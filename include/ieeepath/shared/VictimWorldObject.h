#ifndef VICTIMWORLDOBJECT_H
#define VICTIMWORLDOBJECT_H

#include "WorldObject.h"
#include "types.h"

namespace ieeepath {
	class VictimWorldObject : public WorldObject {
		public:
			VictimWorldObject(const Pos &pos);
			virtual ~VictimWorldObject();
			
			virtual void fillWorldGrid(WorldGrid &grid) const;
			virtual int selectionTest(const Pos &selectpos) const;
			virtual void selectionMoved(int id, const Pos &newpos);
			
			inline const Pos &getPos() const { return pos; }
			inline void setPos(const Pos &pos) { this->pos = pos; }
			
		private:
			Pos pos;
	};
}

#endif
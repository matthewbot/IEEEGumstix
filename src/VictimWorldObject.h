#ifndef VICTIMWORLDOBJECT_H
#define VICTIMWORLDOBJECT_H

#include "WorldObject.h"
#include "util.h"

namespace pathsim {
	class VictimWorldObject : public WorldObject {
		public:
			VictimWorldObject(const Pos &pos);
			virtual ~VictimWorldObject();
			
			virtual void fillWorldGrid(WorldGrid &grid) const;
			
			inline const Pos &getPos() const { return pos; }
			inline void setPos(const Pos &pos) { this->pos = pos; }
			
		private:
			Pos pos;
	};
}

#endif

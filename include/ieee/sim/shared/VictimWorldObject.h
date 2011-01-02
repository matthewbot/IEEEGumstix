#ifndef VICTIMWORLDOBJECT_H
#define VICTIMWORLDOBJECT_H

#include "ieee/sim/shared/WorldObject.h"
#include "ieee/shared/types.h"

namespace ieee {
	class VictimWorldObject : public WorldObject {
		public:
			VictimWorldObject(const Coord &coord);
			virtual ~VictimWorldObject();

			virtual void fillWorldGrid(WorldGrid &grid, const CoordScale &gridscale) const;
			virtual int selectionTest(const Coord &selectcoord) const;
			virtual void selectionMoved(int id, const Coord &newcoord);

			inline const Coord &getCoord() const { return coord; }
			inline void setCoord(const Coord &coord) { this->coord = coord; }

			static const float RADIUS = 4;
		private:
			Coord coord;
	};
}

#endif

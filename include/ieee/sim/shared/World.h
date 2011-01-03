#ifndef WORLD_H
#define WORLD_H

#include <boost/ptr_container/ptr_vector.hpp>
#include "ieee/sim/shared/WorldObject.h"

namespace ieee {
	class World {
		typedef boost::ptr_vector<WorldObject> WorldObjectList;

		public:
			World(int w, int h, const CoordScale &gridscale);

			typedef WorldObjectList::iterator iterator;
			typedef WorldObjectList::const_iterator const_iterator;
			iterator add(WorldObject *object); // World takes ownership of pointer
			void remove(iterator object);

			inline iterator begin() { return objects.begin(); }
			inline iterator end() { return objects.end(); }
			inline const_iterator begin() const { return objects.begin(); }
			inline const_iterator end() const { return objects.end(); }

			inline void clear() { objects.clear(); updateGrid(); }
			inline const WorldGrid &getGrid() const { return grid; }
			inline const CoordScale &getGridScale() const { return gridscale; }

			void updateGrid();
			void resizeGrid(int w, int h, const CoordScale &gridscale);

		private:
			WorldGrid grid;
			CoordScale gridscale;

			WorldObjectList objects;
	};

}

#endif

#ifndef WORLD_H
#define WORLD_H

#include <boost/ptr_container/ptr_vector.hpp>
#include "WorldObject.h"

namespace pathsim {
	class World {
		typedef boost::ptr_vector<WorldObject> WorldObjectList;
	
		public:
			World(int w, int h);
			
			typedef WorldObjectList::iterator WorldObjectIterator;
			WorldObjectIterator add(WorldObject *object); // World takes ownership of pointer
			void remove(WorldObjectIterator object);
			inline WorldObjectIterator begin() { return objects.begin(); }
			inline WorldObjectIterator end() { return objects.end(); }
			
			inline const WorldGrid &getGrid() const { return grid; }
			void updateGrid();
			
		private:
			WorldGrid grid;
		
			WorldObjectList objects;
	};

}

#endif

#ifndef SMALLOBSTACLEWORLDOBJECT_H
#define SMALLOBSTACLEWORLDOBJECT_H

#include "WorldObject.h"

namespace pathsim {
	class ObstacleWorldObject : public WorldObject {
		public:
			ObstacleWorldObject(int startx, int starty, int endx, int endy, bool large);
			virtual ~ObstacleWorldObject();
			
			virtual void fillWorldGrid(WorldGrid &grid) const = 0;
			
		private:
			int startx, starty;
			int endx, endy;
			bool large;
	};
}

#endif

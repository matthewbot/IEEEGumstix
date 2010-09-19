#ifndef SMALLOBSTACLEWORLDOBJECT_H
#define SMALLOBSTACLEWORLDOBJECT_H

#include "WorldObject.h"

namespace pathsim {
	class ObstacleWorldObject : public WorldObject {
		public:
			ObstacleWorldObject(int startx, int starty, int endx, int endy, bool large);
			virtual ~ObstacleWorldObject();
			
			virtual void fillWorldGrid(WorldGrid &grid) const;
			
			inline int getStartX() const { return startx; }
			inline int getStartY() const { return starty; }
			inline int getEndX() const { return endx; }
			inline int getEndY() const { return endy; }
			inline bool isLarge() const { return large; }
			
			inline void setStart(int x, int y) { startx = x; starty = y; }
			inline void setEnd(int x, int y) { endx = x; endy = y; }
			
		private:
			int startx, starty;
			int endx, endy;
			bool large;
	};
}

#endif

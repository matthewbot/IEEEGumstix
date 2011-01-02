#ifndef SMALLOBSTACLEWORLDOBJECT_H
#define SMALLOBSTACLEWORLDOBJECT_H

#include "WorldObject.h"

namespace ieee {
	class ObstacleWorldObject : public WorldObject {
		public:
			ObstacleWorldObject(const Coord &startcoord, const Coord &endcoord, bool large);
			virtual ~ObstacleWorldObject();

			virtual void fillWorldGrid(WorldGrid &grid, const CoordScale &gridscale) const;
			virtual int selectionTest(const Coord &selectpos) const;
			virtual void selectionMoved(int id, const Coord &newpos);

			inline const Coord &getStartCoord() const { return startcoord; }
			inline const Coord &getEndCoord() const { return endcoord; }
			inline bool isLarge() const { return large; }

			inline void setStartCoord(const Coord &startcoord) { this->startcoord = startcoord; }
			inline void setEndCoord(const Coord &endcoord) { this->endcoord = endcoord; };

			static const float SMALL_WIDTH=2, LARGE_WIDTH=4;

			float getWidth() const;

		private:
			Coord startcoord, endcoord;
			bool large;

			static void shadeQuad(const Pos (&poses)[4], WorldGrid &grid, WorldGrid::GridSquare square);
	};
}

#endif

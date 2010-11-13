#ifndef SMALLOBSTACLEWORLDOBJECT_H
#define SMALLOBSTACLEWORLDOBJECT_H

#include "WorldObject.h"

namespace ieee {
	class ObstacleWorldObject : public WorldObject {
		public:
			ObstacleWorldObject(const Pos &start, const Pos &end, bool large);
			virtual ~ObstacleWorldObject();
			
			virtual void fillWorldGrid(WorldGrid &grid) const;			
			virtual int selectionTest(const Pos &selectpos) const;
			virtual void selectionMoved(int id, const Pos &newpos);

			inline const Pos &getStartPos() const { return startpos; }
			inline const Pos &getEndPos() const { return endpos; }
			inline bool isLarge() const { return large; }
			
			inline void setStartPos(const Pos &startpos) { this->startpos = startpos; }
			inline void setEndPos(const Pos &endpos) { this->endpos = endpos; };
		private:
			Pos startpos, endpos;
			bool large;
	};
}

#endif

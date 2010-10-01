#ifndef ROBOT_H
#define ROBOT_H

#include "WorldGrid.h"
#include "AStarSearch.h"
#include "util.h"
#include <vector>

namespace pathsim {
	class Robot {
		public:
			Robot(int sensorrange, const Pos &startpos, const WorldGrid &grid);
			
			void reset(const Pos &pos, Dir dir=DIR_E);
			void step();
			
			bool identifiedVictim(const Pos &pos) const;
			inline const Pos &getPosition() const { return curpos; }
			inline Dir getDirection() const { return curdir; }
			inline const Pos &getDestination() const { return path.back(); }
			inline const Path &getPath() const { return path; }
			inline const WorldGrid &getMap() const { return map; }
			
		private:
			void moveStep();
			void updateSensorsStep();
			void computeDestStep();
			void updatePathStep();
			int scorePath(const AStarSearch &search) const;
		
			const int sensorrange;
		
			Pos curpos;
			Dir curdir;
			const WorldGrid &grid;
			
			WorldGrid map;
			Path path;
			std::vector<Pos> identifiedvictims;
	};
}

#endif

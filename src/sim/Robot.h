#ifndef ROBOT_H
#define ROBOT_H

#include "WorldGrid.h"
#include "RoutePlanner.h"
#include "CameraSensorPredictor.h"
#include "types.h"
#include <vector>

namespace ieeepath {
	class Robot {
		public:
			Robot(const Pos &startpos, const WorldGrid &grid);
			
			void reset(const Pos &pos, Dir dir=DIR_E);
			void step();
			
			inline bool isVictimIdentified(const Pos &pos) const { return routeplanner.isVictimIdentified(pos); }
			inline const Pos &getPosition() const { return curpos; }
			inline Dir getDirection() const { return curdir; }
			inline const Pos &getDestination() const { return route.path.back(); }
			inline const RoutePlanner::Route &getRoute() const { return route; }
			inline const WorldGrid &getMap() const { return map; }
			
		private:
			void moveStep();
			void updateSensorsStep();
			void computeDestStep();
			void updatePathStep();
		
			CameraSensorPredictor sensorpred;
		
			Pos curpos;
			Dir curdir;
			const WorldGrid &grid;
			
			WorldGrid map;
			RoutePlanner::Route route;
			RoutePlanner routeplanner;
	};
}

#endif

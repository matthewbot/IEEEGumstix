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
			
			void reset(const Pos &pos);
			void step();
			
			bool identifiedVictim(const Pos &pos) const;
			inline const Pos &getPosition() const { return curpos; }
			inline const Pos &getDestination() const { return route.back(); }
			inline const AStarSearch::Route &getRoute() const { return route; }
			inline const WorldGrid &getMap() const { return map; }
			
		private:
			void moveStep();
			void updateSensorsStep();
			void computeDestStep();
			void updateRouteStep();
			int scoreRoute(const AStarSearch &search) const;
		
			const int sensorrange;
		
			Pos curpos;
			const WorldGrid &grid;
			
			WorldGrid map;
			AStarSearch::Route route;
			std::vector<Pos> identifiedvictims;
	};
}

#endif

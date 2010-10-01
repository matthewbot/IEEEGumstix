#ifndef ROUTEPLANNER_H
#define ROUTEPLANNER_H

#include "WorldGrid.h"
#include "AStarSearch.h"
#include "types.h"

namespace pathsim {
	class RoutePlanner {
		public:
			struct Route {
				Path path;
			};
			
			RoutePlanner(const WorldGrid &map, int sensorrange);
			
			Route planRoute(const Pos &curpos, Dir curdir) const;
			
			bool isVictimIdentified(const Pos &pos) const;
			void setVictimIdentified(const Pos &pos);
			void resetVictims();
			
		private:
			int scorePath(const AStarSearch &search) const;
		
			int sensorrange;
			const WorldGrid &map;
			PosSet identifiedvictims;
	};
}

#endif


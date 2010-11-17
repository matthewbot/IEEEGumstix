#ifndef ROOMPLANNER_H
#define ROOMPLANNER_H

#include "ieeepath/planner/SensorPredictor.h"
#include "ieeepath/planner/NodeGrid.h"
#include "ieeepath/planner/RouteEvaluator.h"
#include "ieeepath/shared/types.h"

namespace ieee {
	class RoomPlanner {
		public:
			struct Plan {
				CoordList coords;
				DirVec facedirs;
				bool identifyvictim;
				Pos victimpos;
			};
		
			RoomPlanner(const SensorPredictor &sensorpred, const WorldGrid &worldmap, const RouteEvaluator::Config &config);
		
			Plan planRoute(const Pos &curpos, Dir curdir);
		
			inline bool isVictimIdentified(const Pos &pos) const { return false; }
			inline void setVictimIdentified(const Pos &pos) { }
			inline void resetVictims() { }
		
		private:
			const RouteEvaluator::Config &config;
			const SensorPredictor &sensorpred;
			const WorldGrid &worldmap;
	
			PosSet identifiedvictims;
			
			static bool unknownAdjacent(const NodeGrid &map, const Pos &pos);
	
	};
}

#endif

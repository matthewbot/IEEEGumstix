#ifndef ROUTEPLANNER_H
#define ROUTEPLANNER_H

#include "ieeepath/planner/AStarSearch.h"
#include "ieeepath/planner/SensorPredictorCache.h"
#include "ieeepath/planner/NodeGrid.h"
#include "ieeepath/shared/types.h"
#include <vector>

namespace ieee {
	class RouteEvaluator {
		public:
			struct Config {
				int unknownPruneDist; // prune squares that are this many squares or more from an unknown square
				int pathCostFactor; // scale factor for a route's path cost
				int revealedScoreFactor; // scale factor for a route's revealed squares score (or negative cost)
			};
			
			struct NodeRoute {
				PosList poses;
				DirVec facedirs;
			};
			
			RouteEvaluator(const SensorPredictorCache &pred, const NodeGrid &map, const Config &config);
			
			void addDestination(const Pos &pos);
			
			NodeRoute planRoute(const Pos &curpos, Dir curdir) const;
			
		private:
			int scorePath(const AStarSearch &search, Dir curdir, DirVec &bestdirs) const;
			PosSet getBestUnknownRevealedFrom(const Pos &pos, Dir prevdir, Dir &bestdir, const PosSet &revealed, bool mustsee) const;	
		
			const SensorPredictorCache &pred;
			const NodeGrid &map;
			const Config &config;
			
			PosList destinations;
	};
}

#endif


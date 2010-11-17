#ifndef ROUTEPLANNER_H
#define ROUTEPLANNER_H

#include "ieeepath/planner/AStarSearch.h"
#include "ieeepath/planner/SensorPredictor.h"
#include "ieeepath/planner/NodeGrid.h"
#include "ieeepath/shared/types.h"
#include <vector>
#include <boost/unordered_map.hpp>

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
			
			RouteEvaluator(const SensorPredictor &sensorpred, const NodeGrid &map, const WorldGrid &worldgrid, const Config &config);
			
			void addDestination(const Pos &pos);
			
			NodeRoute planRoute(const Pos &curpos, Dir curdir) const;
			
		private:
			int scorePath(const AStarSearch &search, Dir curdir, DirVec &bestdirs) const;
			const PosSet &getUnknownRevealedFrom(const Pos &pos, Dir dir) const;
			bool canSeeUnknownInAnyDirFrom(const Pos &pos) const;
			PosSet getBestUnknownRevealedFrom(const Pos &pos, Dir prevdir, Dir &bestdir, const PosSet &revealed, bool mustsee) const;			
		
			void clearSensorCache() const;
		
			const SensorPredictor &sensorpred;
			const NodeGrid &map;
			const WorldGrid &worldgrid;
			const Config &config;
			
			PosList destinations;
			
			typedef boost::unordered_map<std::pair<Pos, Dir>, PosSet> UnknownPosCacheMap;
			mutable UnknownPosCacheMap unknownposes_cache;
	};
}

#endif


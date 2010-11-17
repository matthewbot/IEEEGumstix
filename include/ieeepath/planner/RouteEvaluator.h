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
			struct Route {
				CoordList coords;
				DirVec facedirs;
				bool identifyvictim;
				Pos victimpos;
			};
			
			struct Config {
				int unknownPruneDist; // prune squares that are this many squares or more from an unknown square
				int pathCostFactor; // scale factor for a route's path cost
				int pathCostFactorVictim; // scale factor for a route's path cost, when destination is a victim
				int revealedScoreFactor; // scale factor for a route's revealed squares score (or negative cost)
			};
			
			RouteEvaluator(const SensorPredictor &sensorpred, const WorldGrid &worldmap, const Config &config);
			
			Route planRoute(const Pos &curpos, Dir curdir) const;
			
			bool isVictimIdentified(const Pos &pos) const;
			void setVictimIdentified(const Pos &pos);
			void resetVictims();
			
		private:
			int scorePath(const AStarSearch &search, Dir curdir, DirVec &bestdirs, bool destvictim) const;
			const PosSet &getUnknownRevealedFrom(const Pos &pos, Dir dir) const;
			bool canSeeUnknownInAnyDirFrom(const Pos &pos) const;
			PosSet getBestUnknownRevealedFrom(const Pos &pos, Dir prevdir, Dir &bestdir, const PosSet &revealed, bool mustsee) const;			
		
			void clearSensorCache() const;
		
			const Config &config;
			const SensorPredictor &sensorpred;
			const WorldGrid &worldmap;
			mutable NodeGrid map;
			PosSet identifiedvictims;
			
			typedef boost::unordered_map<std::pair<Pos, Dir>, PosSet> UnknownPosCacheMap;
			mutable UnknownPosCacheMap unknownposes_cache;
	};
}

#endif


#ifndef ROUTEPLANNER_H
#define ROUTEPLANNER_H

#include "ieee/planner/AStarSearch.h"
#include "ieee/planner/SensorPredictorCache.h"
#include "ieee/planner/NodeGrid.h"
#include "ieee/shared/types.h"
#include <vector>

namespace ieee {
	class RouteEvaluator {
		public:
			struct Config {
				int pathcostfactor; // scale factor for a route's path cost
				int revealedscorefactor; // scale factor for a route's revealed squares score (or negative cost)

				// turn cost = constant + factor * number of turns / divider
				int turncostconstant;
				int turncostfactor;
				int turncostdivider;
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


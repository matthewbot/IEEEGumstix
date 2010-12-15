#ifndef SENSORPREDICTORCACHE_H
#define SENSORPREDICTORCACHE_H

#include "ieee/planner/SensorPredictor.h"
#include "ieee/shared/WorldGrid.h"
#include "ieee/shared/types.h"
#include <boost/unordered_map.hpp>

namespace ieee {
	class SensorPredictorCache {
		public:
			SensorPredictorCache(const WorldGrid &grid, const CoordScale &gridscale, const CoordScale &nodescale, const SensorPredictor &sensorpred);

			const PosSet &getUnknownRevealedFrom(const Pos &pos, Dir dir) const;
			bool canSeeUnknownInAnyDirFrom(const Pos &pos) const;

		private:
			const WorldGrid &grid;
			const CoordScale &gridscale;
			const CoordScale &nodescale;
			const SensorPredictor &sensorpred;

			typedef boost::unordered_map<std::pair<Pos, Dir>, PosSet> UnknownPosCacheMap;
			mutable UnknownPosCacheMap unknownposes_cache;
	};
}

#endif

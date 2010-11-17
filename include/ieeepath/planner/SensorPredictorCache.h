#ifndef SENSORPREDICTORCACHE_H
#define SENSORPREDICTORCACHE_H

#include "ieeepath/planner/SensorPredictor.h"
#include "ieeepath/shared/WorldGrid.h"
#include "ieeepath/shared/types.h"
#include <boost/unordered_map.hpp>

namespace ieee {
	class SensorPredictorCache {
		public:
			SensorPredictorCache(const WorldGrid &grid, const SensorPredictor &sensorpred);
			
			const PosSet &getUnknownRevealedFrom(const Pos &pos, Dir dir) const;
			bool canSeeUnknownInAnyDirFrom(const Pos &pos) const;
			
		private:
			const WorldGrid &grid;
			const SensorPredictor &sensorpred;
			
			typedef boost::unordered_map<std::pair<Pos, Dir>, PosSet> UnknownPosCacheMap;
			mutable UnknownPosCacheMap unknownposes_cache;
	};
}

#endif

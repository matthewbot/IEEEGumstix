#include "ieee/planner/SensorPredictorCache.h"

using namespace ieee;
using namespace std;

SensorPredictorCache::SensorPredictorCache(const WorldGrid &grid, const CoordScale &gridscale, const CoordScale &nodescale, const SensorPredictor &sensorpred)
: grid(grid), gridscale(gridscale), nodescale(nodescale), sensorpred(sensorpred) { }

const PosSet &SensorPredictorCache::getUnknownRevealedFrom(const Pos &pos, Dir dir) const {
	UnknownPosCacheMap::key_type key = make_pair(pos, dir);
	UnknownPosCacheMap::iterator i = unknownposes_cache.find(key);
	if (i != unknownposes_cache.end())
		return i->second;

	i = unknownposes_cache.insert(make_pair(key, PosSet())).first;
	PosSet &unknownposes = i->second;

	PosSet poses = sensorpred.predictVision(nodescale.posToCoord(pos), dirToRad(dir), grid, gridscale);
	for (PosSet::const_iterator i = poses.begin(); i != poses.end(); ++i) {
		if (grid[*i] == WorldGrid::UNKNOWN)
			unknownposes.insert(*i);
	}

	return unknownposes;
}

bool SensorPredictorCache::canSeeUnknownInAnyDirFrom(const Pos &pos) const {
	for (Dir dir=DIR_E; dir<MAX_DIR; dir=(Dir)(dir+1)) {
		if (getUnknownRevealedFrom(pos, dir).size() > 0)
			return true;
	}

	return false;
}


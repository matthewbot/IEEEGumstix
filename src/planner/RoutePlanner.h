#ifndef ROUTEPLANNER_H
#define ROUTEPLANNER_H

#include "WorldGrid.h"
#include "AStarSearch.h"
#include "SensorPredictor.h"
#include "types.h"
#include <vector>
#include <boost/unordered_map.hpp>

namespace pathsim {
	class RoutePlanner {
		public:
			struct Route {
				Path path;
				DirVec facedirs;
			};
			
			RoutePlanner(const SensorPredictor &sensorpred, const WorldGrid &map);
			
			Route planRoute(const Pos &curpos, Dir curdir) const;
			
			bool isVictimIdentified(const Pos &pos) const;
			void setVictimIdentified(const Pos &pos);
			void resetVictims();
			
		private:
			int scorePath(const AStarSearch &search, Dir curdir, DirVec &bestdirs) const;
			PosSet getUnknownRevealedFrom(const Pos &pos, Dir dir) const;
			bool canSeeUnknownInAnyDirFrom(const Pos &pos) const;
			PosSet getBestUnknownRevealedFrom(const Pos &pos, Dir prevdir, Dir &bestdir, const PosSet &revealed, bool mustsee) const;			
		
			void clearSensorCache() const;
			const PosSet &predictSensor(const Pos &pos, Dir dir) const;
		
			const SensorPredictor &sensorpred;
			const WorldGrid &map;
			PosSet identifiedvictims;
			
			typedef boost::unordered_map<std::pair<Pos, Dir>, PosSet> SensorCacheMap;
			mutable SensorCacheMap sensorpred_cache;
	};
}

#endif


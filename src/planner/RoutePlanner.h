#ifndef ROUTEPLANNER_H
#define ROUTEPLANNER_H

#include "WorldGrid.h"
#include "AStarSearch.h"
#include "SensorPredictor.h"
#include "types.h"

namespace pathsim {
	class RoutePlanner {
		public:
			struct Route {
				Path path;
				Dir facedir;
			};
			
			RoutePlanner(const SensorPredictor &sensorpred, const WorldGrid &map);
			
			Route planRoute(const Pos &curpos, Dir curdir) const;
			
			bool isVictimIdentified(const Pos &pos) const;
			void setVictimIdentified(const Pos &pos);
			void resetVictims();
			
		private:
			int scorePath(const AStarSearch &search, Dir &bestdir) const;
			int countUnknownRevealedFrom(const Pos &pos, Dir curdir) const;
			bool canSeeUnknownInAnyDirFrom(const Pos &pos) const;
			int countMostUnknownRevealedFrom(const Pos &pos, Dir *bestdir=NULL) const;			
		
			const SensorPredictor &sensorpred;
			const WorldGrid &map;
			PosSet identifiedvictims;
	};
}

#endif


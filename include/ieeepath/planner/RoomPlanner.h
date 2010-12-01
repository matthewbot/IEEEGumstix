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

			struct Config {
			    float roomwidth, roomheight;
			    int nodewidth, nodeheight;
			    float nodeoffsetx, nodeoffsety;

			    RouteEvaluator::Config routeevalconfig;
			};

			RoomPlanner(const SensorPredictor &sensorpred, const WorldGrid &worldmap, const Config &config);

            inline const CoordScale &getGridScale() const { return gridscale; }
            inline const CoordScale &getNodeScale() const { return nodescale; }

			Plan planRoute(const Coord &curcoord, Dir curdir);

			inline bool isVictimIdentified(const Pos &pos) const { return false; }
			inline void setVictimIdentified(const Pos &pos) { }
			inline void resetVictims() { }

		private:
			const Config &config;
			const SensorPredictor &sensorpred;
			const WorldGrid &worldmap;
	        const CoordScale gridscale;
	        const CoordScale nodescale;

			PosSet identifiedvictims;

			static bool unknownAdjacent(const NodeGrid &map, const Pos &pos);

	};
}

#endif

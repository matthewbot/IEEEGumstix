#ifndef ROBOT_H
#define ROBOT_H

#include "ieee/planner/RoomPlanner.h"
#include "ieee/planner/LaserSensorPredictor.h"
#include "ieee/shared/WorldGrid.h"
#include "ieee/shared/types.h"
#include <vector>

namespace ieee {
	class Robot {
		public:
			Robot(const Coord &startpos, const WorldGrid &grid, const RoomPlanner::Config &roomplannerconfig);

			void reset(const Coord &pos, float dir=0);
			void step();

			inline bool isVictimIdentified(const Pos &pos) const { return roomplanner.isVictimIdentified(pos); }
			inline const PosSet &getIdentifiedVictims() const { return roomplanner.getIdentifiedVictims(); }
			inline const Coord &getPosition() const { return curpos; }
			inline float getDirection() const { return curdir; }
			inline const Coord &getDestination() const { return plan.coords.back(); }
			inline const RoomPlanner::Plan &getPlan() const { return plan; }
			inline const WorldGrid &getMap() const { return map; }
			inline const CoordScale &getGridScale() const { return roomplanner.getGridScale(); }
			inline const CoordScale &getVictimScale() const { return roomplanner.getVictimScale(); }

		private:
			void moveStep();
			void updateSensorsStep();
			void computeDestStep();
			void updatePathStep();

			LaserSensorPredictor sensorpred;

			Coord curpos;
			float curdir;
			const WorldGrid &grid;

			WorldGrid map;
			RoomPlanner::Plan plan;

			RoomPlanner roomplanner;
	};
}

#endif

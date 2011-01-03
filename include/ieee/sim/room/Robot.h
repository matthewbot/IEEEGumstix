#ifndef ROBOT_H
#define ROBOT_H

#include "ieee/planner/RoomPlanner.h"
#include "ieee/planner/LaserSensorPredictor.h"
#include "ieee/shared/WorldGrid.h"
#include "ieee/shared/types.h"
#include <boost/scoped_ptr.hpp>
#include <vector>

namespace ieee {
	class Robot {
		public:
			Robot(const WorldGrid &grid, const RoomPlanner::Config &roomplannerconfig, const Coord &pos, float dir=0);

			// reset must be called in the RoomPlanner::Config is modified, as the RoomPlanner object must be recreated
			void reset(const RoomPlanner::Config &roomplannerconfig, const Coord &pos, float dir=0);
			void step();

			inline bool isVictimIdentified(const Pos &pos) const { return roomplannerptr->isVictimIdentified(pos); }
			inline const PosSet &getIdentifiedVictims() const { return roomplannerptr->getIdentifiedVictims(); }
			inline const Coord &getPosition() const { return curpos; }
			inline float getDirection() const { return curdir; }
			inline const Coord &getDestination() const { return plan.coords.back(); }
			inline const RoomPlanner::Plan &getPlan() const { return plan; }
			inline const WorldGrid &getMap() const { return map; }

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

			boost::scoped_ptr<RoomPlanner> roomplannerptr;
			const RoomPlanner::Config *roomplannerconfigptr;
	};
}

#endif

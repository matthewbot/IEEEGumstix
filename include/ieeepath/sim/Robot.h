#ifndef ROBOT_H
#define ROBOT_H

#include "ieeepath/planner/RoomPlanner.h"
#include "ieeepath/planner/CameraSensorPredictor.h"
#include "ieeepath/shared/WorldGrid.h"
#include "ieeepath/shared/types.h"
#include <vector>

namespace ieee {
	class Robot {
		public:
			Robot(const Coord &startpos, const WorldGrid &grid);

			void reset(const Coord &pos, float dir=0);
			void step();

			inline bool isVictimIdentified(const Pos &pos) const { return false; }
			inline const Coord &getPosition() const { return curpos; }
			inline float getDirection() const { return curdir; }
			inline const Coord &getDestination() const { return plan.coords.back(); }
			inline const RoomPlanner::Plan &getPlan() const { return plan; }
			inline const WorldGrid &getMap() const { return map; }
			inline const CoordScale &getGridScale() const { return roomplanner.getGridScale(); }

		private:
			void moveStep();
			void updateSensorsStep();
			void computeDestStep();
			void updatePathStep();

			CameraSensorPredictor sensorpred;

			Coord curpos;
			float curdir;
			const WorldGrid &grid;

			WorldGrid map;
			RoomPlanner::Plan plan;

			struct RoomPlannerConfig : public RoomPlanner::Config {
				RoomPlannerConfig();
			};

			RoomPlannerConfig roomplannerconfig;
			RoomPlanner roomplanner;
	};
}

#endif

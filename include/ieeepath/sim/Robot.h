#ifndef ROBOT_H
#define ROBOT_H

#include "ieeepath/planner/RouteEvaluator.h"
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
			
			inline bool isVictimIdentified(const Pos &pos) const { return routeplanner.isVictimIdentified(pos); }
			inline const Coord &getPosition() const { return curpos; }
			inline float getDirection() const { return curdir; }
			inline const Coord &getDestination() const { return route.coords.back(); }
			inline const RouteEvaluator::Route &getRoute() const { return route; }
			inline const WorldGrid &getMap() const { return map; }
			
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
			RouteEvaluator::Route route;
			
			struct RouteEvaluatorConfig : public RouteEvaluator::Config {
				RouteEvaluatorConfig();
			};
			
			RouteEvaluatorConfig routeplannerconfig;
			RouteEvaluator routeplanner;
	};
}

#endif

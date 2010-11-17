#include "ieeepath/planner/RoomPlanner.h"
#include "ieeepath/planner/RouteEvaluator.h"
#include "ieeepath/shared/Timer.h"
#include <algorithm>

using namespace ieee;

RoomPlanner::RoomPlanner(const SensorPredictor &sensorpred, const WorldGrid &worldmap, const RouteEvaluator::Config &config)
: sensorpred(sensorpred), worldmap(worldmap), config(config) { }

RoomPlanner::Plan RoomPlanner::planRoute(const Pos &curpos, Dir curdir) {
	NodeGrid map = NodeGrid::fromWorldGrid(worldmap);
	
	RouteEvaluator routeeval(sensorpred, map, worldmap, config);
	
	for (int x=0; x<map.getWidth(); x++) {
		for (int y=0; y<map.getHeight(); y++) {
			Pos pos(x, y);
			const Node &node = map[pos];
			if (node.getType() != Node::OPEN || !unknownAdjacent(map, pos))
				continue;
			
			routeeval.addDestination(pos);
		}
	}
	
	RouteEvaluator::NodeRoute noderoute = routeeval.planRoute(curpos, curdir);
	
	Plan plan;
	plan.identifyvictim = false;	
	plan.facedirs = noderoute.facedirs;
	plan.coords.resize(noderoute.poses.size());
	for (int i=0; i<plan.coords.size(); i++) {
		plan.coords[i] = Coord(noderoute.poses[i]);
	}
	
	return plan;
}

bool RoomPlanner::unknownAdjacent(const NodeGrid &map, const Pos &pos) {
	for (int dx=-1; dx<=1; dx++) {
		for (int dy=-1; dy<=1; dy++) {
			if (dx == 0 && dy == 0)
				continue;
			Pos p(pos.x + dx, pos.y + dy);
			
			if (!map.inBounds(p))
				continue;
				
			if (map[p] == Node::UNKNOWN)
				return true;
		}
	}
	
	return false;
}


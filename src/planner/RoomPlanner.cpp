#include "ieeepath/planner/RoomPlanner.h"
#include "ieeepath/planner/RouteEvaluator.h"
#include "ieeepath/shared/Timer.h"
#include <iostream>

using namespace ieee;
using namespace std;

RoomPlanner::RoomPlanner(const SensorPredictor &sensorpred, const WorldGrid &worldmap, const Config &config)
: sensorpred(sensorpred), worldmap(worldmap), config(config),
  gridscale(worldmap.getWidth() / config.roomwidth, worldmap.getHeight() / config.roomheight, -.5, -.5),
  nodescale(config.nodewidth / config.roomwidth, config.nodeheight / config.roomheight, config.nodeoffsetx, config.nodeoffsety),
  victimscale(10 / config.roomwidth, 10 / config.roomheight, -.5, -.5) { }

RoomPlanner::Plan RoomPlanner::planRoute(const Coord &curcoord, Dir curdir) const {
	Pos curpos = nodescale.coordToPos(curcoord);
	NodeGrid map = NodeGrid::fromWorldGrid(worldmap, gridscale, nodescale);

	Plan victimplan = planIdentifyNearestVictim(curpos, curdir, map);
	if (victimplan.coords.size() > 0)
		return victimplan;
	else
		return planSearchUnknown(curpos, curdir, map);
}

PosList RoomPlanner::findUnidentifiedVictimPoses() const {
	PosList victimposes;

	for (int xpos=0; xpos<10; xpos++) {
		for (int ypos=0; ypos<10; ypos++) {
			Pos pos(xpos, ypos);

			if (identifiedvictims.find(pos) != identifiedvictims.end())
				continue;

			Coord center = victimscale.posToCoord(pos);
			Pos topleft = gridscale.coordToPos(Coord(center.x - 4, center.y - 4));
			Pos bottomright = gridscale.coordToPos(Coord(center.x + 4, center.y + 4));

			int count=0;
			for (int x=topleft.x; x<=bottomright.x; x++) {
				for (int y=topleft.y; y<=bottomright.y; y++) {
					if (worldmap[Pos(x,y)] == WorldGrid::VICTIM)
						count++;
				}
			}

			if (count > (bottomright.x - topleft.x)*(bottomright.y - topleft.y)/4)
				victimposes.push_back(pos);
		}
	}

	return victimposes;
}

RoomPlanner::Plan RoomPlanner::planIdentifyNearestVictim(const Pos &curpos, Dir curdir, const NodeGrid &map) const {
	PosList bestpath;
	Pos bestvictimpos;
	int bestcost=9999;

	PosList victimposes = findUnidentifiedVictimPoses();
	for (PosList::const_iterator victimpos = victimposes.begin(); victimpos != victimposes.end(); ++victimpos) {
		Coord victimcoord = victimscale.posToCoord(*victimpos);
		Pos upperleft = nodescale.coordToPos(Coord(victimcoord.x-15,victimcoord.y-15));
		Pos lowerright = nodescale.coordToPos(Coord(victimcoord.x+15,victimcoord.y+15));

		for (int x=upperleft.x; x<=lowerright.x; x++) {
			for (int y=upperleft.y; y<=lowerright.y; y++) {
				Pos destpos(x, y);

				if (!map.inBounds(destpos))
					continue;

				if (map[destpos] != Node::OPEN)
					continue;

				AStarSearch search(map, curpos, destpos);
				if (!search.foundPath())
					continue;

				if (search.getPathCost() >= bestcost)
					continue;

				bestpath = search.getPath();
				bestvictimpos = *victimpos;
				bestcost = search.getPathCost();
			}
		}
	}

	if (bestpath.size() == 0)
		return Plan();

	Coord bestvictimcoord = victimscale.posToCoord(bestvictimpos);
	Coord pathendcoord = nodescale.posToCoord(bestpath.back());
	Dir identifydir = radToNearestDir(atan2(-(bestvictimcoord.y - pathendcoord.y), bestvictimcoord.x - pathendcoord.x));

	return planFromPosList(bestpath, identifydir, &bestvictimpos);
}

RoomPlanner::Plan RoomPlanner::planFromPosList(const PosList &path, Dir facedir, Pos *victimpos) const {
	Plan plan;

	if (victimpos) {
		plan.identifyvictim = true;
		plan.victimpos = *victimpos;
	} else
		plan.identifyvictim = false;

	plan.coords.resize(path.size());
	for (int i=0; i<path.size(); i++) {
		plan.coords[i] = nodescale.posToCoord(path[i]);
	}

	plan.facedirs.resize(path.size(), facedir);

	return plan;
}

RoomPlanner::Plan RoomPlanner::planSearchUnknown(const Pos &curpos, Dir curdir, const NodeGrid &map) const {
	SensorPredictorCache pred(worldmap, gridscale, nodescale, sensorpred);
	RouteEvaluator routeeval(pred, map, config.routeevalconfig);

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
	return planFromNodeRoute(noderoute);
}

RoomPlanner::Plan RoomPlanner::planFromNodeRoute(const RouteEvaluator::NodeRoute &noderoute) const {
	Plan plan;
	plan.identifyvictim = false;
	plan.facedirs = noderoute.facedirs;

	plan.coords.resize(noderoute.poses.size());
	for (int i=0; i<plan.coords.size(); i++) {
		plan.coords[i] = nodescale.posToCoord(noderoute.poses[i]);
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

RoomPlanner::Plan::Plan() : identifyvictim(false) { }


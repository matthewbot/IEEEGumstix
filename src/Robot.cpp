#include "Robot.h"
#include <algorithm>

using namespace pathsim;
using namespace std;

Robot::Robot(int sensorrange, const Pos &startpos, const WorldGrid &grid) 
: sensorrange(sensorrange), 
  grid(grid), 
  map(grid.getWidth(), grid.getHeight()) {
	reset(startpos);
}

void Robot::reset(const Pos &pos) {
	curpos = pos;
	map.clear(WorldGrid::UNKNOWN);
	identifiedvictims.clear();
	
	updateSensorsStep();
	updateRouteStep();
}

void Robot::step() {
	moveStep();
	updateSensorsStep();
	updateRouteStep();
}

void Robot::moveStep() {
	curpos = route[0];
	
	int victimx, victimy;
	if (map.getAdjacent(curpos.x, curpos.y, WorldGrid::VICTIM, &victimx, &victimy))
		identifiedvictims.push_back(Pos(victimx, victimy));
}

void Robot::updateSensorsStep() {
	const int minx = max(curpos.x-sensorrange, 0);
	const int maxx = min(curpos.x+sensorrange, map.getWidth()-1);
	const int miny = max(curpos.y-sensorrange, 0);
	const int maxy = min(curpos.y+sensorrange, map.getHeight()-1);
	
	for (int x=minx; x<=maxx; x++) {
		for (int y=miny; y<=maxy; y++) {
			map(x, y) = grid(x, y);
		}
	}
}

void Robot::updateRouteStep() {
	route.clear();
	
	int bestscore = 9999;
	for (int x=0; x<map.getWidth(); x++) {
		for (int y=0; y<map.getHeight(); y++) {
			Pos pos(x, y);
			if (!map.getPassable(x, y))
				continue;

			if (!map.getAdjacent(x, y, WorldGrid::UNKNOWN)) {
				int victimx, victimy;
				if (!map.getAdjacent(x, y, WorldGrid::VICTIM, &victimx, &victimy))
					continue;
					
				if (find(identifiedvictims.begin(), identifiedvictims.end(), Pos(victimx, victimy)) != identifiedvictims.end())
					continue;
			}
			
			AStarSearch search(map, curpos, pos);
			if (!search.foundRoute())
				continue;
				
			int score = scoreRoute(search);
			if (score >= bestscore)
				continue;
				
			bestscore = score;
			route = search.getRoute();
		}
	}
}

int Robot::scoreRoute(const AStarSearch &search) const {
	const Pos &dest = search.getRoute().back();
	int score=0;

	score += search.getRouteCost()/3;
	
	if (dest.x < sensorrange)
		score += 2 * (sensorrange - dest.x);
	else if (dest.x >= map.getWidth() - sensorrange)
		score += 2 * (sensorrange - (map.getWidth() - dest.x - 1));
	if (dest.y < sensorrange)
		score += 2 * (sensorrange - dest.y);
	else if (dest.y >= map.getHeight() - sensorrange)
		score += 2 * (sensorrange - (map.getHeight() - dest.y - 1));		

	int victimx, victimy;
	if (map.getAdjacent(dest.x, dest.y, WorldGrid::VICTIM, &victimx, &victimy)) {
		if (find(identifiedvictims.begin(), identifiedvictims.end(), Pos(victimx, victimy)) == identifiedvictims.end())
			score -= 500;
	}
	
	return score;
}



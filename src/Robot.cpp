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

void Robot::reset(const Pos &pos, Dir dir) {
	curpos = pos;
	curdir = dir;
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
	curdir = getDirFromPoses(curpos, route[0]);
	curpos = route[0];
	
	Pos victim;
	if (map.getAdjacent(curpos, WorldGrid::VICTIM, &victim))
		identifiedvictims.push_back(victim);
}

void Robot::updateSensorsStep() {
	const int minx = max(curpos.x-sensorrange, 0);
	const int maxx = min(curpos.x+sensorrange, map.getWidth()-1);
	const int miny = max(curpos.y-sensorrange, 0);
	const int maxy = min(curpos.y+sensorrange, map.getHeight()-1);
	
	for (int x=minx; x<=maxx; x++) {
		for (int y=miny; y<=maxy; y++) {
			Pos pos(x, y);
			map[pos] = grid[pos];
		}
	}
}

#include <iostream>

void Robot::updateRouteStep() {
	cout << "--------Update--------" << endl;
	route.clear();
	
	int bestscore = 9999;
	for (int x=0; x<map.getWidth(); x++) {
		for (int y=0; y<map.getHeight(); y++) {
			Pos pos(x, y);
			if (!map.getPassable(pos))
				continue;

			if (!map.getAdjacent(pos, WorldGrid::UNKNOWN, NULL, sensorrange)) {
				Pos victim;
				if (!map.getAdjacent(pos, WorldGrid::VICTIM, &victim))
					continue;
					
				if (identifiedVictim(victim))
					continue;
			}
			
			AStarSearch search(map, curpos, pos);
			if (!search.foundRoute())
				continue;
				
			int score = scoreRoute(search);
			cout << "Evaluated " << pos << " score " << score << endl;
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

	score += search.getRouteCost()/2;

	Pos victim;
	if (map.getAdjacent(dest, WorldGrid::VICTIM, &victim)) {
		if (!identifiedVictim(victim))
			score -= 500;
	}
	
	score -= 2*map.countAdjacent(dest, WorldGrid::UNKNOWN, sensorrange);
	
	return score;
}

bool Robot::identifiedVictim(const Pos &pos) const {
	return find(identifiedvictims.begin(), identifiedvictims.end(), pos) != identifiedvictims.end();
}



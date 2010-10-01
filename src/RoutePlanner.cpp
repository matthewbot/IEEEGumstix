#include "RoutePlanner.h"
#include <algorithm>

using namespace pathsim;
using namespace std;

RoutePlanner::RoutePlanner(const WorldGrid &map, int sensorrange) : map(map), sensorrange(sensorrange) { }

#include <iostream>

RoutePlanner::Route RoutePlanner::planRoute(const Pos &curpos, Dir curdir) const {
	cout << "--------Update--------" << endl;
	Route route;
	
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
					
				if (isVictimIdentified(victim))
					continue;
			}
			
			AStarSearch search(map, curpos, pos);
			if (!search.foundPath())
				continue;
				
			int score = scorePath(search);
			cout << "Evaluated " << pos << " score " << score << endl;
			if (score >= bestscore)
				continue;
				
			bestscore = score;
			route.path = search.getPath();
		}
	}
	
	return route;
}

int RoutePlanner::scorePath(const AStarSearch &search) const {
	const Pos &dest = search.getPath().back();
	int score=0;

	score += search.getPathCost()/2;

	Pos victim;
	if (map.getAdjacent(dest, WorldGrid::VICTIM, &victim)) {
		if (!isVictimIdentified(victim))
			score -= 500;
	}
	
	score -= 2*map.countAdjacent(dest, WorldGrid::UNKNOWN, sensorrange);
	
	return score;
}

bool RoutePlanner::isVictimIdentified(const Pos &pos) const {
	return find(identifiedvictims.begin(), identifiedvictims.end(), pos) != identifiedvictims.end();
}

void RoutePlanner::setVictimIdentified(const Pos &pos) {
	identifiedvictims.push_back(pos);
}

void RoutePlanner::resetVictims() {
	identifiedvictims.clear();
}


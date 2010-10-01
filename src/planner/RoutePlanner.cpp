#include "RoutePlanner.h"
#include <algorithm>

using namespace pathsim;
using namespace std;

RoutePlanner::RoutePlanner(const SensorPredictor &sensorpred, const WorldGrid &map) : sensorpred(sensorpred), map(map) { }

#include <iostream>

RoutePlanner::Route RoutePlanner::planRoute(const Pos &curpos, Dir curdir) const {
	cout << "--------Update--------" << endl;
	Route route;
	route.facedir = DIR_E;
	
	int bestscore = 9999;
	for (int x=0; x<map.getWidth(); x++) {
		for (int y=0; y<map.getHeight(); y++) {
			Pos pos(x, y);
			if (!map.getPassable(pos))
				continue;
			
			if (!canSeeUnknownInAnyDirFrom(pos)) {
				Pos victim;
				if (!map.getAdjacent(pos, WorldGrid::VICTIM, &victim))
					continue;
					
				if (isVictimIdentified(victim))
					continue;
			}
			
			AStarSearch search(map, curpos, pos);
			if (!search.foundPath())
				continue;
			
			Dir bestdir;
			int score = scorePath(search, bestdir);
			cout << "Evaluated " << pos << " score " << score << endl;
			if (score >= bestscore)
				continue;
				
			bestscore = score;
			route.path = search.getPath();
			route.facedir = bestdir;
		}
	}
	
	return route;
}

int RoutePlanner::scorePath(const AStarSearch &search, Dir &bestdir) const {
	const Pos &dest = search.getPath().back();
	int score=0;

	score += search.getPathCost()/2;

	Pos victim;
	if (map.getAdjacent(dest, WorldGrid::VICTIM, &victim)) {
		if (!isVictimIdentified(victim))
			score -= 500;
	}
	
	score -= 2*countMostUnknownRevealedFrom(dest, &bestdir);
	
	return score;
}

int RoutePlanner::countUnknownRevealedFrom(const Pos &pos, Dir dir) const {
	int count=0;

	PosSet seenset = sensorpred.predictVision(pos, dir, map);
	for (PosSet::const_iterator i = seenset.begin(); i != seenset.end(); ++i) {
		if (map[*i] == WorldGrid::UNKNOWN)
			count++;
	}
	
	return count;
}

bool RoutePlanner::canSeeUnknownInAnyDirFrom(const Pos &pos) const {
	for (Dir dir=DIR_E; dir<MAX_DIR; dir=(Dir)(dir+1)) {
		if (countUnknownRevealedFrom(pos, dir) > 0)
			return true;
	}
	
	return false;
}

int RoutePlanner::countMostUnknownRevealedFrom(const Pos &pos, Dir *bestdir) const {
	int bestcount=0;
	
	for (Dir dir=DIR_E; dir<MAX_DIR; dir=(Dir)(dir+1)) {
		int count = countUnknownRevealedFrom(pos, dir);
		if (count > bestcount) {
			bestcount = count;
			if (bestdir)
				*bestdir = dir;
		}
	}
	
	return bestcount;
}

bool RoutePlanner::isVictimIdentified(const Pos &pos) const {
	return identifiedvictims.find(pos) != identifiedvictims.end();
}

void RoutePlanner::setVictimIdentified(const Pos &pos) {
	identifiedvictims.insert(pos);
}

void RoutePlanner::resetVictims() {
	identifiedvictims.clear();
}


#include "RoutePlanner.h"
#include <algorithm>

using namespace pathsim;
using namespace std;

RoutePlanner::RoutePlanner(const SensorPredictor &sensorpred, const WorldGrid &map) : sensorpred(sensorpred), map(map) { }

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
			
			DirVec bestdirs;
			bestdirs.resize(search.getPathLength());
			
			int score = scorePath(search, curdir, bestdirs);
			cout << "Evaluated " << pos << " facing " << bestdirs.back() << " score " << score << endl;
			if (score >= bestscore)
				continue;
				
			bestscore = score;
			route.path = search.getPath();
			route.facedirs = bestdirs;
		}
	}
	
	return route;
}

int RoutePlanner::scorePath(const AStarSearch &search, Dir curdir, DirVec &bestdirs) const {
	const Pos &dest = search.getPath().back();
	int score=0;

	score += search.getPathCost()/3;

	Pos victim;
	bool havevictim;
	if (map.getAdjacent(dest, WorldGrid::VICTIM, &victim)) {
		if (!isVictimIdentified(victim)) {
			havevictim = true;
			score -= 500;
		}
	}
	
	PosSet revealed;
	for (int i=0; i < search.getPathLength(); ++i) {
		if (havevictim && i == search.getPathLength()-1) {
			bestdirs[i] = getDirFromPoses(search.getPath()[i], victim);
			continue;
		}
		
		Dir prevdir;
		if (i == 0)
			prevdir = curdir;
		else
			prevdir = bestdirs[i-1];
		
		bool mustsee = (i == search.getPathLength() - 1) && (revealed.size() == 0) && !havevictim;
		PosSet newseen = getBestUnknownRevealedFrom(search.getPath()[i], prevdir, bestdirs[i], revealed, mustsee);
		revealed.insert(newseen.begin(), newseen.end());	
	}	
	
	score -= 2*revealed.size();
	
	return score;
}

PosSet RoutePlanner::getUnknownRevealedFrom(const Pos &pos, Dir dir) const {
	PosSet poses = sensorpred.predictVision(pos, dir, map);
	
	for (PosSet::const_iterator i = poses.begin(); i != poses.end();) {
		if (map[*i] == WorldGrid::UNKNOWN)
			++i;
		else {
			poses.erase(i);
			i = poses.begin();
		}
	}
	
	return poses;
}

bool RoutePlanner::canSeeUnknownInAnyDirFrom(const Pos &pos) const {
	for (Dir dir=DIR_E; dir<MAX_DIR; dir=(Dir)(dir+1)) {
		if (getUnknownRevealedFrom(pos, dir).size() > 0)
			return true;
	}
	
	return false;
}

PosSet RoutePlanner::getBestUnknownRevealedFrom(const Pos &pos, Dir prevdir, Dir &bestdir, const PosSet &revealed, bool mustsee) const {
	PosSet bestset;
	int bestscore=999;
	
	for (Dir dir=DIR_E; dir<MAX_DIR; dir=(Dir)(dir+1)) { 
		int score=0;
		
		score += 2*abs(getDirDelta(prevdir, dir)); // TODO changeable
		
		PosSet set = getUnknownRevealedFrom(pos, dir);
		for (PosSet::const_iterator i = revealed.begin(); i != revealed.end(); ++i)
			set.erase(*i);
		
		score -= set.size();
	
		if (set.size() == 0 && mustsee)
			continue;
	
		if (score < bestscore) {
			bestset = set;
			bestdir = dir;
			bestscore = score;
		}
	}
	
	return bestset;
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


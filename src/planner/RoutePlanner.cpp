#include "RoutePlanner.h"
#include <algorithm>
#include <iostream>
#include <ctime>

using namespace pathsim;
using namespace std;

RoutePlanner::RoutePlanner(const SensorPredictor &sensorpred, const WorldGrid &map) : sensorpred(sensorpred), map(map) { }

RoutePlanner::Route RoutePlanner::planRoute(const Pos &curpos, Dir curdir) const {
	timespec start;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
	
	clearSensorCache();
	Route route;
	
	int bestscore = 9999;
	for (int x=0; x<map.getWidth(); x++) {
		for (int y=0; y<map.getHeight(); y++) {
			Pos pos(x, y);
			if (!map.getPassable(pos))
				continue;
			
			if (!map.getAdjacent(pos, WorldGrid::UNKNOWN, NULL, 3) || !canSeeUnknownInAnyDirFrom(pos)) {
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
			if (score >= bestscore)
				continue;
				
			bestscore = score;
			route.path = search.getPath();
			route.facedirs = bestdirs;
		}
	}
	
	timespec end;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
	
	float tdelta = (float)(end.tv_sec - start.tv_sec) + (float)(end.tv_nsec - start.tv_nsec)/1E9;
	cout << "Route planned in " << tdelta * 1000 << " ms" << endl;
	
	return route;
}

int RoutePlanner::scorePath(const AStarSearch &search, Dir curdir, DirVec &bestdirs) const {
	const Pos &dest = search.getPath().back();
	int score=0;

	score += search.getPathCost()/3;

	Pos victim;
	bool havevictim=false;
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
	PosSet poses = predictSensor(pos, dir);
	
	PosSet unknownposes;
	
	for (PosSet::const_iterator i = poses.begin(); i != poses.end(); ++i) {
		if (map[*i] == WorldGrid::UNKNOWN)
			unknownposes.insert(*i);
	}
	
	return unknownposes;
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

void RoutePlanner::clearSensorCache() const {
	sensorpred_cache.clear();
}

const PosSet &RoutePlanner::predictSensor(const Pos &pos, Dir dir) const {
	SensorCacheMap::key_type key = make_pair(pos, dir);
	SensorCacheMap::iterator i = sensorpred_cache.find(key);
	if (i == sensorpred_cache.end()) {
		PosSet poses = sensorpred.predictVision(pos, dir, map);
		i = sensorpred_cache.insert(make_pair(key, poses)).first;
	}
	
	return i->second;
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


#include "ieeepath/planner/RoutePlanner.h"
#include "ieeepath/shared/Timer.h"
#include <algorithm>
#include <iostream>
#include <ctime>

using namespace ieeepath;
using namespace std;

RoutePlanner::RoutePlanner(const SensorPredictor &sensorpred, const WorldGrid &worldmap, const Config &config)
: sensorpred(sensorpred), worldmap(worldmap), map(worldmap.getWidth()-1, worldmap.getHeight()-1), config(config) { }

RoutePlanner::Route RoutePlanner::planRoute(const Pos &curpos, Dir curdir) const {
	Timer tim;
	
	map = NodeGrid::fromWorldGrid(worldmap);
	clearSensorCache();
	
	Route route;
	
	int bestscore = 9999;
	for (int x=0; x<map.getWidth(); x++) {
		for (int y=0; y<map.getHeight(); y++) {
			Pos pos(x, y);
			const Node &node = map[pos];
			
			Pos victimpos;
			bool destvictim = false;
			if (node.getType() == Node::VICTIM_ID) {
				if (!getVictimPos(pos, worldmap, node.getDir(), victimpos))
					throw runtime_error("Bad VICTIM_ID node?");
					
				if (!isVictimIdentified(victimpos))
					destvictim = true; 
			} else if (node.getType() != Node::OPEN)
				continue;
			
			if (!destvictim && !canSeeUnknownInAnyDirFrom(pos))
				continue;
			
			AStarSearch search(map, curpos, pos);
			if (!search.foundPath())
				continue;
			
			DirVec bestdirs;
			bestdirs.resize(search.getPathLength());
			
			int score = scorePath(search, curdir, bestdirs, destvictim);
			if (score >= bestscore)
				continue;
				
			bestscore = score;
			route.path = search.getPath();
			route.facedirs = bestdirs;
			route.identifyvictim = destvictim;
			route.victimpos = victimpos;
		}
	}
	
	
	cout << "Route planned in " << tim.getSeconds() * 1000 << " ms" << endl;
	
	return route;
}

int RoutePlanner::scorePath(const AStarSearch &search, Dir curdir, DirVec &bestdirs, bool destvictim) const {
	const Pos &dest = search.getPath().back();
	int score=0;
	
	if (destvictim)
		score -= 500;
	score += search.getPathCost() * (destvictim ? config.pathCostFactorVictim : config.pathCostFactor);
	
	PosSet revealed;
	for (int i=0; i < search.getPathLength(); ++i) {
		if (destvictim && i == search.getPathLength()-1) {
			bestdirs[i] = map[dest].getDir();
			continue;
		}
		
		Dir prevdir;
		if (i == 0)
			prevdir = curdir;
		else
			prevdir = bestdirs[i-1];
		
		bool mustsee = (i == search.getPathLength() - 1) && (revealed.size() == 0) && !destvictim;
		PosSet newseen = getBestUnknownRevealedFrom(search.getPath()[i], prevdir, bestdirs[i], revealed, mustsee);
		revealed.insert(newseen.begin(), newseen.end());	
	}	
	
	score -= revealed.size() * config.revealedScoreFactor;
	
	return score;
}

const PosSet &RoutePlanner::getUnknownRevealedFrom(const Pos &pos, Dir dir) const {
	UnknownPosCacheMap::key_type key = make_pair(pos, dir);
	UnknownPosCacheMap::iterator i = unknownposes_cache.find(key);
	if (i != unknownposes_cache.end())
		return i->second;
	
	i = unknownposes_cache.insert(make_pair(key, PosSet())).first;
	PosSet &unknownposes = i->second;

	PosSet poses = sensorpred.predictVision(pos, dir, worldmap);
	for (PosSet::const_iterator i = poses.begin(); i != poses.end(); ++i) {
		if (worldmap[*i] == WorldGrid::UNKNOWN)
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
	unknownposes_cache.clear();
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


#include "ieeepath/planner/RouteEvaluator.h"
#include <algorithm>

using namespace ieee;
using namespace std;

RouteEvaluator::RouteEvaluator(const SensorPredictorCache &pred, const NodeGrid &map, const Config &config)
: pred(pred), map(map), config(config) { }

void RouteEvaluator::addDestination(const Pos &pos) {
	destinations.push_back(pos);
}

RouteEvaluator::NodeRoute RouteEvaluator::planRoute(const Pos &curpos, Dir curdir) const {
	NodeRoute route;
	
	int bestscore = 9999;
	for (PosList::const_iterator i = destinations.begin(); i != destinations.end(); ++i) {
		const Pos &pos = *i;
		
		AStarSearch search(map, curpos, pos);
		if (!search.foundPath())
			continue;
		
		DirVec bestdirs;
		bestdirs.resize(search.getPathLength());
		
		int score = scorePath(search, curdir, bestdirs);
		if (score >= bestscore)
			continue;
			
		bestscore = score;
		route.poses = search.getPath();
		route.facedirs = bestdirs;
	}
	
	return route;
}

int RouteEvaluator::scorePath(const AStarSearch &search, Dir curdir, DirVec &bestdirs) const {
	const Pos &dest = search.getPath().back();
	int score=0;
	
	score += search.getPathCost() * config.pathCostFactor;
	
	PosSet revealed;
	for (int i=0; i < search.getPathLength(); ++i) {		
		Dir prevdir;
		if (i == 0)
			prevdir = curdir;
		else
			prevdir = bestdirs[i-1];
		
		bool mustsee = (i == search.getPathLength() - 1) && (revealed.size() == 0);
		PosSet newseen = getBestUnknownRevealedFrom(search.getPath()[i], prevdir, bestdirs[i], revealed, mustsee);
		revealed.insert(newseen.begin(), newseen.end());	
	}	
	
	score -= revealed.size() * config.revealedScoreFactor;
	
	return score;
}

PosSet RouteEvaluator::getBestUnknownRevealedFrom(const Pos &pos, Dir prevdir, Dir &bestdir, const PosSet &revealed, bool mustsee) const {
	PosSet bestset;
	int bestscore=999;
	
	for (Dir dir=DIR_E; dir<MAX_DIR; dir=(Dir)(dir+1)) { 
		int score=0;
		
		score += 2*abs(getDirDelta(prevdir, dir)); // TODO changeable
		
		PosSet set = pred.getUnknownRevealedFrom(pos, dir);
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


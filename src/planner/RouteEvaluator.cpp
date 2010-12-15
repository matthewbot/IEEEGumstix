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

	score += search.getPathCost() * config.pathcostfactor;

	PosSet revealed;
	for (int i=0; i < search.getPathLength(); ++i) {
		Dir prevdir;
		if (i == 0)
			prevdir = curdir;
		else
			prevdir = bestdirs[i-1];

		// flag that indicates we cannot be lazy anymore and must turn at this point to see something new
		// we do this if we're at the end of the route, and haven't yet seen anything new
		// this prevents the robot from planning to go to individual unknown squares but not actually turning to view them,
		// because an individual square usually is not enough to cause the robot to turn
		bool mustsee = (i == search.getPathLength() - 1) && (revealed.size() == 0);

		PosSet newseen = getBestUnknownRevealedFrom(search.getPath()[i], prevdir, bestdirs[i], revealed, mustsee);
		revealed.insert(newseen.begin(), newseen.end());

		if (mustsee) {
			// if we're going to go the whole route only to see one thing only at the end by turning
			// due to the must see flag, we may as well travel the whole route facing that direction
			// this way, we might see the square a bit earlier and not actually travel the whole route
			Dir bestdir = bestdirs[i];
			for (int j=0; j < i; j++)
				bestdirs[j] = bestdir;
		}
	}

	score -= revealed.size() * config.revealedscorefactor;

	return score;
}

PosSet RouteEvaluator::getBestUnknownRevealedFrom(const Pos &pos, Dir prevdir, Dir &bestdir, const PosSet &revealed, bool mustsee) const {
	PosSet bestset;
	int bestscore=999;

	for (Dir dir=DIR_E; dir<MAX_DIR; dir=(Dir)(dir+1)) {
		int score=0;

		if (dir != prevdir) {
			int turnamt = abs(getDirDelta(prevdir, dir));
			score += config.turncostconstant + config.turncostfactor*turnamt/config.turncostdivider;
		}

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


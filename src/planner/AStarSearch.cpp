#include "ieeepath/planner/AStarSearch.h"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <list>

using namespace ieeepath;
using namespace std;

AStarSearch::Square::Square()
: parentdir(DIR_NONE), cost(0), heuristic(0), closed(false) { }

AStarSearch::AStarSearch(const NodeGrid &grid, const Pos &start, const Pos &end)
: width(grid.getWidth()), height(grid.getHeight()), squares(new Square[grid.getWidth()*grid.getHeight()]) {
	if (doSearch(grid, start, end)) {
		Pos curpos=end;
		while (curpos != start) {
			path.push_back(curpos);
			curpos = advancePos(curpos, getSquare(curpos).parentdir);
		}
		path.push_back(start);
		
		reverse(path.begin(), path.end());
	}
}

bool AStarSearch::doSearch(const NodeGrid &grid, const Pos &start, const Pos &end) {
	OpenList openlist;
	openlist.push_back(start); // start off with the starting square on the open list

	while (openlist.size() > 0) {
		Pos curpos = openlist.front(); // get the current best candidate's position
		openlist.erase(openlist.begin()); // remove it from the open list
		
		Square &cursquare = getSquare(curpos); // get the current square
		cursquare.closed = true; // set it to closed so we don't search it again
		
		if (curpos == end) // if we just closed the end square, we're done searching
			return true;
		
		for (Dir dir = DIR_E; dir <= DIR_SE; dir=(Dir)(dir+1)) { // for each possible direction
			Pos pos = advancePos(curpos, dir); // get the position of the location we'd end up
			if (!grid.inBounds(pos)) // skip it if its not valid
				continue;
			if (!grid[pos].traverseableFrom(dir)) // or if its not passable
				continue;
					
			Square &square = getSquare(pos); // get the square of where we'd end up
			if (square.closed) // if its already been searched, skip it
				continue;
		
			int ourcost = cursquare.cost + pathCost(dir, pos, grid); // otherwise compute the cost of moving from our square to this one
			
			if (square.parentdir != DIR_NONE) { // if the other square already has a parent/path to it
				if (square.cost <= ourcost) // and the path from its parent to it is better than the path from us to it
					continue; // then skip it, we're not as good
				
				// otherwise, we're better, and going to recompute its cost
				openlist.erase(find(openlist.begin(), openlist.end(), pos)); // so remove it from the open list for now, since its going to get a new spot
			} else { // the square has never been found before
				square.heuristic = positionHeuristic(pos, end); // go ahead and compute its position heuristic
			}
			
			// if we get here, we're the best path to this square
			square.parentdir = oppositeDir(dir); // set the squares parent direction to the opposite direction we used to find it
			square.cost = ourcost; // set its cost to our computed cost

			insertPosToOpenList(openlist, pos, square.cost + square.heuristic); // put this square on the open list
		}
	}
	
	return false;
}

void AStarSearch::insertPosToOpenList(OpenList &openlist, const Pos &pos, int fscore) {
	OpenList::iterator i;
	for (i = openlist.begin(); i != openlist.end(); ++i) {
		Square &othersquare = getSquare(*i);
		if (othersquare.cost + othersquare.heuristic > fscore)
			break;
	}

	openlist.insert(i, pos);
}

int AStarSearch::pathCost(Dir dir, const Pos &pos, const NodeGrid &grid) {
	int cost;
	if (isDirDiagonal(dir))
		cost = 14;
	else
		cost = 10;
	
	if (grid[pos].getType() == Node::CLIMB)
		cost += 20;
		
	return cost;
}

int AStarSearch::positionHeuristic(const Pos &pos, const Pos &end) {
	return abs(pos.x - end.x) + abs(pos.y - end.y);
}



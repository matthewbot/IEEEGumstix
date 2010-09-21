#include "AStarSearch.h"
#include <algorithm>
#include <cstring>
#include <iostream>

using namespace pathsim;
using namespace std;

bool AStarSearch::Pos::operator==(const Pos &other) const {
	return x == other.x && y == other.y;
}

AStarSearch::Dir AStarSearch::oppositeDir(Dir dir) {
	int newdir = dir + 4;
	if (newdir >= MAX_DIR)
		newdir -= MAX_DIR - 1; // -1 to skip DIR_NONE
	return (Dir)newdir;
}

bool AStarSearch::isDirDiagonal(Dir dir) {
	switch (dir) {
		case DIR_NE:
		case DIR_NW:
		case DIR_SW:
		case DIR_SE:
			return true;
			
		default:
			return false;
	}
}

AStarSearch::Pos AStarSearch::advancePos(Pos pos, Dir dir) {
	if (dir == DIR_NE || dir == DIR_E || dir == DIR_SE)
		pos.x += 1;
	else if (dir == DIR_NW || dir == DIR_W || dir == DIR_SW)
		pos.x -= 1;
	
	if (dir == DIR_NW || dir == DIR_N || dir == DIR_NE)
		pos.y -= 1;
	else if (dir == DIR_SW || dir == DIR_S || dir == DIR_SE)
		pos.y += 1;
		
	return pos;
}

AStarSearch::AStarSearch(const WorldGrid &grid, const Pos &start, const Pos &end)
: width(grid.getWidth()), height(grid.getHeight()), squares(new Square[grid.getWidth()*grid.getHeight()]) {
	memset(squares.get(), '\0', sizeof(Square)*grid.getWidth()*grid.getHeight());

	typedef std::vector<Pos> PosList;
	PosList openlist;
	
	openlist.push_back(start);
	while (openlist.size() > 0) {
		Pos curpos = openlist.front();
		openlist.erase(openlist.begin());
		
		Square &cursquare = getSquare(curpos);
		cursquare.closed = true;
		
		if (curpos == end)
			break;
		
		for (Dir dir = DIR_E; dir <= DIR_SE; dir=(Dir)(dir+1)) {
			Pos pos = advancePos(curpos, dir);
			if (pos.x < 0 || pos.x >= width)
				continue;
			if (pos.y < 0 || pos.y >= height)
				continue;
			if (!grid.getPassable(pos.x, pos.y))
				continue;
					
			Square &square = getSquare(pos);
		
			if (square.closed)
				continue;
		
			int gscore = cursquare.gscore + (isDirDiagonal(dir) ? 14 : 10);
			if (grid(pos.x, pos.y) == WorldGrid::SMALL_OBSTACLE)
				gscore += 20;
			
			if (square.parentdir == DIR_NONE) {
				square.parentdir = oppositeDir(dir);
				square.gscore = gscore;
				square.hscore = abs(pos.x - end.x) + abs(pos.y - end.y);
				int fscore = square.gscore + square.hscore;
				
				PosList::iterator i;
				for (i = openlist.begin(); i != openlist.end(); ++i) {
					Square &othersquare = getSquare(*i);
					if (othersquare.gscore + othersquare.hscore > fscore)
						break;
				}
			
				openlist.insert(i, pos);
			} else if (square.gscore > gscore) {
				square.gscore = gscore;
				square.parentdir = oppositeDir(dir);
				int fscore = gscore + square.hscore;
				
				PosList::iterator i = find(openlist.begin(), openlist.end(), pos);
				openlist.erase(i);
				for (i = openlist.begin(); i != openlist.end(); ++i) {
					Square &othersquare = getSquare(*i);
					if (othersquare.gscore + othersquare.hscore > fscore)
						break;
				}
				
				openlist.insert(i, pos);
			}
		}
	}
	
	Pos curpos=end;
	while (curpos != start) {
		route.push_back(curpos);
		curpos = advancePos(curpos, getSquare(curpos).parentdir);
	}
	
	reverse(route.begin(), route.end());
}


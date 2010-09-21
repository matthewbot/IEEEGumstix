#include "RoomSimApp.h"
#include "SimFrame.h"
#include "VictimWorldObject.h"
#include "ObstacleWorldObject.h"
#include "AStarSearch.h"
#include <iostream>

using namespace pathsim;
using namespace std;

IMPLEMENT_APP(RoomSimApp)

RoomSimApp::RoomSimApp() : world(10, 10) {
	world.add(new VictimWorldObject(2, 2));
	world.add(new VictimWorldObject(9, 4));
	world.add(new VictimWorldObject(5, 9));
	world.add(new ObstacleWorldObject(5, 0, 7, 8, true));
	world.add(new ObstacleWorldObject(1, 6, 4, 7, false));
	
	AStarSearch::Pos start = {0, 0};
	AStarSearch::Pos end = {8, 1};
	AStarSearch search(world.getGrid(), start, end);
	for (AStarSearch::Route::const_iterator i = search.getRoute().begin(); i != search.getRoute().end(); ++i) {
		cout << "(" << i->x << "," << i->y << ")" << endl;
	}
}

bool RoomSimApp::OnInit() {
	frame = new SimFrame(world);
	frame->Show(true);
	SetTopWindow(frame);
	return true;
}

int RoomSimApp::OnExit() {
	delete frame;
}


#include "RoomSimApp.h"
#include "SimFrame.h"
#include "VictimWorldObject.h"
#include "ObstacleWorldObject.h"
#include "AStarSearch.h"
#include <iostream>

using namespace pathsim;
using namespace std;

IMPLEMENT_APP(RoomSimApp)

RoomSimApp::SimWorld::SimWorld()
: World(10, 10) {
	add(new ObstacleWorldObject(0, 3, 3, 3, false));
	add(new ObstacleWorldObject(2, 6, 3, 9, true));
	add(new ObstacleWorldObject(6, 3, 9, 2, true));
	add(new ObstacleWorldObject(7, 6, 6, 9, false));

	add(new VictimWorldObject(8, 1));
	add(new VictimWorldObject(1, 8));
	add(new VictimWorldObject(8, 8));
}

RoomSimApp::RoomSimApp()
: robot(2, Pos(0, 0), world.getGrid()) {
}

bool RoomSimApp::OnInit() {
	frame = new SimFrame(world, robot);
	frame->Show(true);
	SetTopWindow(frame);
	return true;
}


#include "ieeepath/sim/RoomSimApp.h"
#include "ieeepath/sim/SimFrame.h"
#include "ieeepath/shared/VictimWorldObject.h"
#include "ieeepath/shared/ObstacleWorldObject.h"
#include <iostream>

using namespace ieee;
using namespace std;

IMPLEMENT_APP(RoomSimApp)

RoomSimApp::SimWorld::SimWorld()
: World(10, 10) {
	add(new ObstacleWorldObject(Pos(0, 3), Pos(3, 3), true));
	add(new ObstacleWorldObject(Pos(2, 6), Pos(3, 9), true));
	add(new ObstacleWorldObject(Pos(6, 3), Pos(9, 2), true));
	add(new ObstacleWorldObject(Pos(7, 6), Pos(6, 9), false));

	add(new VictimWorldObject(Pos(8, 1)));
	add(new VictimWorldObject(Pos(1, 8)));
	add(new VictimWorldObject(Pos(8, 8)));
}

RoomSimApp::RoomSimApp()
: robot(Coord(1, 1), world.getGrid()) {
}

bool RoomSimApp::OnInit() {
	frame = new SimFrame(world, robot);
	frame->Show(true);
	SetTopWindow(frame);
	return true;
}


#include "ieee/sim/RoomSimApp.h"
#include "ieee/sim/RoomSimFrame.h"
#include <iostream>

using namespace ieee;
using namespace std;

IMPLEMENT_APP(RoomSimApp)

RoomSimApp::RoomSimApp() { }

bool RoomSimApp::OnInit() {
	frame = new RoomSimFrame();
	frame->Show(true);
	SetTopWindow(frame);
	return true;
}


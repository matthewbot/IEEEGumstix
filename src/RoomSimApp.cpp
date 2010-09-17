#include "RoomSimApp.h"
#include "SimFrame.h"

using namespace pathsim;

IMPLEMENT_APP(RoomSimApp)

RoomSimApp::RoomSimApp() : world(10, 10) { }

bool RoomSimApp::OnInit() {
	frame = new SimFrame();
	frame->Show(true);
	SetTopWindow(frame);
	return true;
}

int RoomSimApp::OnExit() {
	delete frame;
}


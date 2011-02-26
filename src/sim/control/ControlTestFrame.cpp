#include "ieee/sim/control/ControlTestFrame.h"
#include <iostream>

using namespace ieee;
using namespace boost;
using namespace std;

enum {
	SYNC_TIMER
};

BEGIN_EVENT_TABLE(ControlTestFrame, wxFrame)
	EVT_TIMER(SYNC_TIMER, ControlTestFrame::OnSyncEvent)
END_EVENT_TABLE()

ControlTestFrame::ControlTestFrame()
: wxFrame(NULL, -1, _("Control Test"), wxDefaultPosition, wxSize(320, 240)),
  poscontrol(poscontrolconfig),
  grid(10, 10),
  gridscale(.1, .1, -.5, -.5),
  panel(this),
  gridlayer(grid, gridscale),
  posconlayer(poscontrol),
  synctimer(this, SYNC_TIMER) {
	panel.addLayer(&gridlayer);
	panel.addLayer(&posconlayer);

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(sizer);
	sizer->Add(&panel, 1, wxEXPAND);

	synctimer.Start(40);

	try {
		robotptr.reset(new AVRRobot(robotconfig));
	} catch (std::exception &ex) {
		cerr << "Exception while creating AVRRobot: " << endl << ex.what() << endl;
		SetStatusText(_("Failed to create AVRRobot"));
	}
}

void ControlTestFrame::OnSyncEvent(wxTimerEvent &evt) {
	if (!robotptr)
		return;
	AVRRobot &robot = *robotptr;

	while (robot.syncIn()) { }
	poscontrol.update(robot);
	robot.syncOut();

	panel.Refresh();
}

ControlTestFrame::PositionControllerConfig::PositionControllerConfig() {
	posfilter.roomwidth = posfilter.roomheight = 100;

	driveequ.left.relpos.x = 3.1903;
	driveequ.left.relpos.y = 5.5257;
	driveequ.right.relpos.x = 3.1903;
	driveequ.right.relpos.y = -5.5257;
	driveequ.back.relpos.x = -6.3805;
	driveequ.back.relpos.y = 0;

	const float outscale = .1;
	const float outoffset = .5;
	driveequ.left.outscale = outscale;
	driveequ.left.outoffset = outoffset;
	driveequ.right.outscale = outscale;
	driveequ.right.outoffset = outoffset;
	driveequ.back.outscale = outscale;
	driveequ.back.outoffset = outoffset;

	driveequ.rotationoffset = M_PI/2;
	driveequ.minspeed = .1;
}

ControlTestFrame::AVRRobotConfig::AVRRobotConfig() {
	wheels.left.minstop = wheels.right.minstop = wheels.back.minstop = 0;
	wheels.left.maxstop = wheels.right.maxstop = wheels.back.maxstop = 1800;
	wheels.left.offset = wheels.right.offset = wheels.back.offset = 0;

	sonar1.alpha = 1;
	sonar1.beta = 0;
	sonar2.alpha = 1;
	sonar2.beta = 0;

	compass.centerx = 825;
	compass.centery = 1200;
	compass.yscale = .985;
}


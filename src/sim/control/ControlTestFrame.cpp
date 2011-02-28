#include "ieee/sim/control/ControlTestFrame.h"
#include <boost/assign/std/vector.hpp>
#include <iostream>

using namespace ieee;
using namespace boost;
using namespace boost::assign;
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
  panel(this, 100, 100),
  gridlayer(grid, gridscale),
  posconlayer(*this, poscontrol),
  optionspanel(this, -1),
  drivecheck(&optionspanel, -1, _("Drive")),
  synctimer(this, SYNC_TIMER) {
	panel.addLayer(&gridlayer);
	panel.addLayer(&posconlayer);

	wxBoxSizer *optionspanel_sizer = new wxBoxSizer(wxHORIZONTAL);
	optionspanel.SetSizer(optionspanel_sizer);
	optionspanel_sizer->Add(&drivecheck, 0, wxEXPAND);

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(sizer);
	sizer->Add(&panel, 1, wxEXPAND);
	sizer->Add(&optionspanel, 0, wxEXPAND);

	synctimer.Start(40);

	try {
		robotptr.reset(new AVRRobot(robotconfig));

		robotptr->syncIOWait(20);
		robotptr->calibrateCompassOffset();
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

	if (!drivecheck.GetValue()) {
		GumstixPacket &gp = robot.getGumstixPacket();
		gp.leftwheel_effort = gp.rightwheel_effort = gp.backwheel_effort = 0;
	}

	robot.syncOut();

	panel.Refresh();
}

void ControlTestFrame::onCommand(const Coord &coord, float dir) {
	PositionController::Command command;
	command.destpos = coord;
	command.speed = .2;
	poscontrol.setCommand(command);
}

ControlTestFrame::PositionControllerConfig::PositionControllerConfig() {
	posfilter.roomwidth = posfilter.roomheight = 100;
	posfilter.sonaroffset = Vec2D(4, 0);
	posfilter.sonarstepperrad = 2.5f;

	driveequ.left.relpos.x = 3.1903;
	driveequ.left.relpos.y = 5.5257;
	driveequ.right.relpos.x = 3.1903;
	driveequ.right.relpos.y = -5.5257;
	driveequ.back.relpos.x = -6.3805;
	driveequ.back.relpos.y = 0;

	const float outscale = .1;
	const float outoffset = .7;
	driveequ.left.outscale = outscale;
	driveequ.left.outoffset = outoffset;
	driveequ.right.outscale = outscale;
	driveequ.right.outoffset = outoffset;
	driveequ.back.outscale = outscale;
	driveequ.back.outoffset = outoffset;

	driveequ.rotationoffset = M_PI/2;
	driveequ.minspeed = .1;

	lockdist = 20;
	lockangdiff = M_PI/4;
	stopdist = 3;
}

ControlTestFrame::AVRRobotConfig::AVRRobotConfig() {
	wheels.left.minstop = wheels.right.minstop = wheels.back.minstop = 0;
	wheels.left.maxstop = wheels.right.maxstop = wheels.back.maxstop = 1800;
	wheels.left.offset = 40;
	wheels.right.offset = 0;
	wheels.back.offset = 70;

	sonar1.alpha = .3078;
	sonar1.beta = -53.11357 + 3.5;
	sonar2.alpha = .26518;
	sonar2.beta = -50.10155 + 3.5;

	compass.centerx = 198.68;
	compass.centery = 47.52;
	compass.yscale = 1.0631;
	compass.leftwheel_offset.magx += -0.97367, -1.01313, 21.84237, -13.18883, -21.30770, -1.84136;
	compass.leftwheel_offset.magy += -7.8588, 57.2176, -129.1713, 111.0248, -81.0347, 1.0370;
	compass.rightwheel_offset.magx += -2.3619, 18.1477, -47.1516, 54.6400, -11.5349, -2.3130;
	compass.rightwheel_offset.magy += -2.1977, 20.6271, -65.0001, 88.1855, -72.0203, 8.2104;
	compass.backwheel_offset.magx += -1.91791, 12.60694, -24.89343, 16.92729, -12.33935, -0.35387;
	compass.backwheel_offset.magy += -0.423544, 5.234523, -18.306325, 19.816709, -7.516840, -0.073927;
}


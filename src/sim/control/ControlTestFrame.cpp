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
	command.speed = 12;
	poscontrol.setCommand(command);
}

ControlTestFrame::PositionControllerConfig::PositionControllerConfig() {
	posfilter.roomwidth = posfilter.roomheight = 100;
	posfilter.sonaroffset = Vec2D(4, 0);
	posfilter.sonarstepperrad = 2.5f;
	posfilter.sonarmindist = 30;

	driveequ.left.relpos.x = 3.1903;
	driveequ.left.relpos.y = 5.5257;
	driveequ.right.relpos.x = 3.1903;
	driveequ.right.relpos.y = -5.5257;
	driveequ.back.relpos.x = -6.3805;
	driveequ.back.relpos.y = 0;

	const float outscale = .0108;
	const float outoffset = .6;
	driveequ.left.outscale = outscale;
	driveequ.left.outoffset = outoffset;
	driveequ.right.outscale = outscale;
	driveequ.right.outoffset = outoffset - .15;
	driveequ.back.outscale = outscale;
	driveequ.back.outoffset = outoffset;

	driveequ.rotationoffset = M_PI/2;
	driveequ.minspeed = .1;

	lockdist = 30;
	lockangdiff = M_PI/4;
	stopdist = 5;
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

	compass.centerx = 255;
	compass.centery = 11.42;
	compass.yscale = 1.0013;
	compass.leftwheel_offset.magx += 1.105413, -17.766327, 67.379417, -61.844683, -0.033371, -2.222486;
	compass.leftwheel_offset.magy += -9.2314, 65.9709, -145.6980, 118.5804, -74.1991, 2.2407;
	compass.rightwheel_offset.magx += -0.68205, 5.15608, -11.20517, 10.77148, 12.03845, -4.65361;
	compass.rightwheel_offset.magy += 0.087533, 2.294021, -10.848735, 16.962901, -38.659120, 8.029173;
	compass.backwheel_offset.magx += -0.51772, 4.10939, -8.35313, 3.73830, -9.84221, 0.27012;
	compass.backwheel_offset.magy += 0.46597, -2.08954, 2.30283, -1.12453, -11.01647, 0.65481;


	stepper.wrapangle = M_PI/2;
	stepper.stepsize = 1.8/180*M_PI;
}


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

	stopdist = 10;
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

	compass.centerx = 204.26;
	compass.centery = 92.898;
	compass.yscale = 1.0037;
	compass.leftwheel_offset.magx += -0.35999, 6.58014, -49.68741, 206.44643, -523.28899, 824.64117, -766.87722, 397.08822, -114.75798, 3.22681;
	compass.leftwheel_offset.magy += -3.0764, 46.1999, -288.8646, 971.4253, -1899.0372, 2185.6556, -1425.3537, 473.9252, -116.4624, 2.2518;
	compass.rightwheel_offset.magx += -1.4085, 22.0113, -146.5448, 540.8970, -1205.7032, 1653.9614, -1353.2702, 604.1224, -101.1374, 1.7001;
	compass.rightwheel_offset.magy += 3.1626, -47.9573, 306.5287, -1072.9989, 2236.3373, -2818.2724, 2073.0765, -805.0540, 102.8950, -1.1871;
	compass.backwheel_offset.magx += -0.94414, 12.30830, -64.99283, 175.33200, -244.83722, 140.21278, 31.50476, -64.51520, 2.68270, -0.20292;
	compass.backwheel_offset.magy += 0.74264, -9.29949, 44.60206, -94.78007, 48.08379, 154.88016, -288.62041, 176.29711, -35.34805, 2.52520;
}


#include "ieee/sim/control/ControlTestFrame.h"
#include <boost/assign/std/vector.hpp>
#include <iostream>

using namespace ieee;
using namespace boost;
using namespace boost::assign;
using namespace std;

enum {
	SYNC_TIMER,
	STOP_BUTTON,
	RESET_BUTTON
};

BEGIN_EVENT_TABLE(ControlTestFrame, wxFrame)
	EVT_BUTTON(STOP_BUTTON, ControlTestFrame::OnStopEvent)
	EVT_BUTTON(RESET_BUTTON, ControlTestFrame::OnResetEvent)
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
  stopbutton(&optionspanel, STOP_BUTTON, _("Stop"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT),
  resetbutton(&optionspanel, RESET_BUTTON, _("Reset"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT),
  synctimer(this, SYNC_TIMER) {
	panel.addLayer(&gridlayer);
	panel.addLayer(&posconlayer);

	wxBoxSizer *optionspanel_sizer = new wxBoxSizer(wxHORIZONTAL);
	optionspanel.SetSizer(optionspanel_sizer);
	optionspanel_sizer->Add(&stopbutton, 0, wxEXPAND);
	optionspanel_sizer->Add(&resetbutton, 0, wxEXPAND);

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(sizer);
	sizer->Add(&panel, 1, wxEXPAND);
	sizer->Add(&optionspanel, 0, wxEXPAND);

	synctimer.Start(40);

	try {
		robotptr.reset(new AVRRobot(robotconfig));

		robotptr->setWheelAngles(M_PI/2, M_PI/2, M_PI/2);
		robotptr->syncIOWait(20);
		robotptr->calibrateCompassOffset();
	} catch (std::exception &ex) {
		cerr << "Exception while creating AVRRobot: " << endl << ex.what() << endl;
		SetStatusText(_("Failed to create AVRRobot"));
	}
}

void ControlTestFrame::OnStopEvent(wxCommandEvent &evt) {
	poscontrol.stop();
	panel.Refresh();
}

void ControlTestFrame::OnResetEvent(wxCommandEvent &evt) {
	if (!robotptr)
		return;
	robotptr->calibrateCompassOffset();
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

void ControlTestFrame::onCommand(const Coord &coord, float dir) {
	PositionController::Command command;
	command.destpos = coord;
	command.destheading = Angle(dir);
	command.speed = 14;
	poscontrol.setCommand(command);
}

ControlTestFrame::PositionControllerConfig::PositionControllerConfig() {
	posfilter.roomwidth = posfilter.roomheight = 100;
	posfilter.sonaroffset = Vec2D(4, 0);
	posfilter.sonarstepperrad = 2.5f;
	posfilter.sonarmindist = 30;
	posfilter.posbufsize = 3;

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

	driveequ.rotationoffset = Angle(M_PI/2);
	driveequ.minspeed = .1;

	lockdist = 15;
	lockangdiff = M_PI/4;
	lockspeed = 10;
	stopdist = 3;

	angvelfactor = 1;
	maxangvel = 1;

	maxturndist = .1;
	turnspeed = 1;
}

ControlTestFrame::AVRRobotConfig::AVRRobotConfig() {
	wheels.left.minstop = wheels.right.minstop = wheels.back.minstop = 0;
	wheels.left.maxstop = wheels.right.maxstop = wheels.back.maxstop = 1800;
	wheels.left.offset = 40;
	wheels.right.offset = 0;
	wheels.back.offset = 70;
	wheels.turnhysteresis = 150;

	sonar1.alpha = .3078;
	sonar1.beta = -53.11357 + 3.5;
	sonar2.alpha = .26518;
	sonar2.beta = -50.10155 + 3.5;

	compass.centerx = -126.16;
	compass.centery = 242.57;
	compass.yscale = .9568;
	compass.leftwheel_offset.magx += -0.72880, 1.03260, 28.29561, -140.05029, 238.21493, -7.61922;
	compass.leftwheel_offset.magy += -6.7594, 57.4896, -178.8064, 241.6305, -103.1244, -3.5440;
	compass.rightwheel_offset.magx += 1.0366, -8.7516, 25.4013, -27.4524, 15.4142, 1.2577;
	compass.rightwheel_offset.magy += -3.3286, 29.0120, -95.1656, 149.4331, -117.6783, -4.2431;
	compass.backwheel_offset.magx += -0.73648, 8.29814, -27.30958, 30.65160, -28.18682, 2.86296;
	compass.backwheel_offset.magy += -0.70298, 0.22097, 13.25488, -24.32732, -10.30858, 2.56629;

	stepper.stepsize = 1.5/180*M_PI;
}


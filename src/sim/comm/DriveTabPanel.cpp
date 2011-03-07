#include "ieee/sim/comm/DriveTabPanel.h"

using namespace ieee;

DriveTabPanel::DriveTabPanel(wxWindow *parent, const DriveEquation::Config &driveequconfig)
: TabPanel(parent),
  xvellabel(this, -1, _("X Velocity")),
  xvelspin(this, -1, _("0")),
  yvellabel(this, -1, _("Y Velocity")),
  yvelspin(this, -1, _("0")),
  curdirlabel(this, -1, _("Current Dir")),
  curdirspin(this, -1, _("0")),
  angvellabel(this, -1, _("Angular Vel")),
  angvelspin(this, -1, _("0")),
  maxeffortlabel(this, -1, _("Max Effort")),
  maxeffortspin(this, -1, _("0")),
  enablecheck(this, -1, _("Enable")),
  compasscheck(this, -1, _("Compass")),
  driveequ(driveequconfig) {
	wxFlexGridSizer *sizer = new wxFlexGridSizer(2, 5);
	SetSizer(sizer);
	sizer->Add(&xvellabel, 0, wxEXPAND);
	sizer->Add(&xvelspin, 0, wxEXPAND);
	sizer->Add(&yvellabel, 0, wxEXPAND);
	sizer->Add(&yvelspin, 0, wxEXPAND);
	sizer->Add(&curdirlabel, 0, wxEXPAND);
	sizer->Add(&curdirspin, 0, wxEXPAND);
	sizer->Add(&angvellabel, 0, wxEXPAND);
	sizer->Add(&angvelspin, 0, wxEXPAND);
	sizer->Add(&maxeffortlabel, 0, wxEXPAND);
	sizer->Add(&maxeffortspin, 0, wxEXPAND);
	sizer->Add(&enablecheck, 0, wxEXPAND);
	sizer->Add(&compasscheck, 0, wxEXPAND);

	angvelspin.SetRange(-100, 100);
}

char DriveTabPanel::getTabCharacter() const { return 'D'; }

void DriveTabPanel::onSync(AVRRobot &robot) {
	if (!enablecheck.GetValue())
		return;

	if (compasscheck.GetValue()) {
		float angle = robot.getCompassAngle()/M_PI*180;
		curdirspin.SetValue((int)angle);
	}

	DriveEquation::Motion motion;
	motion.vel.x = xvelspin.GetValue();
	motion.vel.y = yvelspin.GetValue();
	motion.curdir = Angle(curdirspin.GetValue()/180.0*M_PI);
	motion.angvel = angvelspin.GetValue()/180.0*M_PI;

	robot.setWheels(driveequ.compute(motion));
}


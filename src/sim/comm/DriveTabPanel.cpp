#include "ieee/sim/comm/DriveTabPanel.h"

using namespace ieee;

DriveTabPanel::DriveTabPanel(wxWindow *parent)
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
  driveequ(equconf) {
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
	motion.curangle = curdirspin.GetValue()/180.0*M_PI;
	motion.angvel = angvelspin.GetValue()/180.0*M_PI;

	WheelsControl::Output out = driveequ.compute(motion);
	robot.setWheels(out);
}

DriveTabPanel::DriveEquationConfig::DriveEquationConfig() {
	left.relpos.x = 3.1903;
	left.relpos.y = 5.5257;
	right.relpos.x = 3.1903;
	right.relpos.y = -5.5257;
	back.relpos.x = -6.3805;
	back.relpos.y = 0;

	const float outscale = .1;
	const float outoffset = .5;

	left.outscale = outscale;
	left.outoffset = outoffset;
	right.outscale = outscale;
	right.outoffset = outoffset;
	back.outscale = outscale;
	back.outoffset = outoffset;

	rotationoffset = M_PI/2;
	minspeed = .1;
}


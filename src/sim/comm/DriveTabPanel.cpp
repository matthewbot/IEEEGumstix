#include "ieee/sim/comm/DriveTabPanel.h"

using namespace ieee;

enum {
	XVEL_SPIN,
	YVEL_SPIN,
	CURDIR_SPIN,
	ANGVEL_SPIN,
	MAXEFFORT_SPIN,
	ENABLE_CHECK
};

BEGIN_EVENT_TABLE(DriveTabPanel, wxPanel)
	EVT_SPINCTRL(XVEL_SPIN, DriveTabPanel::OnSpin)
	EVT_TEXT_ENTER(XVEL_SPIN, DriveTabPanel::OnCommand)
	EVT_SPINCTRL(YVEL_SPIN, DriveTabPanel::OnSpin)
	EVT_TEXT_ENTER(YVEL_SPIN, DriveTabPanel::OnCommand)
	EVT_SPINCTRL(CURDIR_SPIN, DriveTabPanel::OnSpin)
	EVT_TEXT_ENTER(CURDIR_SPIN, DriveTabPanel::OnCommand)
	EVT_SPINCTRL(ANGVEL_SPIN, DriveTabPanel::OnSpin)
	EVT_TEXT_ENTER(ANGVEL_SPIN, DriveTabPanel::OnCommand)
	EVT_SPINCTRL(MAXEFFORT_SPIN, DriveTabPanel::OnSpin)
	EVT_TEXT_ENTER(MAXEFFORT_SPIN, DriveTabPanel::OnCommand)
	EVT_CHECKBOX(ENABLE_CHECK, DriveTabPanel::OnCommand)
END_EVENT_TABLE()

DriveTabPanel::DriveTabPanel(wxWindow *parent, Callbacks &callbacks)
: wxPanel(parent, -1),
  xvellabel(this, -1, _("X Velocity")),
  xvelspin(this, XVEL_SPIN, _("0")),
  yvellabel(this, -1, _("Y Velocity")),
  yvelspin(this, YVEL_SPIN, _("0")),
  curdirlabel(this, -1, _("Current Dir")),
  curdirspin(this, CURDIR_SPIN, _("0")),
  angvellabel(this, -1, _("Angular Vel")),
  angvelspin(this, ANGVEL_SPIN, _("0")),
  maxeffortlabel(this, -1, _("Max Effort")),
  maxeffortspin(this, MAXEFFORT_SPIN, _("0")),
  enablecheck(this, ENABLE_CHECK, _("Enable")),
  driveequ(equconf),
  callbacks(callbacks) {
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

void DriveTabPanel::writeOutput(GumstixPacket &gp) const {
	DriveEquation::Motion motion;
	motion.vel.x = xvelspin.GetValue();
	motion.vel.y = yvelspin.GetValue();
	motion.curangle = curdirspin.GetValue()/180.0*M_PI;
	motion.angvel = angvelspin.GetValue()/180.0*M_PI;

	WheelsDriver::Output out = driveequ.compute(motion);

	gp.leftwheel_angle = toRawAngle(out.left.angle);
	gp.rightwheel_angle = toRawAngle(out.right.angle);
	gp.backwheel_angle = toRawAngle(out.back.angle);

	if (enablecheck.GetValue()) {
		gp.leftwheel_speed = toRawSpeed(out.left.effort, out.left.angle);
		gp.rightwheel_speed = toRawSpeed(out.right.effort, out.right.angle);
		gp.backwheel_speed = toRawSpeed(out.back.effort, out.back.angle);
	} else {
		gp.leftwheel_speed = 0;
		gp.rightwheel_speed = 0;
		gp.backwheel_speed = 0;
	}
}

void DriveTabPanel::OnSpin(wxSpinEvent &evt) {
	callbacks.onOutputChanged();
}

void DriveTabPanel::OnCommand(wxCommandEvent &evt) {
	callbacks.onOutputChanged();
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

int16_t DriveTabPanel::toRawAngle(float angle) {
	int val = (int)(angle/M_PI*1800);
	if (val < 0)
		val += 1800;
	else if (val > 1800)
		val -= 1800;
	return (int16_t)val;
}

int16_t DriveTabPanel::toRawSpeed(float speed, float angle) {
	if (angle < 0 || angle > M_PI)
		speed = -speed;
	return (int16_t)(speed*1000);
}

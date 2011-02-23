#include "ieee/sim/comm/WheelTabPanel.h"
#include <wx/gbsizer.h>

using namespace ieee;

enum {
	ENABLED_CHECK,
	SYNC_SPEED_CHECK,
	SYNC_ANGLE_CHECK,
	REVERSE_CHECK,
	RAISE_CHECK
};

BEGIN_EVENT_TABLE(WheelTabPanel, wxPanel)
	EVT_CHECKBOX(ENABLED_CHECK, WheelTabPanel::OnCheckEvent)
	EVT_CHECKBOX(SYNC_SPEED_CHECK, WheelTabPanel::OnCheckEvent)
	EVT_CHECKBOX(SYNC_ANGLE_CHECK, WheelTabPanel::OnCheckEvent)
	EVT_CHECKBOX(REVERSE_CHECK, WheelTabPanel::OnCheckEvent)
	EVT_CHECKBOX(RAISE_CHECK, WheelTabPanel::OnCheckEvent)
END_EVENT_TABLE()

WheelTabPanel::WheelTabPanel(wxWindow *parent, TabPanel::Callbacks &callbacks)
: TabPanel(parent),
  callbacks(callbacks),
  leftwidget(this, *this),
  rightwidget(this, *this),
  bottomwidget(this, *this),
  centerpanel(this, -1),
  enabledcheck(&centerpanel, ENABLED_CHECK, _("Enabled")),
  reversecheck(&centerpanel, REVERSE_CHECK, _("Reverse")),
  syncanglecheck(&centerpanel, SYNC_ANGLE_CHECK, _("S Angle")),
  syncspeedcheck(&centerpanel, SYNC_SPEED_CHECK, _("S Speed")),
  raisecheck(&centerpanel, RAISE_CHECK, _("Raise")) {
	wxBoxSizer *centerpanel_sizer = new wxBoxSizer(wxVERTICAL);
	centerpanel.SetSizer(centerpanel_sizer);
	centerpanel_sizer->Add(&enabledcheck, 0, wxEXPAND);
	centerpanel_sizer->Add(&reversecheck, 0, wxEXPAND);
	centerpanel_sizer->Add(&syncanglecheck, 0, wxEXPAND);
	centerpanel_sizer->Add(&syncspeedcheck, 0, wxEXPAND);
	centerpanel_sizer->Add(&raisecheck, 0, wxEXPAND);

	wxGridBagSizer *sizer = new wxGridBagSizer();
	SetSizer(sizer);
	sizer->Add(&leftwidget, wxGBPosition(0, 0), wxDefaultSpan, wxEXPAND);
	sizer->Add(&rightwidget, wxGBPosition(0, 2), wxDefaultSpan, wxEXPAND);
	sizer->Add(&centerpanel, wxGBPosition(0, 1), wxDefaultSpan, wxEXPAND);
	sizer->Add(&bottomwidget, wxGBPosition(1, 1), wxDefaultSpan, wxEXPAND);
	sizer->AddGrowableRow(1, 1);
	for (int i=0; i<3; i++)
		sizer->AddGrowableCol(i, 1);
}

char WheelTabPanel::getTabCharacter() const { return 'W'; }

void WheelTabPanel::updateGumstixPacket(GumstixPacket &gp, const WheelsControl &wheelscontrol) const {
	if (!enabledcheck.GetValue())
		return;

	WheelsControl::Output output;
	output.left.angle = leftwidget.getDirection();
	output.right.angle = rightwidget.getDirection();
	output.back.angle = bottomwidget.getDirection();
	output.left.effort = leftwidget.getSpeed();
	output.right.effort = rightwidget.getSpeed();
	output.back.effort = bottomwidget.getSpeed();

	wheelscontrol.writeOutput(output, gp);

	gp.retract = raisecheck.GetValue();
}

void WheelTabPanel::onNewAVRPacket(const AVRPacket &ap) { }

void WheelTabPanel::update() {
	if (syncanglecheck.GetValue()) {
		float dir = bottomwidget.getDirection();
		leftwidget.setDirection(dir);
		rightwidget.setDirection(dir);
	}

	if (syncspeedcheck.GetValue()) {
		float speed = bottomwidget.getSpeed();
		leftwidget.setSpeed(speed);
		rightwidget.setSpeed(speed);
	}

	callbacks.onTabUpdated(this);
}

void WheelTabPanel::onWheelChanged(WheelWidget *widget) {
	update();
}

void WheelTabPanel::OnCheckEvent(wxCommandEvent &) {
	update();
}

int16_t WheelTabPanel::toRawAngle(float angle) {
	int val = (int)(angle/M_PI*1800);
	if (val < 0)
		val += 1800;
	return (int16_t)val;
}

int16_t WheelTabPanel::toRawSpeed(float speed, float angle) {
	if (angle < 0)
		speed = -speed;
	return (int16_t)(speed*1000);
}


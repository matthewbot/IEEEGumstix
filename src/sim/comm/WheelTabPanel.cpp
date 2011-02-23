#include "ieee/sim/comm/WheelTabPanel.h"
#include <wx/gbsizer.h>

using namespace ieee;

enum {
	SYNCANGLE_CHECK,
	SYNCSPEED_CHECK
};

BEGIN_EVENT_TABLE(WheelTabPanel, wxPanel)
	EVT_CHECKBOX(SYNCANGLE_CHECK, WheelTabPanel::OnCheckEvent)
	EVT_CHECKBOX(SYNCSPEED_CHECK, WheelTabPanel::OnCheckEvent)
END_EVENT_TABLE()

WheelTabPanel::WheelTabPanel(wxWindow *parent)
: TabPanel(parent),
  leftwidget(this, *this),
  rightwidget(this, *this),
  bottomwidget(this, *this),
  centerpanel(this, -1),
  enabledcheck(&centerpanel, -1, _("Enabled")),
  reversecheck(&centerpanel, -1, _("Reverse")),
  syncanglecheck(&centerpanel, SYNCANGLE_CHECK, _("S Angle")),
  syncspeedcheck(&centerpanel, SYNCSPEED_CHECK, _("S Speed")),
  raisecheck(&centerpanel, -1, _("Raise")) {
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

void WheelTabPanel::onSync(AVRRobot &robot) {
	if (!enabledcheck.GetValue())
		return;

	handleSyncChecks();

	WheelsControl::Output output;
	output.left.angle = leftwidget.getDirection();
	output.right.angle = rightwidget.getDirection();
	output.back.angle = bottomwidget.getDirection();
	output.left.effort = leftwidget.getSpeed();
	output.right.effort = rightwidget.getSpeed();
	output.back.effort = bottomwidget.getSpeed();
	robot.setWheels(output);

	robot.setUpDown(raisecheck.GetValue());
}

void WheelTabPanel::onWheelChanged(WheelWidget *widget) {
	handleSyncChecks();
}

void WheelTabPanel::OnCheckEvent(wxCommandEvent &) {
	handleSyncChecks();
}

void WheelTabPanel::handleSyncChecks() {
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
}


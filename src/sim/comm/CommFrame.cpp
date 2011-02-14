#include "ieee/sim/comm/CommFrame.h"
#include <wx/gbsizer.h>
#include <iostream>

using namespace ieee;
using namespace std;

enum {
	SYNC_EVENT,
	ENABLED_CHECK,
	SYNC_SPEED_CHECK,
	SYNC_ANGLE_CHECK,
	REVERSE_CHECK
};

BEGIN_EVENT_TABLE(CommFrame, wxFrame)
	EVT_MENU(SYNC_EVENT, CommFrame::OnSyncEvent)
	EVT_CHECKBOX(ENABLED_CHECK, CommFrame::OnCheckEvent)
	EVT_CHECKBOX(SYNC_SPEED_CHECK, CommFrame::OnCheckEvent)
	EVT_CHECKBOX(SYNC_ANGLE_CHECK, CommFrame::OnCheckEvent)
	EVT_CHECKBOX(REVERSE_CHECK, CommFrame::OnCheckEvent)
END_EVENT_TABLE()

CommFrame::CommFrame()
: wxFrame(NULL, -1, _("IEEE Comm"), wxDefaultPosition, wxSize(320, 240)),
  leftwidget(this, *this),
  rightwidget(this, *this),
  bottomwidget(this, *this),
  centerpanel(this, -1),
  enabledcheck(&centerpanel, ENABLED_CHECK, _("Enabled")),
  syncanglecheck(&centerpanel, SYNC_ANGLE_CHECK, _("Sync Angle")),
  syncspeedcheck(&centerpanel, SYNC_SPEED_CHECK, _("Sync Speed")),
  reversecheck(&centerpanel, REVERSE_CHECK, _("Reverse")),
  thread(*this) {
	wxBoxSizer *centerpanel_sizer = new wxBoxSizer(wxVERTICAL);
	centerpanel.SetSizer(centerpanel_sizer);
	centerpanel_sizer->Add(&enabledcheck, 0, wxEXPAND);
	centerpanel_sizer->Add(&syncanglecheck, 0, wxEXPAND);
	centerpanel_sizer->Add(&syncspeedcheck, 0, wxEXPAND);
	centerpanel_sizer->Add(&reversecheck, 0, wxEXPAND);

	wxGridBagSizer *sizer = new wxGridBagSizer();
	SetSizer(sizer);
	sizer->Add(&leftwidget, wxGBPosition(0, 0), wxDefaultSpan, wxEXPAND);
	sizer->Add(&rightwidget, wxGBPosition(0, 2), wxDefaultSpan, wxEXPAND);
	sizer->Add(&centerpanel, wxGBPosition(0, 1), wxGBSpan(2, 1), wxEXPAND);
	sizer->Add(&bottomwidget, wxGBPosition(2, 1), wxDefaultSpan, wxEXPAND);
	sizer->AddGrowableRow(0, 1);
	sizer->AddGrowableRow(2, 1);
	for (int i=0; i<3; i++)
		sizer->AddGrowableCol(i, 1);

	CreateStatusBar();

	updatePacket();
	thread.start();
}

void CommFrame::OnSyncEvent(wxCommandEvent &) {
	SetStatusText(_("Debug output: ") + wxString::FromAscii(thread.getAVRPacket().debugoutput));
}

void CommFrame::onSync() {
	// this is the best thing I can find short of defining my own event class (assuming thats possible?)
	wxCommandEvent event(wxEVT_COMMAND_MENU_SELECTED, SYNC_EVENT);
	AddPendingEvent(event);
}

void CommFrame::onWheelChanged(WheelWidget *widget) {
	updatePacket();
}

void CommFrame::OnCheckEvent(wxCommandEvent &) {
	updatePacket();
}

void CommFrame::updatePacket() {
	GumstixPacket gp;
	memset(&gp, 0, sizeof(GumstixPacket));

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

	gp.leftwheel_angle = toRawAngle(leftwidget.getDirection());
	gp.rightwheel_angle = toRawAngle(rightwidget.getDirection());
	gp.backwheel_angle = toRawAngle(bottomwidget.getDirection());

	if (enabledcheck.GetValue()) {
		gp.leftwheel_speed = toRawSpeed(leftwidget.getSpeed(), leftwidget.getDirection());
		gp.rightwheel_speed = toRawSpeed(rightwidget.getSpeed(), rightwidget.getDirection());
		gp.backwheel_speed = toRawSpeed(bottomwidget.getSpeed(), bottomwidget.getDirection());
	}

	if (reversecheck.GetValue()) {
		gp.leftwheel_speed = -gp.leftwheel_speed;
		gp.rightwheel_speed = -gp.rightwheel_speed;
		gp.backwheel_speed = -gp.backwheel_speed;
	}

	thread.setGumstixPacket(gp);
}

int16_t CommFrame::toRawAngle(float angle) {
	int val = (int)(angle/M_PI*1800);
	if (val < 0)
		val += 1800;
	return (int16_t)val;
}

int16_t CommFrame::toRawSpeed(float speed, float angle) {
	if (angle < 0)
		speed = -speed;
	return (int16_t)(speed*1000);
}



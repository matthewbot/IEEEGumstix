#include "ieee/sim/comm/CommFrame.h"

using namespace ieee;
using namespace std;

enum {
	SYNC_EVENT
};

BEGIN_EVENT_TABLE(CommFrame, wxFrame)
	EVT_MENU(SYNC_EVENT, CommFrame::OnSyncEvent)
END_EVENT_TABLE()

CommFrame::CommFrame()
: wxFrame(NULL, -1, _("IEEE Comm"), wxDefaultPosition, wxSize(320, 240)),
  notebook(this, -1, wxDefaultPosition, wxDefaultSize, wxNB_RIGHT),
  wheelscontrol(wheelscontrolconf),
  thread(*this) {
	panels.push_back(new WheelTabPanel(&notebook, *this));
	panels.push_back(new SensorsTabPanel(&notebook, *this));
	panels.push_back(new DriveTabPanel(&notebook, *this));

	for (TabPanelVec::iterator i = panels.begin(); i != panels.end(); ++i)
		notebook.AddPage(*i, wxString((wxChar)(*i)->getTabCharacter()));

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(&notebook, 1, wxEXPAND);
	SetSizer(sizer);

	CreateStatusBar();

	updatePacket();
	thread.start();
}

void CommFrame::OnSyncEvent(wxCommandEvent &) {
	const AVRPacket &ap = thread.getAVRPacket();
	SetStatusText(wxString::FromAscii(ap.debugoutput));

	for (TabPanelVec::iterator i = panels.begin(); i != panels.end(); ++i)
		(*i)->onNewAVRPacket(ap);
}

void CommFrame::onSync() {
	// this is the best thing I can find short of defining my own event class (assuming thats possible?)
	wxCommandEvent event(wxEVT_COMMAND_MENU_SELECTED, SYNC_EVENT);
	AddPendingEvent(event);
}

void CommFrame::onTabUpdated(TabPanel *tp) {
	updatePacket();
}

void CommFrame::updatePacket() {
	GumstixPacket gp;
	memset(&gp, 0, sizeof(GumstixPacket));

	gp.leftwheel_angle = gp.rightwheel_angle = gp.backwheel_angle = 900;

	for (TabPanelVec::iterator i = panels.begin(); i != panels.end(); ++i)
		(*i)->updateGumstixPacket(gp, wheelscontrol);

	thread.setGumstixPacket(gp);
}

CommFrame::WheelsControlConfig::WheelsControlConfig() {
	left.minstop = right.minstop = back.minstop = 100;
	left.maxstop = right.maxstop = back.maxstop = 1500;
	left.offset = right.offset = back.offset = 0;
}


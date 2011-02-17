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
  wheelpanel(new WheelTabPanel(&notebook, *this)),
  sensorspanel(new SensorsTabPanel(&notebook, *this)),
  thread(*this) {
	notebook.AddPage(wheelpanel, _("W"));
	notebook.AddPage(sensorspanel, _("S"));

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

	sensorspanel->readSensorData(ap);
}

void CommFrame::onSync() {
	// this is the best thing I can find short of defining my own event class (assuming thats possible?)
	wxCommandEvent event(wxEVT_COMMAND_MENU_SELECTED, SYNC_EVENT);
	AddPendingEvent(event);
}

void CommFrame::onWheelsMoved() {
	updatePacket();
}

void CommFrame::onSonarAngleChanged() {
	updatePacket();
}

void CommFrame::updatePacket() {
	GumstixPacket gp;
	memset(&gp, 0, sizeof(GumstixPacket));

	wheelpanel->writeWheelStates(gp);
	sensorspanel->writeSonarAngle(gp);

	thread.setGumstixPacket(gp);
}




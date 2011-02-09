#include "ieee/sim/comm/CommFrame.h"
#include <wx/gbsizer.h>

using namespace ieee;
using namespace std;

enum {
	SYNC_EVENT
};

BEGIN_EVENT_TABLE(CommFrame, wxFrame)
	EVT_MENU(SYNC_EVENT, CommFrame::OnSyncEvent)
END_EVENT_TABLE()

CommFrame::CommFrame()
: wxFrame(NULL, -1, _("IEEE Comm"), wxDefaultPosition, wxSize(450, 400)),
  leftwidget(this),
  rightwidget(this),
  bottomwidget(this),
  thread(*this) {
	wxGridBagSizer *sizer = new wxGridBagSizer();
	SetSizer(sizer);
	sizer->Add(&leftwidget, wxGBPosition(0, 0), wxDefaultSpan, wxEXPAND);
	sizer->Add(&rightwidget, wxGBPosition(0, 2), wxDefaultSpan, wxEXPAND);
	sizer->Add(&bottomwidget, wxGBPosition(1, 1), wxDefaultSpan, wxEXPAND);
	sizer->AddGrowableRow(0);
	sizer->AddGrowableRow(1);
	for (int i=0; i<3; i++)
		sizer->AddGrowableCol(i);

	CreateStatusBar();
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


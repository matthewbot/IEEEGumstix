#include "ieee/sim/comm/CommFrame.h"

using namespace ieee;
using namespace boost;
using namespace std;

enum {
	SYNC_TIMER
};

BEGIN_EVENT_TABLE(CommFrame, wxFrame)
	EVT_TIMER(SYNC_TIMER, CommFrame::OnSyncEvent)
END_EVENT_TABLE()

CommFrame::CommFrame()
: wxFrame(NULL, -1, _("IEEE Comm"), wxDefaultPosition, wxSize(320, 240)),
  synctimer(this, SYNC_TIMER),
  notebook(this, -1, wxDefaultPosition, wxDefaultSize, wxNB_RIGHT) {
	synctimer.Start(40);

	panels.push_back(new WheelTabPanel(&notebook));
	panels.push_back(new SensorsTabPanel(&notebook));
	panels.push_back(new DriveTabPanel(&notebook));
	panels.push_back(new DumpTabPanel(&notebook));

	for (TabPanelVec::iterator i = panels.begin(); i != panels.end(); ++i)
		notebook.AddPage(*i, wxString((wxChar)(*i)->getTabCharacter()));

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(&notebook, 1, wxEXPAND);
	SetSizer(sizer);

	CreateStatusBar();

	try {
		robotptr.reset(new AVRRobot(robotconfig));
	} catch (std::exception &ex) {
		cerr << "Exception while creating AVRRobot: " << endl << ex.what() << endl;
		SetStatusText(_("Failed to create AVRRobot"));
	}
}

void CommFrame::OnSyncEvent(wxTimerEvent &) {
	if (!robotptr)
		return;
	AVRRobot &robot = *robotptr;

	while (robot.syncIn()) { }
	SetStatusText(wxString::FromAscii(robot.getAVRPacket().debugoutput));

	robot.disableAll(); // disable everything, tabs then enable what is currently in use

	for (TabPanelVec::iterator i = panels.begin(); i != panels.end(); ++i)
		(*i)->onSync(robot);

	robot.syncOut();
}

CommFrame::RobotConfig::RobotConfig() {
	wheels.left.minstop = wheels.right.minstop = wheels.back.minstop = 0;
	wheels.left.maxstop = wheels.right.maxstop = wheels.back.maxstop = 1800;
	wheels.left.offset = wheels.right.offset = wheels.back.offset = 0;

	sonar1.alpha = .3078;
	sonar1.beta = -53.11357 + 3.5;
	sonar2.alpha = .26518;
	sonar2.beta = -50.10155 + 3.5;

	compass.centerx = 57.500;
	compass.centery = 281.5;
	compass.yscale = 1.0644;
}


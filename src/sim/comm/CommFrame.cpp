#include "ieee/sim/comm/CommFrame.h"
#include <boost/assign/std/vector.hpp>

using namespace ieee;
using namespace boost;
using namespace boost::assign;
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
	wheels.left.offset = 40;
	wheels.right.offset = 0;
	wheels.back.offset = 70;
	wheels.turnhysteresis = 150;

	sonar1.alpha = .3078;
	sonar1.beta = -53.11357 + 3.5;
	sonar2.alpha = .26518;
	sonar2.beta = -50.10155 + 3.5;

	compass.centerx = 255;
	compass.centery = 11.42;
	compass.yscale = 1.0013;
	compass.leftwheel_offset.magx += 1.105413, -17.766327, 67.379417, -61.844683, -0.033371, -2.222486;
	compass.leftwheel_offset.magy += -9.2314, 65.9709, -145.6980, 118.5804, -74.1991, 2.2407;
	compass.rightwheel_offset.magx += -0.68205, 5.15608, -11.20517, 10.77148, 12.03845, -4.65361;
	compass.rightwheel_offset.magy += 0.087533, 2.294021, -10.848735, 16.962901, -38.659120, 8.029173;
	compass.backwheel_offset.magx += -0.51772, 4.10939, -8.35313, 3.73830, -9.84221, 0.27012;
	compass.backwheel_offset.magy += 0.46597, -2.08954, 2.30283, -1.12453, -11.01647, 0.65481;

	stepper.stepsize = 1.65/180*M_PI;
}


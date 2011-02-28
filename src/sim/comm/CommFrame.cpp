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

	sonar1.alpha = .3078;
	sonar1.beta = -53.11357 + 3.5;
	sonar2.alpha = .26518;
	sonar2.beta = -50.10155 + 3.5;

	compass.centerx = 204.26;
	compass.centery = 92.898;
	compass.yscale = 1.0037;
	compass.leftwheel_offset.magx += -0.35999, 6.58014, -49.68741, 206.44643, -523.28899, 824.64117, -766.87722, 397.08822, -114.75798, 3.22681;
	compass.leftwheel_offset.magy += -3.0764, 46.1999, -288.8646, 971.4253, -1899.0372, 2185.6556, -1425.3537, 473.9252, -116.4624, 2.2518;
	compass.rightwheel_offset.magx += -1.4085, 22.0113, -146.5448, 540.8970, -1205.7032, 1653.9614, -1353.2702, 604.1224, -101.1374, 1.7001;
	compass.rightwheel_offset.magy += 3.1626, -47.9573, 306.5287, -1072.9989, 2236.3373, -2818.2724, 2073.0765, -805.0540, 102.8950, -1.1871;
	compass.backwheel_offset.magx += -0.94414, 12.30830, -64.99283, 175.33200, -244.83722, 140.21278, 31.50476, -64.51520, 2.68270, -0.20292;
	compass.backwheel_offset.magy += 0.74264, -9.29949, 44.60206, -94.78007, 48.08379, 154.88016, -288.62041, 176.29711, -35.34805, 2.52520;
}


#include "ieee/sim/comm/CommFrame.h"

using namespace ieee;
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
  notebook(this, -1, wxDefaultPosition, wxDefaultSize, wxNB_RIGHT),
  robot(robotconfig) {
	synctimer.Start(40);

	panels.push_back(new WheelTabPanel(&notebook));
	panels.push_back(new SensorsTabPanel(&notebook));
	panels.push_back(new DriveTabPanel(&notebook));

	for (TabPanelVec::iterator i = panels.begin(); i != panels.end(); ++i)
		notebook.AddPage(*i, wxString((wxChar)(*i)->getTabCharacter()));

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(&notebook, 1, wxEXPAND);
	SetSizer(sizer);

	CreateStatusBar();
}

void CommFrame::OnSyncEvent(wxTimerEvent &) {
	while (robot.syncIn()) { }
	SetStatusText(wxString::FromAscii(robot.getAVRPacket().debugoutput));

	GumstixPacket &gp = robot.getGumstixPacket();
	gp.leftwheel_speed = gp.rightwheel_speed = gp.backwheel_speed = 0; // zero all speeds so that if no panel is writing them motors stop

	for (TabPanelVec::iterator i = panels.begin(); i != panels.end(); ++i)
		(*i)->onSync(robot);

	robot.syncOut();
}

CommFrame::RobotConfig::RobotConfig() {
	wheels.left.minstop = wheels.right.minstop = wheels.back.minstop = 0;
	wheels.left.maxstop = wheels.right.maxstop = wheels.back.maxstop = 1800;
	wheels.left.offset = wheels.right.offset = wheels.back.offset = 0;

	sonar1.calpoints.push_back(SonarSensor::CalPoint(1000, 50));
	sonar2.calpoints.push_back(SonarSensor::CalPoint(1000, 50));

	compass.centerx = -170;
	compass.centery = 1150;
	compass.yscale = .94;
}


#include "ieee/sim/control/ControlTestFrame.h"
#include <boost/assign/std/vector.hpp>
#include <iostream>

using namespace ieee;
using namespace boost;
using namespace boost::assign;
using namespace boost::property_tree;
using namespace std;

enum {
	SYNC_TIMER,
	STOP_BUTTON,
	RESET_BUTTON
};

BEGIN_EVENT_TABLE(ControlTestFrame, wxFrame)
	EVT_BUTTON(STOP_BUTTON, ControlTestFrame::OnStopEvent)
	EVT_BUTTON(RESET_BUTTON, ControlTestFrame::OnResetEvent)
	EVT_TIMER(SYNC_TIMER, ControlTestFrame::OnSyncEvent)
END_EVENT_TABLE()

ControlTestFrame::ControlTestFrame()
: wxFrame(NULL, -1, _("Control Test"), wxDefaultPosition, wxSize(320, 240)),
  poscontrol(configloader.getPositionControllerConfig()),
  grid(10, 10),
  gridscale(.1, .1, -.5, -.5),
  panel(this, 100, 100),
  gridlayer(grid, gridscale),
  posconlayer(*this, poscontrol),
  optionspanel(this, -1),
  stopbutton(&optionspanel, STOP_BUTTON, _("Stop"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT),
  resetbutton(&optionspanel, RESET_BUTTON, _("Reset"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT),
  statetext(&optionspanel, -1, _("")),
  synctimer(this, SYNC_TIMER) {
	panel.addLayer(&gridlayer);
	panel.addLayer(&posconlayer);

	wxBoxSizer *optionspanel_sizer = new wxBoxSizer(wxHORIZONTAL);
	optionspanel.SetSizer(optionspanel_sizer);
	optionspanel_sizer->Add(&stopbutton, 0, wxEXPAND);
	optionspanel_sizer->Add(&resetbutton, 0, wxEXPAND);
	optionspanel_sizer->Add(&statetext, 0, wxEXPAND);

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(sizer);
	sizer->Add(&panel, 1, wxEXPAND);
	sizer->Add(&optionspanel, 0, wxEXPAND);

	synctimer.Start(40);

	try {
		robotptr.reset(new AVRRobot(configloader.getAVRRobotConfig()));

		robotptr->setWheelAngles(M_PI/2, M_PI/2, M_PI/2);
		robotptr->syncIOWait(20);
		robotptr->calibrateCompassOffset();
	} catch (std::exception &ex) {
		cerr << "Exception while creating AVRRobot: " << endl << ex.what() << endl;
		SetStatusText(_("Failed to create AVRRobot"));
	}
}

void ControlTestFrame::OnStopEvent(wxCommandEvent &evt) {
	poscontrol.stop();
	panel.Refresh();
}

void ControlTestFrame::OnResetEvent(wxCommandEvent &evt) {
	if (!robotptr)
		return;
	robotptr->calibrateCompassOffset();
}

void ControlTestFrame::OnSyncEvent(wxTimerEvent &evt) {
	if (!robotptr)
		return;
	AVRRobot &robot = *robotptr;

	while (robot.syncIn()) { }
	poscontrol.update(robot);
	statetext.SetLabel(wxString::From8BitData(PositionController::stateToString(poscontrol.getState())));

	robot.syncOut();

	panel.Refresh();
}

void ControlTestFrame::onCommand(const Coord &coord, float dir) {
	PositionController::Command command;
	command.destpos = coord;
	command.destheading = Angle(dir);
	command.speed = 14;
	poscontrol.setCommand(command);
}


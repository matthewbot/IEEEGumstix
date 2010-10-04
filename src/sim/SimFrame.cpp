#include "SimFrame.h"

using namespace pathsim;
using namespace std;

enum { 
	STEP_BUTTON,
	RESET_BUTTON,
};

BEGIN_EVENT_TABLE(SimFrame, wxFrame)
	EVT_BUTTON(STEP_BUTTON, SimFrame::onStepPressed)
	EVT_BUTTON(RESET_BUTTON, SimFrame::onResetPressed)
END_EVENT_TABLE()

SimFrame::SimFrame(const World &world, Robot &robot) 
: robot(robot),
  wxFrame(NULL, -1, _("Hello World"), wxDefaultPosition, wxSize(400, 400)), 
  worldpanel(this, *this, world, robot),
  buttonpanel(this),
  stepbutton(&buttonpanel, STEP_BUTTON, _("Step")),
  resetbutton(&buttonpanel, RESET_BUTTON, _("Reset")) {
	wxBoxSizer *buttonpanelsizer = new wxBoxSizer(wxHORIZONTAL);
	buttonpanelsizer->Add(&stepbutton, 0, 0);
	buttonpanelsizer->Add(&resetbutton, 0, 0);
	buttonpanel.SetSizer(buttonpanelsizer);

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(&worldpanel, 1, wxEXPAND);
	sizer->Add(&buttonpanel, 0, wxEXPAND);
	SetSizer(sizer);
}

void SimFrame::onStepPressed(wxCommandEvent &evt) {
	robot.step();
	worldpanel.Refresh();
}

void SimFrame::onResetPressed(wxCommandEvent &evt) {
	robot.reset(Pos(0, 0), DIR_E);
	worldpanel.Refresh();
}

#include <iostream>

bool SimFrame::onWorldClicked(const Pos &pos) {
	cout << "World clicked at " << pos << endl;
	return true;
}

void SimFrame::onWorldDragged(const Pos &pos) {
	cout << "World dragged to " << pos << endl;
}


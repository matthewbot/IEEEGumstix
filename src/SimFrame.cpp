#include "SimFrame.h"

using namespace pathsim;
using namespace std;

enum { 
	STEP_BUTTON
};

BEGIN_EVENT_TABLE(SimFrame, wxFrame)
	EVT_BUTTON(STEP_BUTTON, SimFrame::onStepPressed)
END_EVENT_TABLE()

SimFrame::SimFrame(const World &world, Robot &robot) 
: robot(robot),
  wxFrame(NULL, -1, _("Hello World"), wxDefaultPosition, wxSize(400, 400)), 
  worldpanel(this, world, robot),
  stepbutton(this, STEP_BUTTON, _("Step")) {
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	
	sizer->Add(&worldpanel, 1, wxEXPAND);
	sizer->Add(&stepbutton, 0, 0);

	SetSizer(sizer);
}

void SimFrame::onStepPressed(wxCommandEvent &evt) {
	robot.step();
	worldpanel.Refresh();
}


#include "ieeepath/sim/SimFrame.h"

using namespace ieee;
using namespace std;

enum { 
	STEP_BUTTON,
	RESET_BUTTON,
};

BEGIN_EVENT_TABLE(SimFrame, wxFrame)
	EVT_BUTTON(STEP_BUTTON, SimFrame::onStepPressed)
	EVT_BUTTON(RESET_BUTTON, SimFrame::onResetPressed)
END_EVENT_TABLE()

SimFrame::SimFrame(World &world, Robot &robot) 
: wxFrame(NULL, -1, _("Hello World"), wxDefaultPosition, wxSize(400, 400)), 
  world(world),
  robot(robot),
  worldpanel(this, *this, world, robot),
  selectedid(-1),
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

bool SimFrame::onWorldClicked(const Pos &pos) {
	for (World::iterator i = world.begin(); i != world.end(); ++i) {
		int id = i->selectionTest(pos);
		if (id != -1) {
			selectedid = id;
			selectedobj = i;
			return true;
		}
	}
	
	return false;
}

void SimFrame::onWorldDragged(const Pos &pos) {
	selectedobj->selectionMoved(selectedid, pos);
	world.updateGrid();
	worldpanel.Refresh();
}


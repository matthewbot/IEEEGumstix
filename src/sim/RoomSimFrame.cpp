#include "ieee/sim/RoomSimFrame.h"
#include "ieee/sim/VictimWorldObject.h"
#include "ieee/sim/ObstacleWorldObject.h"

using namespace ieee;
using namespace std;

enum {
	STEP_BUTTON,
	RESET_BUTTON,
};

BEGIN_EVENT_TABLE(RoomSimFrame, wxFrame)
	EVT_BUTTON(STEP_BUTTON, RoomSimFrame::onStepPressed)
	EVT_BUTTON(RESET_BUTTON, RoomSimFrame::onResetPressed)
END_EVENT_TABLE()

RoomSimFrame::SimWorld::SimWorld()
: World(10, 10) {
	add(new ObstacleWorldObject(Pos(0, 3), Pos(3, 3), true));
	add(new ObstacleWorldObject(Pos(2, 6), Pos(3, 9), true));
	add(new ObstacleWorldObject(Pos(6, 3), Pos(9, 2), true));
	add(new ObstacleWorldObject(Pos(7, 6), Pos(6, 9), false));

	add(new VictimWorldObject(Pos(8, 1)));
	add(new VictimWorldObject(Pos(1, 8)));
	add(new VictimWorldObject(Pos(8, 8)));
}

RoomSimFrame::RoomSimFrame()
: wxFrame(NULL, -1, _("Hello World"), wxDefaultPosition, wxSize(400, 400)),
  robot(Coord(10, 10), world.getGrid()),
  gridlayer(robot.getMap(), robot.getGridScale()),
  objectlayer(world, robot.getGridScale(), *this),
  robotlayer(robot, robot.getGridScale()),
  worldpanel(this),
  selectedid(-1),
  buttonpanel(this),
  stepbutton(&buttonpanel, STEP_BUTTON, _("Step")),
  resetbutton(&buttonpanel, RESET_BUTTON, _("Reset")) {
  	worldpanel.addLayer(&gridlayer);
  	worldpanel.addLayer(&objectlayer);
  	worldpanel.addLayer(&robotlayer);

	wxBoxSizer *buttonpanelsizer = new wxBoxSizer(wxHORIZONTAL);
	buttonpanelsizer->Add(&stepbutton, 0, 0);
	buttonpanelsizer->Add(&resetbutton, 0, 0);
	buttonpanel.SetSizer(buttonpanelsizer);

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(&worldpanel, 1, wxEXPAND);
	sizer->Add(&buttonpanel, 0, wxEXPAND);
	SetSizer(sizer);
}

void RoomSimFrame::onStepPressed(wxCommandEvent &evt) {
	robot.step();
	worldpanel.Refresh();
}

void RoomSimFrame::onResetPressed(wxCommandEvent &evt) {
	robot.reset(Coord(10, 10));
	worldpanel.Refresh();
}

bool RoomSimFrame::onWorldClicked(const Pos &pos) {
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

void RoomSimFrame::onWorldDragged(const Pos &pos) {
	selectedobj->selectionMoved(selectedid, pos);
	world.updateGrid();
	worldpanel.Refresh();
}

bool RoomSimFrame::isVictimIdentified(const Pos &pos) {
	return robot.isVictimIdentified(pos);
}


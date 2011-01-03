#include "ieee/sim/room/RoomSimFrame.h"
#include "ieee/sim/shared/VictimWorldObject.h"
#include "ieee/sim/shared/ObstacleWorldObject.h"
#include "ieee/sim/shared/WorldIO.h"
#include <fstream>

using namespace ieee;
using namespace std;

enum {
	STEP_BUTTON,
	RESET_BUTTON,
	OBJECTS_MENU,
	WORLDGRID_MENU,
	MAPGRID_MENU
};

BEGIN_EVENT_TABLE(RoomSimFrame, wxFrame)
	EVT_BUTTON(STEP_BUTTON, RoomSimFrame::onStepPressed)
	EVT_BUTTON(RESET_BUTTON, RoomSimFrame::onResetPressed)
	EVT_MENU(wxID_OPEN, RoomSimFrame::onMenuOpen)
	EVT_MENU(wxID_SAVE, RoomSimFrame::onMenuSave)
	EVT_MENU(wxID_EXIT, RoomSimFrame::onMenuQuit)
	EVT_MENU(OBJECTS_MENU, RoomSimFrame::onMenuObjects)
	EVT_MENU(WORLDGRID_MENU, RoomSimFrame::onMenuWorldGrid)
	EVT_MENU(MAPGRID_MENU, RoomSimFrame::onMenuMapGrid)
END_EVENT_TABLE()

RoomSimFrame::SimWorld::SimWorld()
: World(10, 10) {
	add(new ObstacleWorldObject(Coord(5, 35), Coord(35, 35), true));
	add(new ObstacleWorldObject(Coord(25, 65), Coord(35, 95), true));
	add(new ObstacleWorldObject(Coord(65, 35), Coord(95, 25), true));
	add(new ObstacleWorldObject(Coord(75, 65), Coord(65, 95), false));

	add(new VictimWorldObject(Coord(85, 15)));
	add(new VictimWorldObject(Coord(15, 85)));
	add(new VictimWorldObject(Coord(85, 85)));
}


RoomSimFrame::RoomPlannerConfig::RoomPlannerConfig() {
	static const float roomwidth = 100, roomheight = 100;
	static const float gridwidth = 10, gridheight = 10;

	gridscale.sx = gridwidth/roomwidth;
	gridscale.sy = gridheight/roomheight;
    gridscale.xoff = gridscale.yoff = -.5;

	static const float nodegridwidth = 10, nodegridheight = 10;

	nodescale.sx = nodegridwidth/roomwidth;
	nodescale.sy = nodegridheight/roomheight;
    nodescale.xoff = nodescale.yoff = 0;

	static const int victimgridwidth = 10, victimgridheight = 10;

	victimscale.sx = victimgridwidth/roomwidth;
	victimscale.sy = victimgridheight/roomheight;
	victimscale.xoff = victimscale.yoff = -.5;
	victimradius = 4;
	victimidentifyradius = 14;

	routeevalconfig.pathcostfactor = 1;
	routeevalconfig.revealedscorefactor = 6;
	routeevalconfig.turncostconstant = 4;
	routeevalconfig.turncostfactor = 1;
	routeevalconfig.turncostdivider = 2;
}

RoomSimFrame::RoomSimFrame()
: wxFrame(NULL, -1, _("Hello World"), wxDefaultPosition, wxSize(400, 400)),
  robot(world.getGrid(), roomplannerconfig, Coord(10, 10)),
  worldgridlayer(world.getGrid(), roomplannerconfig.gridscale),
  mapgridlayer(robot.getMap(), roomplannerconfig.gridscale),
  objectlayer(world, *this),
  robotlayer(robot, roomplannerconfig.victimscale),
  worldpanel(this, 100, 100),
  selectedid(-1),
  buttonpanel(this),
  stepbutton(&buttonpanel, STEP_BUTTON, _("Step")),
  resetbutton(&buttonpanel, RESET_BUTTON, _("Reset")) {
  	worldpanel.addLayer(&mapgridlayer);
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

	wxMenu *file = new wxMenu();
	file->Append(wxID_OPEN, _T("&Open layout"));
	file->Append(wxID_SAVE, _T("&Save layout"));
	file->AppendSeparator();
	file->Append(wxID_EXIT, _T("&Quit"));

	wxMenu *view = new wxMenu();
	view->AppendCheckItem(OBJECTS_MENU, _T("&Objects"))->Check();
	view->AppendSeparator();
	view->AppendRadioItem(MAPGRID_MENU, _T("&Map Grid"));
	view->AppendRadioItem(WORLDGRID_MENU, _T("&World Grid"));

	wxMenuBar *menubar = new wxMenuBar();
	menubar->Append(file, _T("&File"));
	menubar->Append(view, _T("&View"));

	SetMenuBar(menubar);
}

void RoomSimFrame::onStepPressed(wxCommandEvent &evt) {
	robot.step();
	worldpanel.Refresh();
}

void RoomSimFrame::onResetPressed(wxCommandEvent &evt) {
	robot.reset(roomplannerconfig, Coord(10, 10));
	worldpanel.Refresh();
}

void RoomSimFrame::onMenuOpen(wxCommandEvent &evt) {
	wxFileDialog opendialog(this, _("Open layout"), _(""), _(""), _("DAT files (*.dat)|*.dat"), wxFD_OPEN|wxFD_FILE_MUST_EXIST);
	if (opendialog.ShowModal() == wxID_CANCEL)
		return;

	ifstream in(opendialog.GetPath().fn_str());
	world.clear();
	readWorldObjects(in, world);
	worldpanel.Refresh();
}

void RoomSimFrame::onMenuSave(wxCommandEvent &evt) {
	wxFileDialog savedialog(this, _("Save layout"), _(""), _("layout.dat"), _("DAT files (*.dat)|*.dat"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	if (savedialog.ShowModal() == wxID_CANCEL)
		return;     // the user changed idea...

	ofstream out(savedialog.GetPath().fn_str());
	writeWorldObjects(out, world);
}

void RoomSimFrame::onMenuQuit(wxCommandEvent &evt) {
	Close(false);
}

void RoomSimFrame::onMenuObjects(wxCommandEvent &evt) {
	if (evt.IsChecked() == worldpanel.hasLayer(&objectlayer))
		return;

	if (evt.IsChecked())
		worldpanel.addLayer(&objectlayer);
	else
		worldpanel.removeLayer(&objectlayer);
	worldpanel.Refresh();
}

void RoomSimFrame::onMenuWorldGrid(wxCommandEvent &evt) {
	showGrid(false);
}

void RoomSimFrame::onMenuMapGrid(wxCommandEvent &evt) {
	showGrid(true);
}

void RoomSimFrame::showGrid(bool mapgrid) {
	if (mapgrid == worldpanel.hasLayer(&mapgridlayer))
		return;

	if (mapgrid) {
		worldpanel.addLayer(&mapgridlayer);
		worldpanel.removeLayer(&worldgridlayer);
	} else {
		worldpanel.addLayer(&worldgridlayer);
		worldpanel.removeLayer(&mapgridlayer);
	}

	worldpanel.Refresh();
}

bool RoomSimFrame::onWorldClicked(const Coord &coord) {
	for (World::iterator i = world.begin(); i != world.end(); ++i) {
		int id = i->selectionTest(coord);
		if (id != -1) {
			selectedid = id;
			selectedobj = i;
			return true;
		}
	}

	return false;
}

void RoomSimFrame::onWorldDragged(const Coord &coord) {
	selectedobj->selectionMoved(selectedid, coord);
	world.updateGrid();
	worldpanel.Refresh();
}


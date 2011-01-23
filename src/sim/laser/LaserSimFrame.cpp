#include "ieee/sim/laser/LaserSimFrame.h"
#include "ieee/drivers/LaserPlot.h"

using namespace ieee;
using namespace std;

enum {
	WORLDGRID_UDPATE_EVENT
};

BEGIN_EVENT_TABLE(LaserSimFrame, wxFrame)
	EVT_MENU(WORLDGRID_UDPATE_EVENT, LaserSimFrame::OnWorldGridUpdateEvent)
END_EVENT_TABLE()

LaserSimFrame::LaserSimFrame()
: wxFrame(NULL, -1, _("Hello World"), wxDefaultPosition, wxSize(450, 400)),
  thread(*this),
  grid(10, 10, WorldGrid::UNKNOWN),
  gridscale(.1, .1, -.5, -.5),
  gridlayer(grid, gridscale),
  laserlayer(readings),
  notebook(this, -1, wxDefaultPosition, wxDefaultSize, wxNB_RIGHT) {
  	gridworldpanel = new WorldPanel(&notebook);
	gridworldpanel->addLayer(&gridlayer);
	gridworldpanel->addLayer(&laserlayer);

	rawimagepanel = new ImagePanel(&notebook);

	notebook.AddPage(gridworldpanel, _("Grid"));
	notebook.AddPage(rawimagepanel, _("Raw"));

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(&notebook, 1, wxEXPAND);
	SetSizer(sizer);

	thread.start();
}

LaserSimFrame::~LaserSimFrame() {
	thread.stop();
}

void LaserSimFrame::onNewLaserData() {
	// this is the best thing I can find short of defining my own event class (assuming thats possible?)
	wxCommandEvent event(wxEVT_COMMAND_MENU_SELECTED, WORLDGRID_UDPATE_EVENT);
	AddPendingEvent(event);
}

LaserSimFrame::LaserPlotConfig::LaserPlotConfig() {
	maxlasers = 3;
	maxangle = 100;

	static const int minhits_array[] = { 20, 20, 20};
	minhits = minhits_array;

	static const WorldGrid::GridSquare squarelookup_array[] = { WorldGrid::LARGE_OBSTACLE, WorldGrid::VICTIM, WorldGrid::SMALL_OBSTACLE };
	squarelookup = squarelookup_array;
}

void LaserSimFrame::OnWorldGridUpdateEvent(wxCommandEvent& event) {
	grid.clear(WorldGrid::UNKNOWN);

	readings = thread.getLaserReadings();
	LaserPlot laserplot(laserplotconfig, readings, Coord(50, 100), M_PI/2, grid, gridscale);

	rawimagepanel->update(thread.getLaserDebug().rawframe);

	Refresh();
}


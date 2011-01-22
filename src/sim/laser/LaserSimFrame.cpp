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
: wxFrame(NULL, -1, _("Hello World"), wxDefaultPosition, wxSize(400, 400)),
  thread(*this),
  grid(10, 10, WorldGrid::UNKNOWN),
  gridscale(.1, .1, -.5, -.5), // TODO get rid of const references in gridlayer and other places, use setters. This isn't really thread safe until this is chagned
  gridlayer(grid, gridscale),
  laserlayer(readings),
  worldpanel(this) {
	worldpanel.addLayer(&gridlayer);
	worldpanel.addLayer(&laserlayer);

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(&worldpanel, 1, wxEXPAND);
	SetSizer(sizer);

	thread.start();
}

LaserSimFrame::~LaserSimFrame() {
	thread.stop();
}

void LaserSimFrame::onNewLaserData(const LaserSensor::Readings &readings) {
	wxCriticalSectionLocker locker(grid_critsect);

	this->readings = readings;

	grid.clear(WorldGrid::UNKNOWN);
	LaserPlot laserplot(laserplotconfig, readings, Coord(50, 100), M_PI/2, grid, gridscale);

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
	Refresh();
}


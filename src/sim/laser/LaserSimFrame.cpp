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
  rawreadingtext(this, -1, _("")),
  notebook(this, -1, wxDefaultPosition, wxDefaultSize, wxNB_RIGHT),
  gridworldpanel(new WorldPanel(&notebook)),
  laserimagepanel(new LaserImagePanel(&notebook)),
  greenimagepanel(new ImagePanel(&notebook)),
  rawimagepanel(new ImagePanel(&notebook)) {
	gridworldpanel->addLayer(&gridlayer);
	gridworldpanel->addLayer(&laserlayer);

	notebook.AddPage(gridworldpanel, _("Grid"));
	notebook.AddPage(laserimagepanel, _("Laser"));
	notebook.AddPage(greenimagepanel, _("Green"));
	notebook.AddPage(rawimagepanel, _("Raw"));

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(&notebook, 1, wxEXPAND);
	sizer->Add(&rawreadingtext, 0, wxEXPAND);
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
	readings = thread.getLaserReadings();
	LaserSensor::Debug debug = thread.getLaserDebug();

	grid.clear(WorldGrid::UNKNOWN);
	LaserPlot laserplot(laserplotconfig, readings, Coord(50, 100), M_PI/2, grid, gridscale);

	laserimagepanel->update(debug.rawframe, debug.rawreadings);
	greenimagepanel->update(debug.greenframe);
	rawimagepanel->update(debug.rawframe);

	stringstream buf;
	buf << "Raw readings: ";
	for (int laser=0; laser<readings.size(); laser++) {
		const LaserTrack::LineData &linedata = debug.rawreadings[laser];
		buf << linedata[linedata.size()/2] << " ";
	}

	rawreadingtext.SetLabel(wxString(buf.str().c_str(), wxConvUTF8)); // eh...

	Refresh();
}


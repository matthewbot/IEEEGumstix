#include "ieee/sim/laser/LaserSimFrame.h"
#include "ieee/drivers/laser/LaserPlot.h"
#include <sstream>
#include <boost/assign.hpp>

using namespace ieee;
using namespace boost::assign;
using namespace std;

enum {
	WORLDGRID_UDPATE_EVENT,
	MAIN_NOTEBOOK
};

BEGIN_EVENT_TABLE(LaserSimFrame, wxFrame)
	EVT_MENU(WORLDGRID_UDPATE_EVENT, LaserSimFrame::OnWorldGridUpdateEvent)
	EVT_NOTEBOOK_PAGE_CHANGED(MAIN_NOTEBOOK, LaserSimFrame::OnPageChangeEvent)
END_EVENT_TABLE()

LaserSimFrame::LaserSimFrame()
: wxFrame(NULL, -1, _("Hello World"), wxDefaultPosition, wxSize(450, 400)),
  grid(10, 10, WorldGrid::UNKNOWN),
  gridscale(.1, .1, -.5, -.5),
  lasermapper(configloader.getLaserConfig(), *this, grid.getWidth(), grid.getHeight(), gridscale),
  gridlayer(grid, gridscale),
  laserlayer(readings),
  rawreadingtext(this, -1, _("")),
  notebook(this, MAIN_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxNB_RIGHT),
  gridworldpanel(new WorldPanel(&notebook, 100, 100)),
  laserimagepanel(new LaserImagePanel(&notebook)),
  greenimagepanel(new ImagePanel(&notebook)),
  rawimagepanel(new ImagePanel(&notebook)),
  calibratepanel(new LaserCalibratePanel(&notebook, *this)) {
  	lasermapper.updateState(Coord(50, 100), M_PI/2);

	gridworldpanel->addLayer(&gridlayer);
	gridworldpanel->addLayer(&laserlayer);

	notebook.AddPage(gridworldpanel, _("Grid"));
	notebook.AddPage(laserimagepanel, _("Laser"));
	notebook.AddPage(greenimagepanel, _("Green"));
	notebook.AddPage(rawimagepanel, _("Raw"));
	notebook.AddPage(calibratepanel, _("Cal"));

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(&notebook, 1, wxEXPAND);
	sizer->Add(&rawreadingtext, 0, wxEXPAND);
	SetSizer(sizer);

	rawreadingtext.SetFont(wxFont(12, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
}

void LaserSimFrame::onNewLaserData() {
	// this is the best thing I can find short of defining my own event class (assuming thats possible?)
	wxCommandEvent event(wxEVT_COMMAND_MENU_SELECTED, WORLDGRID_UDPATE_EVENT);
	AddPendingEvent(event);
}

void LaserSimFrame::onApplyCalibrations(const std::vector<LaserSensor::Calibration> &calibrations) {
	configloader.getLaserConfig().sensor.calibrations = calibrations;
}

void LaserSimFrame::onSaveCalibrations() {
	configloader.saveLaserConfig();
}

void LaserSimFrame::OnWorldGridUpdateEvent(wxCommandEvent& event) {
	readings = lasermapper.getLaserReadings();
	LaserSensor::Debug debug = lasermapper.getLaserDebug();
	grid = lasermapper.getMapGrid();
	lasermapper.clearMapGrid();

	laserimagepanel->update(debug.rawframe, debug.rawreadings);
	greenimagepanel->update(debug.greenframe);
	rawimagepanel->update(debug.rawframe);
	calibratepanel->update(debug.rawreadings);

	stringstream buf;
	if (debug.rawreadings.size()) {
		buf << "Raw\t";
		for (int laser=0; laser<debug.rawreadings.size(); laser++) {
			const LaserTrack::LineData &linedata = debug.rawreadings[laser];
			buf << linedata[linedata.size()/2] << "\t";
		}
	}
	buf << "Time " << (int)lasermapper.getUpdateTime() << " ms";

	rawreadingtext.SetLabel(wxString(buf.str().c_str(), wxConvUTF8)); // eh...

	Refresh();
}

void LaserSimFrame::OnPageChangeEvent(wxNotebookEvent &evt) {
	bool plot = evt.GetSelection() == 0;
	bool debug = !plot;
	lasermapper.setFlags(debug, plot);
}


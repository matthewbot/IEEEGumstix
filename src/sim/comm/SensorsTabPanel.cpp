#include "ieee/sim/comm/SensorsTabPanel.h"

using namespace ieee;

enum {
	SONAR1_ITEM,
	SONAR2_ITEM
};

SensorsTabPanel::SensorsTabPanel(wxWindow *parent)
: wxPanel(parent, -1),
  sensorlist(this, -1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT) {
	sensorlist.InsertColumn(0, _("Sensor"));
	sensorlist.InsertColumn(1, _("Raw Value"));
	sensorlist.InsertColumn(2, _("Calibrated Value"));
	sensorlist.InsertItem(SONAR1_ITEM, _("Sonar 1"));
	sensorlist.InsertItem(SONAR2_ITEM, _("Sonar 2"));

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(sizer);
	sizer->Add(&sensorlist, 1, wxEXPAND);
}

void SensorsTabPanel::readSensorData(const AVRPacket &avr) {
	sensorlist.SetItem(SONAR1_ITEM, 1, wxString::Format(_("%i"), avr.sonar1_reading));
	sensorlist.SetItem(SONAR2_ITEM, 1, wxString::Format(_("%i"), avr.sonar2_reading));
}


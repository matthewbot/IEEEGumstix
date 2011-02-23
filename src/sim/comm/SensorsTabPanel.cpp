#include "ieee/sim/comm/SensorsTabPanel.h"

using namespace ieee;

enum {
	SONAR1_ITEM,
	SONAR2_ITEM,
	MAGX_ITEM,
	MAGY_ITEM,
	MAGZ_ITEM
};

SensorsTabPanel::SensorsTabPanel(wxWindow *parent)
: TabPanel(parent),
  sensorlist(this, -1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT),
  freezecheck(this, -1, _("Freeze")),
  sonaranglespin(this, -1, _("0")) {
	sensorlist.InsertColumn(0, _("Sensor"));
	sensorlist.InsertColumn(1, _("Raw Value"));
	sensorlist.InsertColumn(2, _("Calibrated Value"));
	sensorlist.InsertItem(SONAR1_ITEM, _("Sonar 1"));
	sensorlist.InsertItem(SONAR2_ITEM, _("Sonar 2"));
	sensorlist.InsertItem(MAGX_ITEM, _("Mag X"));
	sensorlist.InsertItem(MAGY_ITEM, _("Mag Y"));
	sensorlist.InsertItem(MAGZ_ITEM, _("Mag Z"));

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(sizer);
	sizer->Add(&sensorlist, 1, wxEXPAND);
	sizer->Add(&sonaranglespin, 0, 0);
	sizer->Add(&freezecheck, 0, wxEXPAND);

	sonaranglespin.SetRange(-360, 360);
}

char SensorsTabPanel::getTabCharacter() const { return 'S'; }

void SensorsTabPanel::onSync(AVRRobot &robot) {
	if (freezecheck.GetValue())
		return;

	const AVRPacket &ap = robot.getAVRPacket();

	sensorlist.SetItem(SONAR1_ITEM, 1, wxString::Format(_("%i"), ap.sonar1_reading));
	sensorlist.SetItem(SONAR2_ITEM, 1, wxString::Format(_("%i"), ap.sonar2_reading));
	sensorlist.SetItem(SONAR1_ITEM, 2, wxString::Format(_("%.2f"), robot.getSonar1().first));
	sensorlist.SetItem(SONAR2_ITEM, 2, wxString::Format(_("%.2f"), robot.getSonar2().first));

	sensorlist.SetItem(MAGX_ITEM, 1, wxString::Format(_("%i"), ap.mag_x));
	sensorlist.SetItem(MAGY_ITEM, 1, wxString::Format(_("%i"), ap.mag_y));
	sensorlist.SetItem(MAGZ_ITEM, 1, wxString::Format(_("%i"), ap.mag_z));

	robot.setSonarAngle(sonaranglespin.GetValue() / 180 * M_PI);
}


#include "ieee/sim/comm/SensorsTabPanel.h"

using namespace ieee;

enum {
	SONAR1_ITEM,
	SONAR2_ITEM,
	MAGX_ITEM,
	MAGY_ITEM,
	MAGZ_ITEM,
	MAGANGLE_ITEM,
	BATTVOLTS_ITEM
};

SensorsTabPanel::SensorsTabPanel(wxWindow *parent)
: TabPanel(parent),
  sensorlist(this, -1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT),
  freezecheck(this, -1, _("Freeze")),
  sonaranglepanel(this),
  sonaranglelabel(&sonaranglepanel, -1, _("Stepper")),
  sonaranglespin(&sonaranglepanel, -1, _("0")),
  compasssync(&sonaranglepanel, -1, _("Mag Sync")),
  stepperen(&sonaranglepanel, -1, _("En")) {
	sensorlist.InsertColumn(0, _("Sensor"));
	sensorlist.InsertColumn(1, _("Raw Value"));
	sensorlist.InsertColumn(2, _("Calibrated Value"));
	sensorlist.InsertItem(SONAR1_ITEM, _("Sonar 1"));
	sensorlist.InsertItem(SONAR2_ITEM, _("Sonar 2"));
	sensorlist.InsertItem(MAGX_ITEM, _("Mag X"));
	sensorlist.InsertItem(MAGY_ITEM, _("Mag Y"));
	sensorlist.InsertItem(MAGZ_ITEM, _("Mag Z"));
	sensorlist.InsertItem(MAGANGLE_ITEM, _("Mag Angle"));
	sensorlist.InsertItem(BATTVOLTS_ITEM, _("Batt V"));

	wxBoxSizer *sonaranglepanel_sizer = new wxBoxSizer(wxHORIZONTAL);
	sonaranglepanel.SetSizer(sonaranglepanel_sizer);
	sonaranglepanel_sizer->Add(&sonaranglelabel);
	sonaranglepanel_sizer->Add(&sonaranglespin);
	sonaranglepanel_sizer->Add(&compasssync);
	sonaranglepanel_sizer->Add(&stepperen);

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(sizer);
	sizer->Add(&sensorlist, 1, wxEXPAND);
	sizer->Add(&freezecheck, 0, wxEXPAND);
	sizer->Add(&sonaranglepanel, 0, 0);

	sonaranglespin.SetRange(-360, 360);
}

char SensorsTabPanel::getTabCharacter() const { return 'S'; }

void SensorsTabPanel::onSync(AVRRobot &robot) {
	if (freezecheck.GetValue())
		return;

	const AVRPacket &ap = robot.getAVRPacket();

	sensorlist.SetItem(SONAR1_ITEM, 1, wxString::Format(_("%i"), ap.sonar1_reading));
	sensorlist.SetItem(SONAR2_ITEM, 1, wxString::Format(_("%i"), ap.sonar2_reading));
	sensorlist.SetItem(SONAR1_ITEM, 2, wxString::Format(_("%.2f"), robot.getSonar1().dist));
	sensorlist.SetItem(SONAR2_ITEM, 2, wxString::Format(_("%.2f"), robot.getSonar2().dist));

	sensorlist.SetItem(MAGX_ITEM, 1, wxString::Format(_("%i"), ap.mag_x));
	sensorlist.SetItem(MAGY_ITEM, 1, wxString::Format(_("%i"), ap.mag_y));
	sensorlist.SetItem(MAGZ_ITEM, 1, wxString::Format(_("%i"), ap.mag_z));
	float angle = robot.getCompassAngle()/M_PI*180;
	sensorlist.SetItem(MAGANGLE_ITEM, 2, wxString::Format(_("%.2f"), angle));

	sensorlist.SetItem(BATTVOLTS_ITEM, 1, wxString::Format(_("%.2f"), ap.batt_volts / 1000.0));

	if (compasssync.GetValue())
		sonaranglespin.SetValue(-(int)round(angle));
	robot.setSonarAngle(sonaranglespin.GetValue()/180.0*M_PI);
	robot.setStepperEnabled(stepperen.GetValue());
}


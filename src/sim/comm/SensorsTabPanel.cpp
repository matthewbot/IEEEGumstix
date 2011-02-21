#include "ieee/sim/comm/SensorsTabPanel.h"

using namespace ieee;

enum {
	SONAR1_ITEM,
	SONAR2_ITEM,
	MAGX_ITEM,
	MAGY_ITEM,
	MAGZ_ITEM
};

enum {
	FREEZE_CHECK,
	SONAR_SPIN
};

BEGIN_EVENT_TABLE(SensorsTabPanel, wxPanel)
	EVT_SPINCTRL(SONAR_SPIN, SensorsTabPanel::OnSonarSpin)
	EVT_TEXT_ENTER(SONAR_SPIN, SensorsTabPanel::OnSonarEnter)
END_EVENT_TABLE()

SensorsTabPanel::SensorsTabPanel(wxWindow *parent, Callbacks &callbacks)
: wxPanel(parent, -1),
  callbacks(callbacks),
  sensorlist(this, -1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT),
  freezecheck(this, FREEZE_CHECK, _("Freeze")),
  sonaranglespin(this, SONAR_SPIN, _("0")) {
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

void SensorsTabPanel::readSensorData(const AVRPacket &avr) {
	if (freezecheck.GetValue())
		return;

	sensorlist.SetItem(SONAR1_ITEM, 1, wxString::Format(_("%i"), avr.sonar1_reading));
	sensorlist.SetItem(SONAR2_ITEM, 1, wxString::Format(_("%i"), avr.sonar2_reading));
	sensorlist.SetItem(SONAR1_ITEM, 2, wxString::Format(_("%.2f"), 0.131*avr.sonar1_reading));
	sensorlist.SetItem(SONAR2_ITEM, 2, wxString::Format(_("%.2f"), 0.131*avr.sonar2_reading));

	sensorlist.SetItem(MAGX_ITEM, 1, wxString::Format(_("%i"), avr.mag_x));
	sensorlist.SetItem(MAGY_ITEM, 1, wxString::Format(_("%i"), avr.mag_y));
	sensorlist.SetItem(MAGZ_ITEM, 1, wxString::Format(_("%i"), avr.mag_z));
}

void SensorsTabPanel::writeSonarAngle(GumstixPacket &gp) const {
	gp.sonar_angle = sonaranglespin.GetValue();
}

void SensorsTabPanel::OnSonarSpin(wxSpinEvent &evt) {
	callbacks.onSonarAngleChanged();
}

void SensorsTabPanel::OnSonarEnter(wxCommandEvent &evt) {
	callbacks.onSonarAngleChanged();
}



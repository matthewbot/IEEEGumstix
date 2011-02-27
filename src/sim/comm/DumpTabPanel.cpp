#include "ieee/sim/comm/DumpTabPanel.h"
#include <fstream>

using namespace ieee;
using namespace std;

enum {
	CLEAR_BUTTON,
	SAVE_BUTTON
};

BEGIN_EVENT_TABLE(DumpTabPanel, wxPanel)
	EVT_BUTTON(CLEAR_BUTTON, DumpTabPanel::OnClearButton)
	EVT_BUTTON(SAVE_BUTTON, DumpTabPanel::OnSaveButton)
END_EVENT_TABLE()

DumpTabPanel::DumpTabPanel(wxWindow *parent)
: TabPanel(parent),
  dumptext(this, -1, _(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY),
  selectpanel(this, -1),
  enablecheck(&selectpanel, -1, _("En")),
  sonarcheck(&selectpanel, -1, _("Sonar")),
  magcheck(&selectpanel, -1, _("Mag")),
  clearbutton(&selectpanel, CLEAR_BUTTON, _("Clear"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT),
  savebutton(&selectpanel, SAVE_BUTTON, _("Save"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT) {
  	wxBoxSizer *selectpanel_sizer = new wxBoxSizer(wxHORIZONTAL);
  	selectpanel.SetSizer(selectpanel_sizer);
  	selectpanel_sizer->Add(&enablecheck, 0, wxEXPAND);
  	selectpanel_sizer->Add(&sonarcheck, 0, wxEXPAND);
  	selectpanel_sizer->Add(&magcheck, 0, wxEXPAND);
  	selectpanel_sizer->AddStretchSpacer();
  	selectpanel_sizer->Add(&clearbutton, 0);
  	selectpanel_sizer->Add(&savebutton, 0);

  	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
  	SetSizer(sizer);
  	sizer->Add(&dumptext, 1, wxEXPAND);
  	sizer->Add(&selectpanel, 0, wxEXPAND);
}

char DumpTabPanel::getTabCharacter() const { return 'U'; }
void DumpTabPanel::onSync(AVRRobot &robot) {
	if (!enablecheck.GetValue())
		return;

	const AVRPacket &ap = robot.getAVRPacket();

	if (sonarcheck.GetValue())
		dumptext << ap.sonar1_reading << _(" ") << ap.sonar2_reading;
	if (magcheck.GetValue())
		dumptext << ap.mag_x << _(" ") << ap.mag_y << _(" ") << ap.mag_z;
	dumptext << _("\n");
}

void DumpTabPanel::OnClearButton(wxCommandEvent &evt) {
	dumptext.Clear();
}

void DumpTabPanel::OnSaveButton(wxCommandEvent &evt) {
	wxFileDialog filedialog(this, _("Save dump"), _(""), _(""), _("Text files|*.txt"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (filedialog.ShowModal() == wxID_CANCEL)
		return;

	ofstream out(filedialog.GetPath().mb_str());
	out << dumptext.GetValue().mb_str();
}


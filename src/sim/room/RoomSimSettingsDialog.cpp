#include "ieee/sim/room/RoomSimSettingsDialog.h"

using namespace ieee;

RoomSimSettingsDialog::RoomSimSettingsDialog(wxWindow *parent, Settings &settings)
: settings(settings),
  wxDialog(parent, -1, _("RoomSim Settings"), wxDefaultPosition, wxSize(300, 400)),
  buttonpanel(this),
  okbutton(&buttonpanel, wxID_OK, _T("Close")),
  cancelbutton(&buttonpanel, wxID_CANCEL, _T("Cancel")),
  fieldpanel(this),
  gridsizelabel(&fieldpanel, -1, _T("Grid size")),
  gridsizepanel(&fieldpanel),
  gridwidthtext(&gridsizepanel, -1, _(""), wxDefaultPosition, wxSize(40, wxDefaultCoord)),
  gridlabelx(&gridsizepanel, -1, _("x")),
  gridheighttext(&gridsizepanel, -1, _(""), wxDefaultPosition, wxSize(40, wxDefaultCoord)) {
	wxBoxSizer *buttonsizer = new wxBoxSizer(wxHORIZONTAL);
	buttonpanel.SetSizer(buttonsizer);
	buttonsizer->Add(&cancelbutton, 0, wxALIGN_RIGHT);
	buttonsizer->Add(&okbutton, 0, wxALIGN_RIGHT);

	wxBoxSizer *gridsizesizer = new wxBoxSizer(wxHORIZONTAL);
	gridsizepanel.SetSizer(gridsizesizer);
	gridsizesizer->Add(&gridwidthtext, 0, 0);
	gridsizesizer->Add(&gridlabelx, 0, wxALIGN_CENTER);
	gridsizesizer->Add(&gridheighttext, 0, 0);

	wxFlexGridSizer *fieldsizer = new wxFlexGridSizer(2, 10, 10);
	fieldsizer->AddGrowableCol(0);
	fieldpanel.SetSizer(fieldsizer);
	fieldsizer->Add(&gridsizelabel, 0, wxALIGN_CENTER_VERTICAL);
	fieldsizer->Add(&gridsizepanel, 0, wxALIGN_RIGHT);

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(sizer);
	sizer->Add(&fieldpanel, 1, wxEXPAND | wxALL, 5);
	sizer->Add(&buttonpanel, 0, wxALL | wxALIGN_RIGHT, 5);
}

bool RoomSimSettingsDialog::show() {
	gridwidthtext.SetValue(wxString() << settings.gridwidth);
	gridheighttext.SetValue(wxString() << settings.gridheight);

	if (ShowModal() != wxID_OK)
		return false;

	long tmp;

	if (gridwidthtext.GetValue().ToLong(&tmp))
		settings.gridwidth = tmp;

	if (gridheighttext.GetValue().ToLong(&tmp))
		settings.gridheight = tmp;

	return true;
}



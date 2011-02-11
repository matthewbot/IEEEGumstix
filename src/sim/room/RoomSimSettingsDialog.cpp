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
  gridheighttext(&gridsizepanel, -1, _(""), wxDefaultPosition, wxSize(40, wxDefaultCoord)),
  nodesizelabel(&fieldpanel, -1, _T("Node grid size")),
  nodesizepanel(&fieldpanel),
  nodewidthtext(&nodesizepanel, -1, _(""), wxDefaultPosition, wxSize(40, wxDefaultCoord)),
  nodelabelx(&nodesizepanel, -1, _("x")),
  nodeheighttext(&nodesizepanel, -1, _(""), wxDefaultPosition, wxSize(40, wxDefaultCoord)),
  robotsizelabel(&fieldpanel, -1, _T("Robot radius")),
  robotsizetext(&fieldpanel, -1, _("")),
  victimidentifylabel(&fieldpanel, -1, _T("Victim identification radius")),
  victimidentifytext(&fieldpanel, -1, _("")) {
	wxBoxSizer *buttonsizer = new wxBoxSizer(wxHORIZONTAL);
	buttonpanel.SetSizer(buttonsizer);
	buttonsizer->Add(&cancelbutton, 0, wxALIGN_RIGHT);
	buttonsizer->Add(&okbutton, 0, wxALIGN_RIGHT);

	wxBoxSizer *gridsizesizer = new wxBoxSizer(wxHORIZONTAL);
	gridsizepanel.SetSizer(gridsizesizer);
	gridsizesizer->Add(&gridwidthtext, 0, 0);
	gridsizesizer->Add(&gridlabelx, 0, wxALIGN_CENTER);
	gridsizesizer->Add(&gridheighttext, 0, 0);

	wxBoxSizer *nodesizesizer = new wxBoxSizer(wxHORIZONTAL);
	nodesizepanel.SetSizer(nodesizesizer);
	nodesizesizer->Add(&nodewidthtext, 0, 0);
	nodesizesizer->Add(&nodelabelx, 0, wxALIGN_CENTER);
	nodesizesizer->Add(&nodeheighttext, 0, 0);

	wxFlexGridSizer *fieldsizer = new wxFlexGridSizer(2, 10, 10);
	fieldsizer->AddGrowableCol(0);
	fieldpanel.SetSizer(fieldsizer);
	fieldsizer->Add(&gridsizelabel, 0, wxALIGN_CENTER_VERTICAL);
	fieldsizer->Add(&gridsizepanel, 0, wxALIGN_RIGHT);
	fieldsizer->Add(&nodesizelabel, 0, wxALIGN_CENTER_VERTICAL);
	fieldsizer->Add(&nodesizepanel, 0, wxALIGN_RIGHT);
	fieldsizer->Add(&robotsizelabel, 0, wxALIGN_CENTER_VERTICAL);
	fieldsizer->Add(&robotsizetext, 0, wxALIGN_RIGHT);
	fieldsizer->Add(&victimidentifylabel, 0, wxALIGN_CENTER_VERTICAL);
	fieldsizer->Add(&victimidentifytext, 0, wxALIGN_RIGHT);

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(sizer);
	sizer->Add(&fieldpanel, 1, wxEXPAND | wxALL, 5);
	sizer->Add(&buttonpanel, 0, wxALL | wxALIGN_RIGHT, 5);
}

bool RoomSimSettingsDialog::show() {
	gridwidthtext.SetValue(wxString() << settings.gridwidth);
	gridheighttext.SetValue(wxString() << settings.gridheight);
	nodewidthtext.SetValue(wxString() << settings.nodewidth);
	nodeheighttext.SetValue(wxString() << settings.nodeheight);
	robotsizetext.SetValue(wxString() << settings.robotradius);
	victimidentifytext.SetValue(wxString() << settings.victimidentifyradius);

	if (ShowModal() != wxID_OK)
		return false;

	long tmp;
	double dtmp;

	if (gridwidthtext.GetValue().ToLong(&tmp))
		settings.gridwidth = tmp;

	if (gridheighttext.GetValue().ToLong(&tmp))
		settings.gridheight = tmp;

	if (nodewidthtext.GetValue().ToLong(&tmp))
		settings.nodewidth = tmp;

	if (nodeheighttext.GetValue().ToLong(&tmp))
		settings.nodeheight = tmp;

	if (robotsizetext.GetValue().ToDouble(&dtmp))
		settings.robotradius = (float)dtmp;

	if (victimidentifytext.GetValue().ToDouble(&dtmp))
		settings.victimidentifyradius = (float)dtmp;

	return true;
}



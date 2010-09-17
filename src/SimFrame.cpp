#include "SimFrame.h"

using namespace pathsim;

SimFrame::SimFrame(const World &world) 
: wxFrame(NULL, -1, _("Hello World"), wxDefaultPosition, wxSize(400, 400)), 
  worldpanel(this, world) {
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(&worldpanel, 1, wxEXPAND);
	SetSizer(sizer);
}


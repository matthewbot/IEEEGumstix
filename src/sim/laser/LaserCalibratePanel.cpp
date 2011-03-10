#include "ieee/sim/laser/LaserCalibratePanel.h"

using namespace ieee;
using namespace std;

enum {
	CAPTURE_BUTTON,
	DROP_BUTTON,
	APPLY_BUTTON,
	SAVE_BUTTON
};

BEGIN_EVENT_TABLE(LaserCalibratePanel, wxPanel)
	EVT_BUTTON(CAPTURE_BUTTON, LaserCalibratePanel::OnCaptureButton)
	EVT_BUTTON(DROP_BUTTON, LaserCalibratePanel::OnDropButton)
	EVT_BUTTON(APPLY_BUTTON, LaserCalibratePanel::OnApplyButton)
	EVT_BUTTON(SAVE_BUTTON, LaserCalibratePanel::OnSaveButton)
END_EVENT_TABLE()

LaserCalibratePanel::LaserCalibratePanel(wxWindow *parent, Callbacks &callbacks)
: wxPanel(parent),
  callbacks(callbacks),
  entrypanel(this),
  distspintext(&entrypanel, -1, _("Distance: ")),
  distspin(&entrypanel, -1, _("10")),
  capturebutton(&entrypanel, CAPTURE_BUTTON, _("Capture")),
  datalist(this, -1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT),
  buttonpanel(this),
  dropbutton(&buttonpanel, DROP_BUTTON, _("Drop")),
  applybutton(&buttonpanel, APPLY_BUTTON, _("Apply")),
  savebutton(&buttonpanel, SAVE_BUTTON, _("Save")),
  rvaluetext(&buttonpanel, -1, _("")) {
	wxBoxSizer *entrypanel_sizer = new wxBoxSizer(wxHORIZONTAL);
	entrypanel.SetSizer(entrypanel_sizer);
	entrypanel_sizer->Add(&distspintext, 0, wxALIGN_CENTER);
	entrypanel_sizer->Add(&distspin, 0, wxALIGN_CENTER);
	entrypanel_sizer->AddSpacer(20);
	entrypanel_sizer->Add(&capturebutton);

	datalist.InsertColumn(0, _("Dist"));
	datalist.InsertColumn(1, _("Laser 1"));
	datalist.InsertColumn(2, _("Laser 2"));
	datalist.InsertColumn(3, _("Laser 3"));

	wxBoxSizer *buttonpanel_sizer = new wxBoxSizer(wxHORIZONTAL);
	buttonpanel.SetSizer(buttonpanel_sizer);
	buttonpanel_sizer->Add(&dropbutton);
	buttonpanel_sizer->Add(&applybutton);
	buttonpanel_sizer->Add(&savebutton);
	buttonpanel_sizer->Add(&rvaluetext, 1, wxALIGN_CENTER);

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(sizer);
	sizer->Add(&entrypanel, 0, wxEXPAND);
	sizer->Add(&datalist, 1, wxEXPAND);
	sizer->Add(&buttonpanel, 0, wxEXPAND);
}

void LaserCalibratePanel::update(const LaserSensor::RawReadings &rawreadings) {
	if (rawreadings.size() == 0)
		return;

	int center = rawreadings[0].size()/2;

	curreadings.resize(rawreadings.size());
	for (int i=0; i<curreadings.size(); i++)
		curreadings[i] = rawreadings[i][center];
}

void LaserCalibratePanel::OnCaptureButton(wxCommandEvent &evt) {
	int dist = distspin.GetValue();

	int pos;
	for (pos=0; pos < datalist.GetItemCount(); pos++) {
		if (getDist(pos) > dist)
			break;
	}

	int id = datalist.InsertItem(pos, wxString() << distspin.GetValue());
	for (int i=0; i<curreadings.size(); i++)
		datalist.SetItem(id, 1+i, wxString() << curreadings[i]);
	datalist.EnsureVisible(id);

	int nextval = distspin.GetValue()+5;
	while (datalist.FindItem(-1, wxString() << nextval) != -1)
		nextval += 5;
	distspin.SetValue(nextval);

	updateRegression();
}

void LaserCalibratePanel::OnDropButton(wxCommandEvent &evt) {
	int id = datalist.GetNextItem(-1, wxLIST_NEXT_BELOW, wxLIST_STATE_SELECTED);
	if (id == -1)
		return;

	int val = getDist(id);
	if (distspin.GetValue() > val)
		distspin.SetValue(val);
	datalist.DeleteItem(id);
	datalist.SetItemState(id, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);

	updateRegression();
}

void LaserCalibratePanel::OnApplyButton(wxCommandEvent &evt) {
	vector<LaserSensor::Calibration> calibrations(regresults.size());

	for (int i=0; i<regresults.size(); i++) {
		calibrations[i].alpha = regresults[i].alpha;
		calibrations[i].beta = regresults[i].beta;
	}

	callbacks.onApplyCalibrations(calibrations);
}

void LaserCalibratePanel::OnSaveButton(wxCommandEvent &evt) {
	callbacks.onSaveCalibrations();
}

void LaserCalibratePanel::updateRegression() {
	regresults.resize(curreadings.size());

	for (int i=0; i < regresults.size(); i++) {
		vector<pair<float, float> > vals(datalist.GetItemCount());
		for (int pos=0; pos < datalist.GetItemCount(); pos++) {
			vals[pos] = make_pair(getReading(pos, i), 1.0f/getDist(pos));
		}

		regresults[i] = linearReg(vals);
	}

	wxString rvalues;
	for (int i=0; i < regresults.size(); i++) {
		float r = regresults[i].rvalue;
		rvalues << wxString::Format(_("%.2f "), r*r);
	}
	rvaluetext.SetLabel(rvalues);
}

LaserCalibratePanel::LinearRegResult LaserCalibratePanel::linearReg(const vector<pair<float, float> > &vals) {
	LinearRegResult result = {0, 0, 0};

	const int n = vals.size();
	if (n < 2)
		return result;

	float meanx=0;
	float meany=0;

	for (vector<pair<float, float> >::const_iterator i = vals.begin(); i != vals.end(); ++i) {
		meanx += i->first;
		meany += i->second;
	}
	meanx /= n;
	meany /= n;

	float atop=0;
	float abottom=0;
	float xstddev=0;
	float ystddev=0;

	for (vector<pair<float, float> >::const_iterator i = vals.begin(); i != vals.end(); ++i) {
		const float xdiff = i->first - meanx;
		const float ydiff = i->second - meany;

		atop += xdiff*ydiff;
		abottom += xdiff*xdiff;
		xstddev += xdiff*xdiff;
		ystddev += ydiff*ydiff;
	}

	result.alpha = atop / abottom;
	result.beta = meany - result.alpha*meanx;
	xstddev = sqrt(xstddev / n);
	ystddev = sqrt(ystddev / n);
	result.rvalue = result.alpha * xstddev / ystddev;

	return result;
}

int LaserCalibratePanel::getReading(int pos, int laser) {
	wxListItem item;
	item.SetId(pos);
	item.SetColumn(laser+1);
	item.SetMask(wxLIST_MASK_TEXT);
	datalist.GetItem(item);
	return wxAtoi(item.GetText());
}

int LaserCalibratePanel::getDist(int pos) {
	return wxAtoi(datalist.GetItemText(pos));
}


#ifndef DRIVETABPANEL_H
#define DRIVETABPANEL_H

#include "ieee/sim/comm/TabPanel.h"
#include "ieee/controls/DriveEquation.h"
#include "ieee/drivers/avr/AVRRobot.h"
#include <wx/wx.h>
#include <wx/spinctrl.h>

namespace ieee {
	class DriveTabPanel : public TabPanel {
		public:
			DriveTabPanel(wxWindow *parent, const DriveEquation::Config &driveequconfig);

			virtual char getTabCharacter() const; // TabPanel
			virtual void onSync(AVRRobot &robot); // TabPanel

		private:
			void update();

			wxStaticText xvellabel;
			wxSpinCtrl xvelspin;
			wxStaticText yvellabel;
			wxSpinCtrl yvelspin;
			wxPanel curdirpanel;
			wxStaticText curdirlabel;
			wxSpinCtrl curdirspin;
			wxStaticText angvellabel;
			wxSpinCtrl angvelspin;
			wxStaticText maxeffortlabel;
			wxSpinCtrl maxeffortspin;
			wxCheckBox enablecheck;
			wxCheckBox compasscheck;

			DriveEquation driveequ;
	};
}

#endif


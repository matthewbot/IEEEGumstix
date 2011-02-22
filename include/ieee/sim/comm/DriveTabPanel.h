#ifndef DRIVETABPANEL_H
#define DRIVETABPANEL_H

#include "ieee/sim/comm/TabPanel.h"
#include "ieee/controls/DriveEquation.h"
#include "ieee/drivers/avr/SerialPackets.h"
#include <wx/wx.h>
#include <wx/spinctrl.h>

namespace ieee {
	class DriveTabPanel : public TabPanel {
		public:
			DriveTabPanel(wxWindow *parent, TabPanel::Callbacks &callbacks);

			virtual char getTabCharacter() const; // TabPanel
			virtual void onNewAVRPacket(const AVRPacket &ap); // TabPanel
			virtual void updateGumstixPacket(GumstixPacket &gp, const WheelsDriver &wheelsdriver) const; // TabPanel

		private:
			void update();

			wxStaticText xvellabel;
			wxSpinCtrl xvelspin;
			wxStaticText yvellabel;
			wxSpinCtrl yvelspin;
			wxStaticText curdirlabel;
			wxSpinCtrl curdirspin;
			wxStaticText angvellabel;
			wxSpinCtrl angvelspin;
			wxStaticText maxeffortlabel;
			wxSpinCtrl maxeffortspin;
			wxCheckBox enablecheck;

			TabPanel::Callbacks &callbacks;

			struct DriveEquationConfig : DriveEquation::Config {
				DriveEquationConfig();
			};

			DriveEquationConfig equconf;
			DriveEquation driveequ;

			DECLARE_EVENT_TABLE()

			void OnSpin(wxSpinEvent &evt);
			void OnCommand(wxCommandEvent &evt);
	};
}

#endif


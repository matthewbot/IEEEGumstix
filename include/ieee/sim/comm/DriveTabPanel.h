#ifndef DRIVETABPANEL_H
#define DRIVETABPANEL_H

#include "ieee/controls/DriveEquation.h"
#include "ieee/drivers/SerialPackets.h"
#include <wx/wx.h>
#include <wx/spinctrl.h>

namespace ieee {
	class DriveTabPanel : public wxPanel {
		public:
			class Callbacks {
				public:
					virtual void onOutputChanged() = 0;
			};

			DriveTabPanel(wxWindow *parent, Callbacks &callbacks);

			void writeOutput(GumstixPacket &gp) const;

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

			Callbacks &callbacks;

			struct DriveEquationConfig : DriveEquation::Config {
				DriveEquationConfig();
			};

			DriveEquationConfig equconf;
			DriveEquation driveequ;

			DECLARE_EVENT_TABLE()

			void OnSpin(wxSpinEvent &evt);
			void OnCommand(wxCommandEvent &evt);
			static int16_t toRawAngle(float angle);
			static int16_t toRawSpeed(float speed, float angle);
	};
}

#endif


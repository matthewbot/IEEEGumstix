#ifndef IEEE_SENSORSTABPANEL_H
#define IEEE_SENSORSTABPANEL_H

#include "ieee/sim/comm/TabPanel.h"
#include "ieee/drivers/avr/SerialPackets.h"
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/spinctrl.h>

namespace ieee {
	class SensorsTabPanel : public TabPanel {
		public:
			SensorsTabPanel(wxWindow *parent, TabPanel::Callbacks &callbacks);

			virtual char getTabCharacter() const; // TabPanel
			virtual void onNewAVRPacket(const AVRPacket &ap); // TabPanel
			virtual void updateGumstixPacket(GumstixPacket &gp, const WheelsDriver &wheelsdriver) const; // TabPanel

		private:
			TabPanel::Callbacks &callbacks;
			wxListCtrl sensorlist;
			wxCheckBox freezecheck;
			wxSpinCtrl sonaranglespin;

			DECLARE_EVENT_TABLE()
			void OnSonarSpin(wxSpinEvent &evt);
			void OnSonarEnter(wxCommandEvent &evt);
	};
}

#endif

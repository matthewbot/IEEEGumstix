#ifndef IEEE_SENSORSTABPANEL_H
#define IEEE_SENSORSTABPANEL_H

#include "ieee/sim/comm/TabPanel.h"
#include "ieee/drivers/avr/AVRRobot.h"
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/spinctrl.h>

namespace ieee {
	class SensorsTabPanel : public TabPanel {
		public:
			SensorsTabPanel(wxWindow *parent);

			virtual char getTabCharacter() const; // TabPanel
			virtual void onSync(AVRRobot &robot); // TabPanel

		private:
			wxListCtrl sensorlist;
			wxCheckBox freezecheck;
			wxPanel sonaranglepanel;
			wxStaticText sonaranglelabel;
			wxSpinCtrl sonaranglespin;
			wxCheckBox compasssync;
	};
}

#endif

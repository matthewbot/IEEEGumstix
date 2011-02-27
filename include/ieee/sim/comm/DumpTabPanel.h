#ifndef IEEE_DUMPTABPANEL_H
#define IEEE_DUMPTABPANEL_H

#include "ieee/sim/comm/TabPanel.h"
#include "ieee/drivers/avr/AVRRobot.h"
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/spinctrl.h>

namespace ieee {
	class DumpTabPanel : public TabPanel {
		public:
			DumpTabPanel(wxWindow *parent);

			virtual char getTabCharacter() const; // TabPanel
			virtual void onSync(AVRRobot &robot); // TabPanel

		private:
			wxTextCtrl dumptext;

			wxPanel selectpanel;
			wxCheckBox enablecheck;
			wxCheckBox sonarcheck;
			wxCheckBox magcheck;
			wxButton clearbutton;
			wxButton savebutton;

			DECLARE_EVENT_TABLE()
			void OnClearButton(wxCommandEvent &evt);
			void OnSaveButton(wxCommandEvent &evt);
	};
}

#endif

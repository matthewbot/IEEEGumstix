#ifndef IEEE_WHEELTABPABEL_H
#define IEEE_WHEELTABPANEL_H

#include <wx/wx.h>
#include "ieee/sim/comm/WheelWidget.h"
#include "ieee/sim/comm/TabPanel.h"
#include "ieee/drivers/avr/AVRRobot.h"

namespace ieee {
	class WheelTabPanel : public TabPanel, WheelWidget::Callbacks {
		public:
			WheelTabPanel(wxWindow *parent);

			virtual char getTabCharacter() const; // TabPanel
			virtual void onSync(AVRRobot &robot); // TabPanel

		private:
			virtual void onWheelChanged(WheelWidget *wheel); // WheelWidget::Callbacks

			WheelWidget leftwidget, rightwidget, bottomwidget;
			wxPanel centerpanel;
			wxCheckBox enabledcheck;
			wxCheckBox syncanglecheck;
			wxCheckBox syncspeedcheck;
			wxCheckBox reversecheck;
			wxCheckBox raisecheck;

			void handleSyncChecks();

			DECLARE_EVENT_TABLE()
			void OnCheckEvent(wxCommandEvent &);
	};
}

#endif

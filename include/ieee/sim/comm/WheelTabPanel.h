#ifndef IEEE_WHEELTABPABEL_H
#define IEEE_WHEELTABPANEL_H

#include <wx/wx.h>
#include "ieee/sim/comm/WheelWidget.h"
#include "ieee/sim/comm/TabPanel.h"
#include "ieee/drivers/SerialPackets.h"

namespace ieee {
	class WheelTabPanel : public TabPanel, WheelWidget::Callbacks {
		public:
			WheelTabPanel(wxWindow *parent, TabPanel::Callbacks &callbacks);
			void writeWheelStates(GumstixPacket &gp) const;

			virtual char getTabCharacter() const; // TabPanel
			virtual void onNewAVRPacket(const AVRPacket &ap); // TabPanel
			virtual void updateGumstixPacket(GumstixPacket &gp, const WheelsDriver &wheelsdriver) const; // TabPanel

		private:
			virtual void onWheelChanged(WheelWidget *widget); // WheelWidget::Callbacks

			void update();
			static int16_t toRawAngle(float angle);
			static int16_t toRawSpeed(float speed, float angle);

			TabPanel::Callbacks &callbacks;

			WheelWidget leftwidget, rightwidget, bottomwidget;
			wxPanel centerpanel;
			wxCheckBox enabledcheck;
			wxCheckBox syncanglecheck;
			wxCheckBox syncspeedcheck;
			wxCheckBox reversecheck;
			wxCheckBox raisecheck;

			DECLARE_EVENT_TABLE()
			void OnCheckEvent(wxCommandEvent &);
	};
}

#endif

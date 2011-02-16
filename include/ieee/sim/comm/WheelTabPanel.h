#ifndef IEEE_WHEELTABPABEL_H
#define IEEE_WHEELTABPANEL_H

#include <wx/wx.h>
#include "ieee/sim/comm/WheelWidget.h"
#include "ieee/drivers/SerialPackets.h"

namespace ieee {
	class WheelTabPanel : public wxPanel, WheelWidget::Callbacks {
		public:
			class Callbacks {
				public:
					virtual void onWheelsMoved() = 0;
			};

			WheelTabPanel(wxWindow *parent, Callbacks &callbacks);
			void writeWheelStates(GumstixPacket &gp) const;

		private:
			virtual void onWheelChanged(WheelWidget *widget); // WheelWidget::Callbacks

			void update();
			static int16_t toRawAngle(float angle);
			static int16_t toRawSpeed(float speed, float angle);

			Callbacks &callbacks;

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

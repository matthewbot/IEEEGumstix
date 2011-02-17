#ifndef IEEE_SENSORSTABPANEL_H
#define IEEE_SENSORSTABPANEL_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "ieee/drivers/SerialPackets.h"

namespace ieee {
	class SensorsTabPanel : public wxPanel {
		public:
			SensorsTabPanel(wxWindow *parent);

			void readSensorData(const AVRPacket &avr);

		private:
			wxListCtrl sensorlist;
			wxCheckBox freezecheck;
	};
}

#endif

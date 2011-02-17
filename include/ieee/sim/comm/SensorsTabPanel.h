#ifndef IEEE_SENSORSTABPANEL_H
#define IEEE_SENSORSTABPANEL_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/spinctrl.h>
#include "ieee/drivers/SerialPackets.h"

namespace ieee {
	class SensorsTabPanel : public wxPanel {
		public:
			class Callbacks {
				public:
					virtual void onSonarAngleChanged() = 0;
			};

			SensorsTabPanel(wxWindow *parent, Callbacks &callbacks);

			void readSensorData(const AVRPacket &avr);
			void writeSonarAngle(GumstixPacket &avr) const;

		private:
			Callbacks &callbacks;
			wxListCtrl sensorlist;
			wxCheckBox freezecheck;
			wxSpinCtrl sonaranglespin;

			DECLARE_EVENT_TABLE()
			void OnSonarSpin(wxSpinEvent &evt);
			void OnSonarEnter(wxCommandEvent &evt);
	};
}

#endif

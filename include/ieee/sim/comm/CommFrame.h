#ifndef COMMFRAME_H
#define COMMFRAME_H

#include "ieee/sim/comm/CommWorkerThread.h"
#include "ieee/sim/comm/SensorsTabPanel.h"
#include "ieee/sim/comm/WheelTabPanel.h"
#include <boost/scoped_ptr.hpp>
#include <wx/wx.h>
#include <wx/notebook.h>
#include <stdint.h>

namespace ieee {
	class CommFrame : public wxFrame, CommWorkerThread::Callbacks, WheelTabPanel::Callbacks, SensorsTabPanel::Callbacks {
		public:
			CommFrame();

		private:
			virtual void onSync(); // CommWorkerThread::Callbacks
			virtual void onWheelsMoved(); // WheelTabPanel::Callbacks
			virtual void onSonarAngleChanged(); // SensorsTabPanel::Callbacks

			void updatePacket();

			wxNotebook notebook;
			WheelTabPanel *wheelpanel; // wxNotebook insists on deleting these
			SensorsTabPanel *sensorspanel;

			CommWorkerThread thread;

			DECLARE_EVENT_TABLE()
			void OnSyncEvent(wxCommandEvent &);
			void OnCheckEvent(wxCommandEvent &);
	};
}

#endif

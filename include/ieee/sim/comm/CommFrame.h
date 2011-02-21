#ifndef COMMFRAME_H
#define COMMFRAME_H

#include "ieee/sim/comm/CommWorkerThread.h"
#include "ieee/sim/comm/SensorsTabPanel.h"
#include "ieee/sim/comm/WheelTabPanel.h"
#include "ieee/sim/comm/DriveTabPanel.h"
#include <boost/scoped_ptr.hpp>
#include <wx/wx.h>
#include <wx/notebook.h>
#include <stdint.h>
#include <vector>

namespace ieee {
	class CommFrame : public wxFrame, CommWorkerThread::Callbacks, TabPanel::Callbacks {
		public:
			CommFrame();

		private:
			virtual void onSync(); // CommWorkerThread::Callbacks
			virtual void onTabUpdated(TabPanel *tp); // TabPanel::Callbacks

			void updatePacket();

			wxNotebook notebook;
			typedef std::vector<TabPanel *> TabPanelVec;
			TabPanelVec panels;

			struct WheelsDriverConfig : WheelsDriver::Config {
				WheelsDriverConfig();
			};

			WheelsDriverConfig wheelsdriverconf;
			WheelsDriver wheelsdriver;
			CommWorkerThread thread;

			DECLARE_EVENT_TABLE()
			void OnSyncEvent(wxCommandEvent &);
			void OnCheckEvent(wxCommandEvent &);
	};
}

#endif

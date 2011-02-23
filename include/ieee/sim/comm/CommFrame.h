#ifndef COMMFRAME_H
#define COMMFRAME_H

#include "ieee/sim/comm/SensorsTabPanel.h"
#include "ieee/sim/comm/WheelTabPanel.h"
#include "ieee/sim/comm/DriveTabPanel.h"
#include <boost/scoped_ptr.hpp>
#include <wx/wx.h>
#include <wx/notebook.h>
#include <stdint.h>
#include <vector>

namespace ieee {
	class CommFrame : public wxFrame {
		public:
			CommFrame();

		private:
			wxTimer synctimer;
			wxNotebook notebook;
			typedef std::vector<TabPanel *> TabPanelVec;
			TabPanelVec panels;

			struct RobotConfig : AVRRobot::Config {
				RobotConfig();
			};

			RobotConfig robotconfig;
			AVRRobot robot;

			DECLARE_EVENT_TABLE()
			void OnSyncEvent(wxTimerEvent &);
	};
}

#endif

#ifndef CONTROLTESTFRAME
#define CONTROLTESTFRAME

#include <wx/wx.h>
#include "ieee/sim/control/PositionControllerLayer.h"
#include "ieee/sim/shared/WorldPanel.h"
#include "ieee/sim/shared/GridPanelLayer.h"
#include "ieee/controls/PositionController.h"
#include "ieee/drivers/avr/AVRRobot.h"
#include "ieee/shared/WorldGrid.h"
#include <boost/scoped_ptr.hpp>

namespace ieee {
	class ControlTestFrame : public wxFrame {
		public:
			ControlTestFrame();

		private:
			struct PositionControllerConfig : PositionController::Config {
				PositionControllerConfig();
			};

			PositionControllerConfig poscontrolconfig;
			PositionController poscontrol;

			struct AVRRobotConfig : AVRRobot::Config {
				AVRRobotConfig();
			};

			AVRRobotConfig robotconfig;
			boost::scoped_ptr<AVRRobot> robotptr;

			WorldGrid grid;
			CoordScale gridscale;

			WorldPanel panel;
			GridPanelLayer gridlayer;
			PositionControllerLayer posconlayer;

			wxTimer synctimer;

			DECLARE_EVENT_TABLE()
			void OnSyncEvent(wxTimerEvent &evt);
	};
}

#endif

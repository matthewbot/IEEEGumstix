#ifndef CONTROLTESTFRAME
#define CONTROLTESTFRAME

#include <wx/wx.h>
#include "ieee/sim/control/PositionControllerLayer.h"
#include "ieee/sim/shared/WorldPanel.h"
#include "ieee/sim/shared/GridPanelLayer.h"
#include "ieee/sim/shared/ConfigLoader.h"
#include "ieee/controls/PositionController.h"
#include "ieee/drivers/avr/AVRRobot.h"
#include "ieee/shared/WorldGrid.h"
#include <boost/scoped_ptr.hpp>

namespace ieee {
	class ControlTestFrame : public wxFrame, PositionControllerLayer::Callbacks {
		public:
			ControlTestFrame();

		private:
			ConfigLoader configloader;
			PositionController poscontrol;
			boost::scoped_ptr<AVRRobot> robotptr;

			WorldGrid grid;
			CoordScale gridscale;

			WorldPanel panel;
			GridPanelLayer gridlayer;
			PositionControllerLayer posconlayer;

			wxPanel optionspanel;
			wxButton stopbutton;
			wxButton resetbutton;
			wxStaticText statetext;

			wxTimer synctimer;

			virtual void onCommand(const Coord &coord, float dir); // PositionControllerLayer::Callbacks

			DECLARE_EVENT_TABLE()
			void OnStopEvent(wxCommandEvent &evt);
			void OnResetEvent(wxCommandEvent &evt);
			void OnSyncEvent(wxTimerEvent &evt);
	};
}

#endif

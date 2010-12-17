#ifndef SIMFRAME_H
#define SIMFRAME_H

#include <wx/wx.h>
#include "ieee/sim/WorldPanel.h"
#include "ieee/sim/GridPanelLayer.h"
#include "ieee/sim/ObjectPanelLayer.h"
#include "ieee/sim/RobotPanelLayer.h"

namespace ieee {
	class RoomSimFrame : public wxFrame, ObjectPanelLayer::Callbacks {
		public:
			RoomSimFrame();

			// WorldPanel::Callbacks
			virtual bool onWorldClicked(const Pos &pos);
			virtual void onWorldDragged(const Pos &pos);
			virtual bool isVictimIdentified(const Pos &pos);

		private:
			struct SimWorld : World {
				SimWorld();
			};

			SimWorld world;
			Robot robot;

			GridPanelLayer gridlayer;
			ObjectPanelLayer objectlayer;
			RobotPanelLayer robotlayer;
			WorldPanel worldpanel;

			World::iterator selectedobj;
			int selectedid;

			wxPanel buttonpanel;
			wxButton stepbutton;
			wxButton resetbutton;

			void onStepPressed(wxCommandEvent &event);
			void onResetPressed(wxCommandEvent &event);

			DECLARE_EVENT_TABLE()
	};
}

#endif

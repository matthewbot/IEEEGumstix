#ifndef SIMFRAME_H
#define SIMFRAME_H

#include <wx/wx.h>
#include "WorldPanel.h"

namespace ieeepath {
	class SimFrame : public wxFrame, public WorldPanel::Callbacks {
		public:
			SimFrame(World &world, Robot &robot);
			
			// WorldPanel::Callbacks
			virtual bool onWorldClicked(const Pos &pos);
			virtual void onWorldDragged(const Pos &pos);
			
		private:
			World &world;
			Robot &robot;
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

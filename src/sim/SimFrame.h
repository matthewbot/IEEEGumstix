#ifndef SIMFRAME_H
#define SIMFRAME_H

#include <wx/wx.h>
#include "WorldPanel.h"

namespace pathsim {
	class SimFrame : public wxFrame, public WorldPanel::Callbacks {
		public:
			SimFrame(const World &world, Robot &robot);
			
			// WorldPanel::Callbacks
			virtual bool onWorldClicked(const Pos &pos);
			virtual void onWorldDragged(const Pos &pos);
			
		private:
			Robot &robot;
			WorldPanel worldpanel;
			wxPanel buttonpanel;
			wxButton stepbutton;
			wxButton resetbutton;
			
			void onStepPressed(wxCommandEvent &event);
			void onResetPressed(wxCommandEvent &event);
			
			DECLARE_EVENT_TABLE()
	};
}

#endif

#ifndef SIMFRAME_H
#define SIMFRAME_H

#include <wx/wx.h>
#include "WorldPanel.h"

namespace pathsim {
	class SimFrame : public wxFrame {
		public:
			SimFrame(const World &world, Robot &robot);
			
		private:
			Robot &robot;
			WorldPanel worldpanel;
			wxButton stepbutton;
			
			void onStepPressed(wxCommandEvent &event);
			
			DECLARE_EVENT_TABLE()
	};
}

#endif

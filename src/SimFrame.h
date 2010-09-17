#ifndef SIMFRAME_H
#define SIMFRAME_H

#include <wx/wx.h>
#include "WorldPanel.h"

namespace pathsim {
	class SimFrame : public wxFrame {
		public:
			SimFrame(const World &world);
			
		private:
			WorldPanel worldpanel;
	};
}

#endif

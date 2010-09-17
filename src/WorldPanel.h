#ifndef WORLDCANVAS_H
#define WORLDCANVAS_H

#include <wx/wx.h>
#include "World.h"

namespace pathsim {
	class WorldPanel : public wxPanel {
		public:
			WorldPanel(wxWindow *parent, const World &world);
			
		private:
			void OnPaint(wxPaintEvent& event);
		
			const World &world;
			
			DECLARE_EVENT_TABLE()
	};
}

#endif

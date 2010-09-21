#ifndef WORLDCANVAS_H
#define WORLDCANVAS_H

#include <wx/wx.h>
#include "World.h"
#include "Robot.h"

namespace pathsim {
	class WorldPanel : public wxPanel {
		public:
			WorldPanel(wxWindow *parent, const World &world, const Robot &robot);
			
		private:
			void OnPaint(wxPaintEvent &event);
			void paintGrid(wxPaintDC &dc);
			void paintObjects(wxPaintDC &dc);
			void paintRobot(wxPaintDC &dc);
		
			const Robot &robot;
			const World &world;
			
			DECLARE_EVENT_TABLE()
	};
}

#endif

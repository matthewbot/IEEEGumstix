#ifndef WORLDCANVAS_H
#define WORLDCANVAS_H

#include "ieeepath/sim/Robot.h"
#include "ieeepath/sim/World.h"
#include <wx/wx.h>

namespace ieee {
	class WorldPanel : public wxPanel {
		public:
			struct Callbacks {
				public:
					virtual bool onWorldClicked(const Pos &pos) =0; // return true to begin dragging
					virtual void onWorldDragged(const Pos &pos) =0;
			};
		
			WorldPanel(wxWindow *parent, Callbacks &callbacks, const World &world, const Robot &robot);
			
		private:		
			Callbacks &callbacks;
			const Robot &robot;
			const World &world;
		
			void OnPaint(wxPaintEvent &event);
			void paintGrid(wxPaintDC &dc);
			void paintObjects(wxPaintDC &dc);
			void paintRobot(wxPaintDC &dc);
		
			void OnLeftDown(wxMouseEvent &event);
			void OnLeftUp(wxMouseEvent &event);
			void OnMotion(wxMouseEvent &event);
			bool dragging;
			Pos lastdragpos;
			
			DECLARE_EVENT_TABLE()
	};
}

#endif

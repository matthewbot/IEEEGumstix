#ifndef ROOMSIMAPP_H
#define ROOMSIMAPP_H

#include <wx/wx.h>
#include "SimFrame.h"
#include "World.h"
#include "Robot.h"

namespace ieee {
	class RoomSimApp : public wxApp {
		public:
			RoomSimApp();
			
		private:
			virtual bool OnInit();
		
			struct SimWorld : World {
				SimWorld();
			};
		
			SimFrame *frame;
			SimWorld world;
			Robot robot;
	};
}

#endif

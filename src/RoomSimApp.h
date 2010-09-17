#ifndef ROOMSIMAPP_H
#define ROOMSIMAPP_H

#include <wx/wx.h>
#include "SimFrame.h"
#include "World.h"

namespace pathsim {
	class RoomSimApp : public wxApp {
		public:
			RoomSimApp();
			
		private:
			virtual bool OnInit();
			virtual int OnExit();
		
			SimFrame *frame;
			World world;
	};
}

#endif

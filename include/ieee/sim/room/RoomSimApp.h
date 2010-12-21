#ifndef ROOMSIMAPP_H
#define ROOMSIMAPP_H

#include <wx/wx.h>
#include "ieee/sim/room/Robot.h"
#include "ieee/sim/room/RoomSimFrame.h"
#include "ieee/sim/shared/World.h"

namespace ieee {
	class RoomSimApp : public wxApp {
		public:
			RoomSimApp();

		private:
			virtual bool OnInit();
	};
}

#endif

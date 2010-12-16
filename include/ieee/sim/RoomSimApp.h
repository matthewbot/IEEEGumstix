#ifndef ROOMSIMAPP_H
#define ROOMSIMAPP_H

#include <wx/wx.h>
#include "RoomSimFrame.h"
#include "World.h"
#include "Robot.h"

namespace ieee {
	class RoomSimApp : public wxApp {
		public:
			RoomSimApp();

		private:
			virtual bool OnInit();

			RoomSimFrame *frame;
	};
}

#endif

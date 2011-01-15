#ifndef ROOMSIMAPP_H
#define ROOMSIMAPP_H

#include <wx/wx.h>

namespace ieee {
	class RoomSimApp : public wxApp {
		public:
			RoomSimApp();

		private:
			virtual bool OnInit();
	};
}

#endif

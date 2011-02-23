#ifndef TABPANEL_H
#define TABPANEL_H

#include "ieee/drivers/avr/AVRRobot.h"
#include <wx/wx.h>

namespace ieee {
	class TabPanel : public wxPanel {
		public:
			inline TabPanel(wxWindow *parent) : wxPanel(parent, -1) { }

			virtual char getTabCharacter() const = 0;
			virtual void onSync(AVRRobot &robot) = 0;
	};
}

#endif

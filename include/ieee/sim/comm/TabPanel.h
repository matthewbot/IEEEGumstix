#ifndef TABPANEL_H
#define TABPANEL_H

#include "ieee/drivers/WheelsDriver.h"
#include <wx/wx.h>

namespace ieee {
	class TabPanel : public wxPanel {
		public:
			inline TabPanel(wxWindow *parent) : wxPanel(parent, -1) { }

			class Callbacks {
				public:
					virtual void onTabUpdated(TabPanel *tp) = 0;
			};

			virtual char getTabCharacter() const = 0;
			virtual void onNewAVRPacket(const AVRPacket &ap) = 0;
			virtual void updateGumstixPacket(GumstixPacket &gp, const WheelsDriver &wheelsdriver) const = 0;
	};
}

#endif

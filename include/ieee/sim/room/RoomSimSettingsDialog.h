#ifndef ROOMSIMSETTINGSDIALOG_H
#define ROOMSIMSETTINGSDIALOG_H

#include <wx/wx.h>
#include "ieee/planner/RoomPlanner.h"

namespace ieee {
	class RoomSimSettingsDialog : public wxDialog {
		public:
			struct Settings {
				int gridwidth, gridheight;
			};

			RoomSimSettingsDialog(wxWindow *parent, Settings &settings);

			bool show();

		private:
			Settings &settings;
			wxPanel buttonpanel;
			wxButton okbutton;
			wxButton cancelbutton;

			wxPanel fieldpanel;
			wxStaticText gridsizelabel;
			wxPanel gridsizepanel;
			wxTextCtrl gridwidthtext;
			wxStaticText gridlabelx;
			wxTextCtrl gridheighttext;
	};
}

#endif


#ifndef CONTROLTESTAPP_H
#define CONTROLTESTAPP_H

#include <wx/wx.h>

namespace ieee {
	class ControlTestApp : public wxApp {
		public:
			ControlTestApp();

		private:
			virtual bool OnInit();
	};
}

#endif


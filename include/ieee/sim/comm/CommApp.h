#ifndef COMMAPP_H
#define COMMAPP_H

#include <wx/wx.h>

namespace ieee {
	class CommApp : public wxApp {
		public:
			CommApp();

		private:
			virtual bool OnInit();
	};
}

#endif

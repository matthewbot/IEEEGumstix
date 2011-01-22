#ifndef LASERSIMAPP_H
#define LASERSIMAPP_H

#include <wx/wx.h>

namespace ieee {
	class LaserSimApp : public wxApp {
		public:
			LaserSimApp();

		private:
			virtual bool OnInit();
	};
}

#endif

#ifndef COMMFRAME_H
#define COMMFRAME_H

#include "ieee/sim/comm/CommWorkerThread.h"
#include "ieee/sim/comm/WheelWidget.h"
#include <wx/wx.h>

namespace ieee {
	class CommFrame : public wxFrame, CommWorkerThread::Callbacks {
		public:
			CommFrame();

		private:
			virtual void onSync(); // CommWorkerThread::Callbacks

			WheelWidget leftwidget, rightwidget, bottomwidget;
			CommWorkerThread thread;

			DECLARE_EVENT_TABLE()
			void OnSyncEvent(wxCommandEvent &);
	};
}

#endif

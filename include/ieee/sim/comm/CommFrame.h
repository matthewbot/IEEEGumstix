#ifndef COMMFRAME_H
#define COMMFRAME_H

#include "ieee/sim/comm/CommWorkerThread.h"
#include "ieee/sim/comm/WheelWidget.h"
#include <wx/wx.h>
#include <stdint.h>

namespace ieee {
	class CommFrame : public wxFrame, CommWorkerThread::Callbacks, WheelWidget::Callbacks {
		public:
			CommFrame();

		private:
			virtual void onSync(); // CommWorkerThread::Callbacks

			virtual void onWheelChanged(WheelWidget *widget); // WheelWidget::Callbacks

			void updatePacket();
			static int16_t toRawAngle(float angle);
			static int16_t toRawSpeed(float speed);

			WheelWidget leftwidget, rightwidget, bottomwidget;
			wxPanel centerpanel;
			wxCheckBox enabledcheck;

			CommWorkerThread thread;

			DECLARE_EVENT_TABLE()
			void OnSyncEvent(wxCommandEvent &);
			void OnEnabledCheckEvent(wxCommandEvent &);
	};
}

#endif

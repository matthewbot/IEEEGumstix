#ifndef LASERSIMFRAME_H
#define LASERSIMFRAME_H

#include "ieee/drivers/LaserPlot.h"
#include "ieee/sim/laser/LaserSimWorkerThread.h"
#include "ieee/sim/laser/LaserPanelLayer.h"
#include "ieee/sim/shared/WorldPanel.h"
#include "ieee/sim/shared/GridPanelLayer.h"
#include "ieee/shared/WorldGrid.h"
#include <wx/wx.h>

namespace ieee {
	class LaserSimFrame : public wxFrame, LaserSimWorkerThread::Callbacks {
		public:
			LaserSimFrame();
			~LaserSimFrame();

		private:
			LaserSimWorkerThread thread;
			WorldGrid grid;
			LaserSensor::Readings readings;

			CoordScale gridscale;
			GridPanelLayer gridlayer;
			LaserPanelLayer laserlayer;
			WorldPanel worldpanel;

			mutable wxCriticalSection grid_critsect;

			struct LaserPlotConfig : LaserPlot::Config {
				LaserPlotConfig();
			};
			LaserPlotConfig laserplotconfig;

			DECLARE_EVENT_TABLE()

			virtual void onNewLaserData(const LaserSensor::Readings &readings);
			void OnWorldGridUpdateEvent(wxCommandEvent &evt);
	};
}

#endif

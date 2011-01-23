#ifndef LASERSIMFRAME_H
#define LASERSIMFRAME_H

#include "ieee/drivers/LaserPlot.h"
#include "ieee/sim/laser/LaserSimWorkerThread.h"
#include "ieee/sim/laser/LaserPanelLayer.h"
#include "ieee/sim/laser/LaserImagePanel.h"
#include "ieee/sim/shared/GridPanelLayer.h"
#include "ieee/sim/shared/WorldPanel.h"
#include "ieee/sim/shared/ImagePanel.h"
#include "ieee/shared/WorldGrid.h"
#include <wx/wx.h>
#include <wx/notebook.h>

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
			WorldPanel *gridworldpanel; // pointers because wxNotebook insists on calling delete on these

			LaserImagePanel *laserimagepanel;

			ImagePanel *rawimagepanel;

			wxNotebook notebook;

			struct LaserPlotConfig : LaserPlot::Config {
				LaserPlotConfig();
			};
			LaserPlotConfig laserplotconfig;

			DECLARE_EVENT_TABLE()

			virtual void onNewLaserData();
			void OnWorldGridUpdateEvent(wxCommandEvent &evt);
	};
}

#endif

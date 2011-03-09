#ifndef LASERSIMFRAME_H
#define LASERSIMFRAME_H

#include "ieee/drivers/laser/LaserPlot.h"
#include "ieee/sim/laser/LaserSimWorkerThread.h"
#include "ieee/sim/laser/LaserPanelLayer.h"
#include "ieee/sim/laser/LaserImagePanel.h"
#include "ieee/sim/laser/LaserCalibratePanel.h"
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
			CoordScale gridscale;
			LaserSensor::Readings readings;

			wxStaticText rawreadingtext;
			wxNotebook notebook;

			WorldPanel *gridworldpanel; // pointers because wxNotebook insists on calling delete on these.. I have yet to think of anything better :/
			GridPanelLayer gridlayer;
			LaserPanelLayer laserlayer;

			LaserImagePanel *laserimagepanel;
			ImagePanel *greenimagepanel;
			ImagePanel *rawimagepanel;
			LaserCalibratePanel *calibratepanel;

			struct LaserPlotConfig : LaserPlot::Config {
				LaserPlotConfig();
			};
			LaserPlotConfig laserplotconfig;

			DECLARE_EVENT_TABLE()

			virtual void onNewLaserData();
			void OnWorldGridUpdateEvent(wxCommandEvent &evt);
			void OnPageChangeEvent(wxNotebookEvent &evt);
	};
}

#endif

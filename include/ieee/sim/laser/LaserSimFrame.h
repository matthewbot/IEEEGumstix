#ifndef LASERSIMFRAME_H
#define LASERSIMFRAME_H

#include "ieee/drivers/laser/LaserMapper.h"
#include "ieee/sim/laser/LaserPanelLayer.h"
#include "ieee/sim/laser/LaserImagePanel.h"
#include "ieee/sim/laser/LaserCalibratePanel.h"
#include "ieee/sim/shared/GridPanelLayer.h"
#include "ieee/sim/shared/WorldPanel.h"
#include "ieee/sim/shared/ImagePanel.h"
#include "ieee/sim/shared/ConfigLoader.h"
#include "ieee/shared/WorldGrid.h"
#include <wx/wx.h>
#include <wx/notebook.h>

namespace ieee {
	class LaserSimFrame : public wxFrame, LaserMapper::Callbacks, LaserCalibratePanel::Callbacks {
		public:
			LaserSimFrame();

		private:
			ConfigLoader configloader;

			WorldGrid grid;
			CoordScale gridscale;
			LaserMapper lasermapper;
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

			DECLARE_EVENT_TABLE()

			virtual void onNewLaserData(); // LaserSimWorkerThread::Callbacks
			virtual void onApplyCalibrations(const std::vector<LaserSensor::Calibration> &calibrations); // LaserCalibratePanel::Callbacks
			virtual void onSaveCalibrations();

			void OnWorldGridUpdateEvent(wxCommandEvent &evt);
			void OnPageChangeEvent(wxNotebookEvent &evt);
	};
}

#endif

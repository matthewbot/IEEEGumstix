#ifndef LASERCALIBRATEPANEL_H
#define LASERCALIBRATEPANEL_H

#include "ieee/drivers/laser/LaserSensor.h"
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <vector>
#include <utility>

namespace ieee {
	class LaserCalibratePanel : public wxPanel {
		public:
			struct Callbacks {
				virtual void onApplyCalibrations(const std::vector<LaserSensor::Calibration> &calibrations) =0;
				virtual void onSaveCalibrations() =0;
			};

			LaserCalibratePanel(wxWindow *parent, Callbacks &callbacks);

			void update(const LaserSensor::RawReadings &rawreadings);

		private:
			Callbacks &callbacks;

			wxPanel entrypanel;
			wxStaticText distspintext;
			wxSpinCtrl distspin;
			wxButton capturebutton;

			wxPanel buttonpanel;
			wxButton dropbutton;
			wxButton applybutton;
			wxButton savebutton;
			wxStaticText rvaluetext;

			wxListCtrl datalist;

			DECLARE_EVENT_TABLE()
			void OnCaptureButton(wxCommandEvent &evt);
			void OnDropButton(wxCommandEvent &evt);
			void OnApplyButton(wxCommandEvent &evt);
			void OnSaveButton(wxCommandEvent &evt);

			std::vector<int> curreadings;

			struct LinearRegResult {
				float alpha, beta, rvalue;
			};
			std::vector<LinearRegResult> regresults;
			void updateRegression();
			static LinearRegResult linearReg(const std::vector<std::pair<float, float> > &vals);

			int getDist(int pos);
			int getReading(int pos, int laser);
	};
}

#endif


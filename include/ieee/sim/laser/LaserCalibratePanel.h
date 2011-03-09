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
			LaserCalibratePanel(wxWindow *parent, std::vector<LaserSensor::Calibration> &configcalibrations);

			void update(const LaserSensor::RawReadings &rawreadings);

		private:
			std::vector<LaserSensor::Calibration> &configcalibrations;

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


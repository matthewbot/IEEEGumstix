#ifndef LASERIMAGEPANEL_H
#define LASERIMAGEPANEL_H

#include "ieee/sim/shared/ImagePanel.h"
#include "ieee/drivers/LaserSensor.h"

namespace ieee {
	class LaserImagePanel : public ImagePanel {
		public:
			LaserImagePanel(wxWindow *parent);

			void update(const cv::Mat &frame, const LaserSensor::RawReadings &readings);

		protected:
			void OnPaint(wxPaintEvent &event);

		private:
			LaserSensor::RawReadings readings;

			DECLARE_EVENT_TABLE()
	};
}

#endif
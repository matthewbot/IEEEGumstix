#ifndef LASERSENSOR_H
#define LASERSENSOR_H

#include "ieee/drivers/V4LCapture.h"
#include "ieee/drivers/LaserTrack.h"
#include "ieee/shared/types.h"

namespace cv {
	class Mat; // predeclare to avoid pulling in cv/cv.h
};

namespace ieee {
	class LaserSensor {
		public:
			struct Debug {
				cv::Mat rawframe;
				cv::Mat greenframe;
			};

			struct Config : LaserTrack::Config {
				inline Config() : debug(NULL) { }

				int exposure;
				Debug *debug;
			};

			typedef LaserTrack::LineVec RawReadings;

			LaserSensor(const Config &config, const std::string &devname="");

			RawReadings captureRawReadings();

			inline void setExposure(int exposure) { cap.setExposure(exposure); }

		private:
			const Config &config;
			V4LCapture cap;
	};
}

#endif

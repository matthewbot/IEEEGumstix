#ifndef LASERSENSOR_H
#define LASERSENSOR_H

#include "ieee/drivers/V4LCapture.h"
#include "ieee/drivers/LaserTrack.h"
#include "ieee/shared/types.h"
#include <vector>

namespace cv {
	class Mat; // predeclare to avoid pulling in cv/cv.h
};

namespace ieee {
	class LaserSensor {
		public:
			typedef LaserTrack::LineVec RawReadings;

			struct DistAngle {
				float dist; // in cm
				float angle;

				inline DistAngle() { }
				inline DistAngle(float dist, float angle) : dist(dist), angle(angle) { }
			};

			typedef std::vector<DistAngle> DistAngleVec;
			typedef std::vector<DistAngleVec> Readings;

			struct Debug {
				cv::Mat rawframe;
				cv::Mat greenframe;
				RawReadings rawreadings;
			};

			struct Calibration {
				float alpha;
				float beta;
				float offset;
			};

			struct Config : LaserTrack::Config {
				inline Config() : debug(NULL) { }

				const Calibration *calibrations;
				int exposure;
				float viewangle; // horizontal, in radians

				Debug *debug;
			};

			LaserSensor(const Config &config, const std::string &devname="");

			RawReadings captureRawReadings();
			Readings captureReadings();

			inline void setExposure(int exposure) { cap.setExposure(exposure); }

		private:
			const Config &config;
			V4LCapture cap;

			DistAngle toDistAngle(int row, int col, int maxcol, int laser) const;
	};
}

#endif

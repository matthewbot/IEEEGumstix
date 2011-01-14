#ifndef LASERSENSOR_H
#define LASERSENSOR_H

#include "ieee/drivers/V4LCapture.h"
#include "ieee/shared/types.h"
#include <boost/array.hpp>
#include <utility>

namespace cv {
	class Mat; // predeclare to avoid pulling in cv/cv.h
};

namespace ieee {
	class LaserSensorDebug; // predeclare to avoid pulling in cv/cv.h

	class LaserSensor {
		public:
			struct Config {
				int gmult;
				int brmult;

				int segthresh;
				int minsegheight;
			};

			typedef std::vector<int> Readings;

			LaserSensor(V4LCapture &cap, const Config &config);

			Readings getReadings(LaserSensorDebug *debug=NULL);

		private:
			V4LCapture &cap;
			const Config &config;

			int scanCol(const cv::Mat &frame, int col) const;
			int pixVal(const uchar *pix) const;
			inline bool checkPix(const uchar *pix) const { return pixVal(pix) > config.segthresh; }
	};
}

#endif

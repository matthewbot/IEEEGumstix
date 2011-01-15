#ifndef LASERSENSOR_H
#define LASERSENSOR_H

#include "ieee/drivers/V4LCapture.h"
#include "ieee/drivers/LaserTrack.h"
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
			struct Config : LaserTrack::Config {
			};

			typedef LaserTrack::LineVec Readings;

			LaserSensor(V4LCapture &cap, const Config &config);

			Readings getReadings(LaserSensorDebug *debug=NULL);

		private:
			V4LCapture &cap;
			const Config &config;
	};
}

#endif

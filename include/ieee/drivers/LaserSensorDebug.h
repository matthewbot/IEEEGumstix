#ifndef LASERSENSORDEBUG_H
#define LASERSENSORDEBUG_H

#include <opencv/cv.h>

namespace ieee {
	struct LaserSensorDebug {
		cv::Mat rawframe;
		cv::Mat greenframe;
	};
}

#endif

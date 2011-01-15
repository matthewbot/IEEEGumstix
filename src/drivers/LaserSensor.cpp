#include "ieee/drivers/LaserSensor.h"
#include <opencv/cv.h>
#include <vector>

using namespace ieee;
using namespace cv;
using namespace std;

LaserSensor::LaserSensor(const Config &config, const std::string &devname)
: config(config), cap(320, 240, devname, config.exposure) { }

LaserSensor::RawReadings LaserSensor::captureRawReadings() {
	Mat frame;
	cap.readFrame(frame);

	LaserTrack track(config, frame);

	if (config.debug) {
		config.debug->rawframe = frame;
		config.debug->greenframe = track.generateGreenChannel();
	}

	return track.getLineVec();
}


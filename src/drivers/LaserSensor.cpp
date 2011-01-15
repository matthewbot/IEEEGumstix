#include "ieee/drivers/LaserSensor.h"
#include "ieee/drivers/LaserSensorDebug.h"
#include <opencv/cv.h>
#include <vector>

using namespace ieee;
using namespace cv;
using namespace std;

LaserSensor::LaserSensor(V4LCapture &cap, const Config &config)
: cap(cap), config(config) { }

LaserSensor::Readings LaserSensor::getReadings(LaserSensorDebug *debug) {
	Mat frame;
	cap.readFrame(frame);

	LaserTrack track(config, frame);

	if (debug) {
		debug->rawframe = frame;
		debug->greenframe = track.generateGreenChannel();
	}

	return track.getLineVec();
}


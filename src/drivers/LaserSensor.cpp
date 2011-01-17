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

LaserSensor::Readings LaserSensor::captureReadings() {
	RawReadings rawreadings = captureRawReadings();

	Readings readings(config.maxpoints);
	for (int laser=0; laser<config.maxpoints; laser++) {
		const LaserTrack::LineData &line = rawreadings[laser];
		for (int col=0; col < line.size(); col++) {
			int row = line[col];
			if (row == -1)
				continue;

			readings[laser].push_back(toDistAngle(row, col, line.size(), laser));
		}
	}

	return readings;
}

LaserSensor::DistAngle LaserSensor::toDistAngle(int row, int col, int maxcol, int laser) const {
	const Calibration &cal = config.calibrations[laser];
	float angle = (col - maxcol/2) * (config.viewangle / maxcol);
	float dist = 1/(cos(angle)*(cal.alpha*row + cal.beta)) - cal.offset;
	return DistAngle(dist, angle);
}


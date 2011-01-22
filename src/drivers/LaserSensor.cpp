#include "ieee/drivers/LaserSensor.h"
#include <opencv/cv.h>
#include <vector>

using namespace ieee;
using namespace cv;
using namespace std;

Coord LaserSensor::DistAngle::toCoord(float rotate) const {
	float a = angle + rotate;
	return Coord(dist*cos(a), dist*sin(a));
}

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

auto_ptr<LaserSensor> LaserSensor::createAndHandleExposureFailure(Config &config, const std::string &devname) {
	auto_ptr<LaserSensor> sensorptr;

	try {
		sensorptr = auto_ptr<LaserSensor>(new LaserSensor(config));
	} catch (runtime_error &err) {
		if (string(err.what()).find("exposure") != string::npos) {
			config.exposure = -1;
			sensorptr = auto_ptr<LaserSensor>(new LaserSensor(config));
		} else
			throw;
	}

	return sensorptr;
}



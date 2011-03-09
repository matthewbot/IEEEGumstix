#include "ieee/drivers/laser/LaserSensor.h"
#include <vector>

using namespace ieee;
using namespace boost::property_tree;
using namespace std;

Coord LaserSensor::DistAngle::toCoord(float rotate) const {
	float a = angle + rotate;
	return Coord(dist*cos(a), dist*sin(a));
}

LaserSensor::LaserSensor(const Config &config, const std::string &devname)
: config(config), cap(320, 240, devname, config.exposure) { }

LaserSensor::RawReadings LaserSensor::captureRawReadings(Debug *debug) {
	Image frame;
	cap.readFrame(frame);

	LaserTrack track(config.lasertrack, frame);

	if (debug) {
		debug->rawframe = frame;
		debug->greenframe = track.generateGreenChannel();
		debug->rawreadings = track.getLineVec();
	}

	return track.getLineVec();
}

LaserSensor::Readings LaserSensor::captureReadings(Debug *debug) {
	RawReadings rawreadings = captureRawReadings(debug);

	Readings readings(rawreadings.size());
	for (int laser=0; laser<rawreadings.size(); laser++) {
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
	float dist = 1/(cos(angle)*(cal.alpha*row + cal.beta));
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

void LaserSensor::Config::readTree(const ptree &pt) {
	lasertrack.readTree(pt.get_child("lasertrack"));

	calibrations.resize(lasertrack.maxpoints);
	for (int i=0; i<lasertrack.maxpoints;i++) {
		stringstream propbuf;
		propbuf << "calibration_" << i;

		stringstream valbuf(pt.get<string>(propbuf.str()));
		valbuf >> calibrations[i].alpha >> calibrations[i].beta;
	}

	exposure = pt.get<int>("exposure");
	viewangle = pt.get<float>("viewangle_deg") / 180 * M_PI;
}


#include "ieee/drivers/laser/LaserMapper.h"
#include "ieee/shared/Timer.h"
#include <boost/bind.hpp>

using namespace ieee;
using namespace boost;

LaserMapper::LaserMapper(Config &config, Callbacks &callbacks, int gridwidth, int gridheight, const CoordScale &gridscale)
: config(config),
  callbacks(callbacks),
  gridscale(gridscale),
  laserptr(LaserSensor::createAndHandleExposureFailure(config.sensor)),
  mapgrid(gridwidth, gridheight, WorldGrid::UNKNOWN),
  curpos(0, 0),
  curdir(0),
  stopflag(false),
  debugflag(false),
  plotflag(true),
  thread(bind(&LaserMapper::run, this)) { }

LaserMapper::~LaserMapper() {
	stopflag = true;
	thread.join();
}

void LaserMapper::updateState(const Coord &curpos, float curdir) {
	lock_guard<boost::mutex> guard(mutex);
	this->curpos = curpos;
	this->curdir = curdir;
}

WorldGrid LaserMapper::getMapGrid() const {
	lock_guard<boost::mutex> guard(mutex);
	return mapgrid;
}

void LaserMapper::clearMapGrid() {
	lock_guard<boost::mutex> guard(mutex);
	mapgrid.clear(WorldGrid::UNKNOWN);
}

LaserSensor::Readings LaserMapper::getLaserReadings() const {
	lock_guard<boost::mutex> guard(mutex);
	return laserreadings;
}

LaserSensor::Debug LaserMapper::getLaserDebug() const {
	lock_guard<boost::mutex> guard(mutex);
	return laserdebug;
}

void LaserMapper::run() {
	LaserSensor::Readings laserreadings;
	LaserSensor::Debug laserdebug;

	while (!stopflag) {
		Timer tim;

		if (debugflag)
			laserreadings = laserptr->captureReadings(&laserdebug);
		else {
			laserreadings = laserptr->captureReadings();
			laserdebug = LaserSensor::Debug();
		}

		updatetime = tim.getMilliseconds();

		{
			lock_guard<boost::mutex> guard(mutex);
			this->laserreadings = laserreadings;
			this->laserdebug = laserdebug;

			LaserPlot plot(config.plot, laserreadings, curpos, curdir, mapgrid, gridscale);
		}

		callbacks.onNewLaserData();
	}
}

void LaserMapper::Config::readTree(const boost::property_tree::ptree &pt) {
	plot.readTree(pt.get_child("plot"));
	sensor.readTree(pt.get_child("sensor"));
}


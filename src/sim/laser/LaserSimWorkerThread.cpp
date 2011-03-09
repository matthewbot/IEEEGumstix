#include "ieee/sim/laser/LaserSimWorkerThread.h"
#include "ieee/shared/Timer.h"
#include <boost/assign.hpp>

using namespace ieee;
using namespace boost::assign;

LaserSimWorkerThread::LaserConfig::LaserConfig() {
	lasertrack.minval = 30;
	lasertrack.maxpoints = 3;
	lasertrack.lasersep = 10;

	calibrations += LaserSensor::Calibration(2.4545e-4, 9.6096e-03), LaserSensor::Calibration(1.9022e-4, 2.1028e-03), LaserSensor::Calibration(3.6926e-4, 9.3072e-03);
	exposure = 4000;
	viewangle = 55.7 / 180 * M_PI;
};

LaserSimWorkerThread::LaserSimWorkerThread(Callbacks &callbacks)
: wxThread(wxTHREAD_JOINABLE),
  callbacks(callbacks),
  laserconfig(),
  laserptr(LaserSensor::createAndHandleExposureFailure(laserconfig)),
  stopflag(false),
  debugflag(true) {
	Create();
}

void LaserSimWorkerThread::start() {
	Run();
}

void LaserSimWorkerThread::stop() {
	stopflag = true;
	Wait();
}

LaserSensor::Readings LaserSimWorkerThread::getLaserReadings() const {
	LaserSensor::Readings retval;
	wxCriticalSectionLocker locker(critsect);
	retval = laserreadings;
	return retval;
}

LaserSensor::Debug LaserSimWorkerThread::getLaserDebug() const {
	LaserSensor::Debug retval;
	wxCriticalSectionLocker locker(critsect);
	retval = laserdebug;
	return retval;
}

wxThread::ExitCode LaserSimWorkerThread::Entry() {
	LaserSensor::Debug debug;
	LaserSensor::Readings readings;

	while (true) {
		if (stopflag)
			return 0;

		Timer tim;

		if (debugflag)
			readings = laserptr->captureReadings(&debug);
		else {
			readings = laserptr->captureReadings();
			debug = LaserSensor::Debug();
		}

		capturetime = tim.getMilliseconds();

		{
			wxCriticalSectionLocker locker(critsect);
			laserreadings = readings;
			laserdebug = debug;
		}

		callbacks.onNewLaserData();
	}
}


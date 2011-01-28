#include "ieee/sim/laser/LaserSimWorkerThread.h"

using namespace ieee;

LaserSimWorkerThread::LaserConfig::LaserConfig() {
	brmult = -1;
	gmult = 2;
	minval = 30;
	maxpoints = 3;
	lasersep = 10;
	exposure = 4000;

	static const LaserSensor::Calibration calibrations_array[] = {
		{ 2.4545e-4, 9.6096e-03, 0},
		{ 1.9022e-4, 2.1028e-03, 0},
		{ 3.6926e-4, 9.3072e-03, 5}
	};

	calibrations = calibrations_array;
	viewangle = 55.7 / 180 * M_PI;
};

LaserSimWorkerThread::LaserSimWorkerThread(Callbacks &callbacks)
: wxThread(wxTHREAD_JOINABLE),
  callbacks(callbacks),
  laserconfig(),
  laserptr(LaserSensor::createAndHandleExposureFailure(laserconfig)),
  stopflag(false) {
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
	while (true) {
		if (stopflag)
			return 0;

		LaserSensor::Debug debug;
		LaserSensor::Readings readings = laserptr->captureReadings(&debug);

		{
			wxCriticalSectionLocker locker(critsect);
			laserreadings = readings;
			laserdebug = debug;
		}

		callbacks.onNewLaserData();
	}
}


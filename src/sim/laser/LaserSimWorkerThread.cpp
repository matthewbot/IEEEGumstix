#include "ieee/sim/laser/LaserSimWorkerThread.h"
#include "ieee/shared/Timer.h"
#include <boost/assign.hpp>

using namespace ieee;
using namespace boost::assign;

LaserSimWorkerThread::LaserSimWorkerThread(Callbacks &callbacks, LaserSensor::Config &sensorconfig)
: wxThread(wxTHREAD_JOINABLE),
  callbacks(callbacks),
  laserptr(LaserSensor::createAndHandleExposureFailure(sensorconfig)),
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


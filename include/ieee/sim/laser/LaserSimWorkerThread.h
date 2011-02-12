#ifndef LASERSIMWORKERTHREAD_H
#define LASERSIMWORKERTHREAD_H

#include "ieee/drivers/LaserSensor.h"
#include <wx/wx.h>
#include <memory>

namespace ieee {
	class LaserSimWorkerThread : wxThread {
		public:
			class Callbacks {
				public:
					virtual void onNewLaserData() = 0;
			};

			LaserSimWorkerThread(Callbacks &callbacks);

			void start();
			void stop();

			LaserSensor::Readings getLaserReadings() const;
			LaserSensor::Debug getLaserDebug() const;
			inline float getCaptureTime() const { return capturetime; } // in milliseconds
		private:
			virtual ExitCode Entry();

			Callbacks &callbacks;
			LaserSensor::Readings laserreadings;
			LaserSensor::Debug laserdebug;
			mutable wxCriticalSection critsect;
			float capturetime;

			struct LaserConfig : LaserSensor::Config {
				LaserConfig();
			};
			LaserConfig laserconfig;
			const std::auto_ptr<LaserSensor> laserptr;

			volatile bool stopflag;
	};
}

#endif


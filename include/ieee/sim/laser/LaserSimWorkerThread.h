#ifndef LASERSIMWORKERTHREAD_H
#define LASERSIMWORKERTHREAD_H

#include "ieee/drivers/laser/LaserSensor.h"
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

			inline void setDebugFlag(bool flag) { debugflag = flag; }
			LaserSensor::Readings getLaserReadings() const;
			LaserSensor::Debug getLaserDebug() const;
			inline float getCaptureTime() const { return capturetime; } // in milliseconds
			inline LaserSensor::Config &getConfig() { return laserconfig; }

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
			volatile bool debugflag;
	};
}

#endif


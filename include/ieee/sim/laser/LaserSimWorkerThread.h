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
					virtual void onNewLaserData(const LaserSensor::Readings &readings) = 0;
			};

			LaserSimWorkerThread(Callbacks &callbacks);

			void start();
			void stop();

		private:
			virtual ExitCode Entry();

			Callbacks &callbacks;

			struct LaserConfig : LaserSensor::Config {
				LaserConfig();
			};

			LaserConfig laserconfig;
			const std::auto_ptr<LaserSensor> laserptr;
			volatile bool stopflag;
	};
}

#endif


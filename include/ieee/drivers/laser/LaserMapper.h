#ifndef LASERMAPPER_H
#define LASERMAPPER_H

#include "ieee/drivers/laser/LaserPlot.h"
#include "ieee/drivers/laser/LaserSensor.h"
#include "ieee/shared/WorldGrid.h"
#include <boost/thread.hpp>

namespace ieee {
	class LaserMapper {
		public:
			struct Config {
				LaserPlot::Config plot;
				LaserSensor::Config sensor;

				void readTree(const boost::property_tree::ptree &pt);
			};

			struct Callbacks {
				virtual void onNewLaserData() =0;
			};

			LaserMapper(Config &config, Callbacks &callbacks, int gridwidth, int gridheight, const CoordScale &gridscale);
			~LaserMapper();

			void updateState(const Coord &curpos, float curdir);
			WorldGrid getMapGrid() const;
			void clearMapGrid();

			inline void setFlags(bool debug, bool plot) { debugflag = debug; plotflag = plot; }
			LaserSensor::Readings getLaserReadings() const;
			LaserSensor::Debug getLaserDebug() const;
			inline float getUpdateTime() const { return updatetime; } // in milliseconds

		private:
			void run();

			const Config &config;
			Callbacks &callbacks;
			const CoordScale &gridscale;

			const std::auto_ptr<LaserSensor> laserptr;

			WorldGrid mapgrid;
			Coord curpos;
			float curdir;

			mutable boost::mutex mutex;
			LaserSensor::Readings laserreadings;
			LaserSensor::Debug laserdebug;

			volatile float updatetime;
			volatile bool stopflag;
			volatile bool debugflag;
			volatile bool plotflag;
			boost::thread thread;
	};
}

#endif

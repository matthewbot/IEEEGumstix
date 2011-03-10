#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include "ieee/drivers/avr/AVRRobot.h"
#include "ieee/drivers/laser/LaserMapper.h" // this isn't great because now all simulator binaries depend on all our libraries..
#include "ieee/controls/PositionController.h"
#include <boost/filesystem.hpp>

namespace ieee {
	class ConfigLoader {
		public:
			static const boost::filesystem::path systemdefaultdir; // path comes from build system based on where we're being installed

			ConfigLoader(const boost::filesystem::path &basedir = systemdefaultdir);

			inline AVRRobot::Config &getAVRRobotConfig() { return avrrobotconfig; }
			inline PositionController::Config &getPositionControllerConfig() { return poscontrolconfig; }
			inline LaserMapper::Config &getLaserConfig() { return laserconfig; }

			void saveLaserConfig();

		private:
			boost::filesystem::path basedir;
			AVRRobot::Config avrrobotconfig;
			PositionController::Config poscontrolconfig;
			LaserMapper::Config laserconfig;
			boost::property_tree::ptree laserconfigpt;
	};
}

#endif

#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include "ieee/drivers/avr/AVRRobot.h"
#include "ieee/controls/PositionController.h"
#include <boost/filesystem.hpp>

namespace ieee {
	class ConfigLoader {
		public:
			static const boost::filesystem::path systemdefaultdir; // path comes from build system based on where we're being installed

			ConfigLoader(const boost::filesystem::path &basedir = systemdefaultdir);

			inline const AVRRobot::Config &getAVRRobotConfig() const { return avrrobotconfig; }
			inline const PositionController::Config &getPositionControllerConfig() const { return poscontrolconfig; }

		private:
			boost::filesystem::path basedir;
			AVRRobot::Config avrrobotconfig;
			PositionController::Config poscontrolconfig;
	};
}

#endif

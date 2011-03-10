#include "ieee/sim/shared/ConfigLoader.h"
#include <boost/filesystem/fstream.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <sstream>

using namespace ieee;
namespace fs = boost::filesystem;
using namespace boost::property_tree::info_parser;
using namespace boost::property_tree;
using namespace std;

template <typename Config> ptree loadConfig(Config &config, boost::filesystem::path filename) {
	ptree pt;
	fs::ifstream in(filename);

	read_info(in, pt);
	config.readTree(pt);
	return pt;
}

ConfigLoader::ConfigLoader(const fs::path &basedir) : basedir(basedir) {
	loadConfig(avrrobotconfig, basedir / "robot.info");
	loadConfig(poscontrolconfig, basedir / "control.info");
	laserconfigpt = loadConfig(laserconfig, basedir / "laser.info");
}

void ConfigLoader::saveLaserConfig() {
	// for now we just save calibration data, ugly

	for (int i=0; i<laserconfig.sensor.track.maxpoints; i++) {
		stringstream propbuf;
		propbuf << "sensor.calibration_" << i;
		stringstream valuebuf;
		valuebuf << laserconfig.sensor.calibrations[i].alpha << " " << laserconfig.sensor.calibrations[i].beta;
		laserconfigpt.put(propbuf.str(), valuebuf.str());
	}

	fs::ofstream out(basedir / "laser.info");
	write_info(out, laserconfigpt);
}

const fs::path ConfigLoader::systemdefaultdir(IEEE_CONFIG_DIR);


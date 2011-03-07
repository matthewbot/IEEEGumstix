#include "ieee/sim/shared/ConfigLoader.h"
#include <boost/filesystem/fstream.hpp>
#include <boost/property_tree/info_parser.hpp>

using namespace ieee;
namespace fs = boost::filesystem;
using namespace boost::property_tree::info_parser;
using namespace boost::property_tree;

ConfigLoader::ConfigLoader(const fs::path &basedir) : basedir(basedir) {
	ptree pt;

	fs::ifstream robotin(basedir / "robot.info");
	read_info(robotin, pt);
	avrrobotconfig.readTree(pt);

	fs::ifstream controlin(basedir / "control.info");
	read_info(controlin, pt);
	poscontrolconfig.readTree(pt);
}

const fs::path ConfigLoader::systemdefaultdir(IEEE_CONFIG_DIR);


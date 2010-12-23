#include "ieee/sim/shared/WorldIO.h"
#include "ieee/sim/shared/VictimWorldObject.h"
#include "ieee/sim/shared/ObstacleWorldObject.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>

using namespace std;
using namespace ieee;
using namespace boost;
using namespace boost::algorithm;

void ieee::readWorldObjects(istream &in, World &world) {
	while (!in.eof()) {
		string line;
		getline(in, line);

		vector<string> parts;
		split(parts, line, is_space());

		if (parts.size() == 0 || parts[0].size() == 0)
			continue;

		WorldObject *obj;

		if (iequals(parts[0], "victim")) {
			if (parts.size() != 3)
				throw runtime_error("Expected victim entry to have 3 parts");

			Pos pos(lexical_cast<int>(parts[1]), lexical_cast<int>(parts[2]));
			obj = new VictimWorldObject(pos);
		} else if (iequals(parts[0], "obstacle")) {
			if (parts.size() != 6)
				throw runtime_error("Expected obstacle entry to have 6 parts");

			bool large = iequals(parts[1], "large");
			Pos startpos(lexical_cast<int>(parts[2]), lexical_cast<int>(parts[3]));
			Pos endpos(lexical_cast<int>(parts[4]), lexical_cast<int>(parts[5]));
			obj = new ObstacleWorldObject(startpos, endpos, large);
		} else {
			throw runtime_error("Unexpected type '" + parts[0] + "' in readWorldObjects");
		}

		world.add(obj);
	}
}

void ieee::writeWorldObjects(ostream &out, const World &world) {
	for (World::const_iterator i = world.begin(); i != world.end(); ++i) {
		if (const VictimWorldObject *victim = dynamic_cast<const VictimWorldObject *>(&*i)) {
			out << "victim ";
			out << victim->getPos().x << " " << victim->getPos().y << endl;
		} else if (const ObstacleWorldObject *obstacle = dynamic_cast<const ObstacleWorldObject *>(&*i)) {
			out << "obstacle " << (obstacle->isLarge() ? "large " : "small ");
			out << obstacle->getStartPos().x << " " << obstacle->getStartPos().y << " ";
			out << obstacle->getEndPos().x << " " << obstacle->getEndPos().y << endl;
		} else {
			throw runtime_error("Unknown WorldObject encountered in writeWorldObjects");
		}
	}
}


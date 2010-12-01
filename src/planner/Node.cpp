#include "ieeepath/planner/Node.h"

using namespace ieee;
using namespace std;

Node::Node(Type type, Dir dir) : type(type), dir(dir) { }

bool Node::traverseableFrom(Dir travdir) const {
	if (type == OPEN)
		return true;
	else if (type == IMPASSABLE || type == UNKNOWN)
		return false;
	else
		return dir == travdir || dir == oppositeDir(travdir);
}

std::ostream &ieee::operator<<(std::ostream &out, const Node &node) {
	static const char typechars[] = " CI-";
	out << typechars[node.getType()];
	return out;
}


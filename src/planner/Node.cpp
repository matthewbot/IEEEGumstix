#include "ieeepath/planner/Node.h"

using namespace ieeepath;
using namespace std;

Node::Node(Type type, Dir dir) : type(type), dir(dir) { }

bool Node::traverseableFrom(Dir travdir) const {
	if (type == OPEN || type == VICTIM_ID)
		return true;
	else if (type == CLOSED)
		return false;
	else
		return dir == travdir || dir == oppositeDir(travdir);	
}


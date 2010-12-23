#ifndef WORLDIO_H
#define WORLDIO_H

#include "ieee/sim/shared/World.h"
#include <istream>
#include <ostream>

namespace ieee {
	void readWorldObjects(std::istream &in, World &world);
	void writeWorldObjects(std::ostream &out, const World &world);
}

#endif

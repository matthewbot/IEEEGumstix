#include "ieee/controls/Angle.h"
#include <cmath>

using namespace ieee;
using namespace std;

void Angle::normalize() {
	while (rad < 0) { rad += M_PI*2; }
	while (rad >= M_PI*2) { rad -= M_PI*2; }
}

Angle Angle::operator+(const Angle &angle) const {
	return Angle(rad + angle.rad);
}

Angle &Angle::operator+=(const Angle &angle) {
	rad += angle.rad;
	normalize();
}

Angle Angle::operator-(const Angle &angle) const {
	return Angle(rad - angle.rad);
}

Angle Angle::operator-() const {
	return Angle(-rad);
}

Angle &Angle::operator-=(const Angle &angle) {
	rad -= angle.rad;
	normalize();
}

std::ostream &ieee::operator<<(std::ostream &out, const Angle &angle) {
	out << angle.getDegrees();
	return out;
}


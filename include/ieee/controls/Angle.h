#ifndef ANGLE_H
#define ANGLE_H

#include <cmath>
#include <ostream>

namespace ieee {
	class Angle {
		public:
			inline Angle() { }
			explicit inline Angle(float rad) : rad(rad) { normalize(); }
			static inline Angle fromDegrees(float degrees) { return Angle(degrees / 180 * M_PI); }

			inline float getRad() const { return rad; }
			inline float getDistFromZero() const { if (rad < M_PI) { return rad; } else { return rad - 2*M_PI; } }
			inline float getDegrees() const { return rad/M_PI*180; }
			void normalize();

			Angle operator+(const Angle &angle) const;
			Angle &operator+=(const Angle &angle);
			Angle operator-(const Angle &angle) const;
			Angle &operator-=(const Angle &angle);
			Angle operator-() const;

			inline float sin() const { return std::sin(rad); }
			inline float cos() const { return std::cos(rad); }

		private:
			float rad;
	};

	std::ostream &operator<<(std::ostream &os, const Angle &angle);
};

#endif

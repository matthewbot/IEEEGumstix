#ifndef VEC2D_H
#define VEC2D_H

#include <cmath>

namespace ieee {
	struct Vec2D {
		float x, y;

		inline Vec2D() { }
		inline Vec2D(float x, float y) : x(x), y(y) { }

		inline float magnitude() const { return sqrt(x*x + y*y); }
		inline float angle() const { return atan2(y, x); }

		inline Vec2D operator+(const Vec2D &vec) const { return Vec2D(x + vec.x, y + vec.y); }
		inline Vec2D operator-(const Vec2D &vec) const { return Vec2D(x - vec.x, y - vec.y); }
		inline Vec2D operator*(float val) const { return Vec2D(x * val, y * val); }
		inline Vec2D operator-() const { return Vec2D(-x, -y); }

		inline Vec2D &operator+=(const Vec2D &vec) { x += vec.x; y += vec.y; }
		inline Vec2D &operator-=(const Vec2D &vec) { x -= vec.x; y -= vec.y; }
		inline Vec2D &operator*=(float val) { x *= val; y *= val; }

		inline float dot(const Vec2D &vec) const { return x * vec.x + y * vec.y; }

		inline Vec2D rotate(float angle) const {
			float c = cos(angle), s = sin(angle);
			return Vec2D(x*c+y*s, y*c-x*s);
		}
	};
}

#endif

#ifndef TIMER_H
#define TIMER_H

#include <ctime>

namespace ieee {
	class Timer {
		public:
			Timer();

            void reset();

			float getSeconds() const;
			inline float getMilliseconds() const { return getSeconds() * 1000; }

		private:
			timespec start;
	};
}

#endif

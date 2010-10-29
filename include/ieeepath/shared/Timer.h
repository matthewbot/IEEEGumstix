#ifndef TIMER_H
#define TIMER_H

#include <ctime>

namespace ieeepath {
	class Timer {
		public:
			Timer();
			
			float getSeconds() const;
			
		private:
			timespec start;
	};
}

#endif

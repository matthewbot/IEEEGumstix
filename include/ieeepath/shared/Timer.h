#ifndef TIMER_H
#define TIMER_H

#include <ctime>

namespace ieee {
	class Timer {
		public:
			Timer();
			
			float getSeconds() const;
			
		private:
			timespec start;
	};
}

#endif

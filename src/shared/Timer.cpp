#include "ieee/shared/Timer.h"

using namespace ieee;

Timer::Timer() {
	reset();
}

void Timer::reset() {
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
}

float Timer::getSeconds() const {
	timespec cur;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cur);

	return cur.tv_sec - start.tv_sec + (cur.tv_nsec - start.tv_nsec)/1E9;
}


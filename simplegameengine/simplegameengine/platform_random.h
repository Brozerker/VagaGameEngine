#ifdef _WIN32
#include <time.h>
#define _CLOCK_ clock
#else
#include <sys/time.h>	// wall-clock timer (as opposed to clock cycle timer)
/** In Linux, clock() returns CPU cycles, not wall-clock time. */
inline __time_t __platform_Clock_NIX() {
	static timeval g_startTime = { 0, 0 };
	if (g_startTime.tv_sec == 0 && g_startTime.tv_usec == 0)
		gettimeofday(&g_startTime, NULL);	// start the timer
	timeval now;
	__time_t seconds, useconds, ms;
	gettimeofday(&now, NULL);
	seconds = now.tv_sec - g_startTime.tv_sec;
	useconds = now.tv_usec - g_startTime.tv_usec;
	ms = (seconds * 1000) + (useconds / 1000);
	return (__time_t)ms;
}
#define _CLOCK_	__platform_Clock_NIX
#endif

/**
* @return relatively random bit sequence (on a fast CPU). Use sparingly!
* Expected runtime is about (a_numBits*2)+1 milliseconds (a little less),
* during which 100% of CPU is used
*/
inline unsigned long platform_randomIntTRNG(int a_numBits) {
	unsigned long result = 0, loops1st, loops2nd;
	int index = 0;
	time_t now = _CLOCK_();
	while (_CLOCK_() == now); // start timing at the turn of the  millisecond
	for (index = 0; index < a_numBits; ++index) {
		for (now = _CLOCK_(), loops1st = 0; _CLOCK_() == now; ++loops1st);
		for (now = _CLOCK_(), loops2nd = 0; _CLOCK_() == now; ++loops2nd);
		result |= (((int)(loops2nd < loops1st)) ^ (loops1st & 0x1)) << index;
	}
	return result;
}

inline unsigned long * __platform_RandomSeed() {
	static unsigned long g_seed = 5223;
	return &g_seed;
}

inline void platform_randomSeed(unsigned long a_seed) { *__platform_RandomSeed() = a_seed; }

inline unsigned long random() {
	// Take the current seed and generate a new value 
	// from it. Due to our use of large constants and 
	// overflow, it would be very hard for someone to
	// predict what the next number is going to be from 
	// the previous one.
	//g_seed = (8253729 * g_seed + 2396403);
	platform_randomSeed(8253729 * (*__platform_RandomSeed()) + 2396403);

	// return a value between 0 and 2.14 billion (+2^^31)
	return (*__platform_RandomSeed()) & 0x7fffffff;
}

inline int platform_randomInt() { return (int)random(); }

inline float platform_randomFloat() {
	unsigned long i = random() & 0xffffff;
	return i / (float)(0x1000000);
}

inline float platform_randomFloat(float min, float max) {
	float delta = max - min;
	float number = platform_randomFloat() * delta;
	number += min;
	return number;
}

#undef _CLOCK_
#include "vector_.h"
#include <functional> // C++11 library that allows functors and lambdas

struct TimerNote {
	long long when; // in milliseconds
	std::function<void()> what;
};

class TimerQueue {
private:
	Vector_<TimerNote> queue;
	long long currentTime;
public:
	TimerQueue() : currentTime(0){}
	long long GetTime() { return currentTime; }
	void Update(long long msPassed) {
		currentTime += msPassed;
		for (int i = 0; i < queue.Count(); ++i) {
			if (queue[i].when <= currentTime) {
				queue[i].what();
				queue.RemoveAt(i);
				i--;
			}
		}
	}
	void Add(long long delay, std::function<void()> what){
		TimerNote note = { currentTime + delay, what };
		queue.Add(note);
	}
};
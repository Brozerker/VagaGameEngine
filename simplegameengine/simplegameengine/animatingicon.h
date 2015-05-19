#pragma once

#include "icon.h"
#include "printable.h"

class AnimatingIcon : public Printable {
public:
	struct Frame {
		Icon icon;
		int time;
		Frame(Icon icon, int time) : icon(icon), time(time){}
	};
private:
	Frame * frames;
	int count;
	int index, timer;
public:
	AnimatingIcon() : frames(nullptr), count(0), index(0), timer(0) {}
	AnimatingIcon(Frame * frames, const int count) : frames(frames), count(count), index(0), timer(0) {}
	void SetFrames(Frame * frames, const int count) { this->frames = frames; this->count = count; }
	void Print() { frames[index].icon.Print(); }
	void Update(int ms) {
		timer += ms;
		if (timer >= frames[index].time) {
			timer -= frames[index].time;
			index++;
			if (index >= count){
				index = 0;
			}
		}
	}
};
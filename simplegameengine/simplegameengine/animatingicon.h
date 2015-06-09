#pragma once

#include "printable.h"
#include "map2d.h"

class AnimatingIcon : public Printable {
public:
	struct Frame {
		Pixel icon;
		int time;
		Frame(Pixel icon, int time) : icon(icon), time(time){}
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
	void Print(Map2D * graphicsContext, Vec2i offset){
		graphicsContext->Set(offset.y, offset.x, frames[index].icon);
	}
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
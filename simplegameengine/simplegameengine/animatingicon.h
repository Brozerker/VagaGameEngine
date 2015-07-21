#pragma once

#include "printable.h"
#include "map2d.h"
#include "pixel.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

class AnimatingIcon : public Printable {
public:
	struct Frame {
		Pixel icon;
		int time;
		Frame(Pixel icon, int time) : icon(icon), time(time){}
		Frame() : icon('?'), time(0) {}
	};
private:
	Frame * frames;
	int count;
	int index, timer;
public:
	AnimatingIcon * Clone() {
		AnimatingIcon * ai = new AnimatingIcon;
		ai->frames = frames;
		ai->count = count;
		ai->index = index;
		ai->timer = timer;
		return ai;
	}
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
	std::string NextToken(std::ifstream & stream) {
		std::string result = "";
		bool isComment;
		do {
			stream >> result;
			isComment = result[0] == '/' && result[1] == '/';
			if (isComment) {
				char c;
				do{
					c = stream.get();
				} while (c != '\n');
			}
		} while (isComment);
		return result;
	}
	int NextInt(std::ifstream & stream) {
		return atoi(NextToken(stream).c_str());
	}
	void LoadFile(const char * filename) {
		std::ifstream fin;
		std::cout << "doing stuff..." << std::endl;
		fin.open(filename);
		count = NextInt(fin);
		frames = new Frame[count];
		for (int i = 0; i < count; ++i){
			frames[i].icon.c = NextInt(fin);
			frames[i].icon.fcolor = NextInt(fin);
			frames[i].icon.bcolor = NextInt(fin);
			frames[i].time = NextInt(fin);
		}
		fin.close();
	}
	AnimatingIcon(const char * filename) : frames(nullptr), count(0), index(0), timer(0) {
		LoadFile(filename);
	}
};
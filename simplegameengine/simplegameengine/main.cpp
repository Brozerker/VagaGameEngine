#include "platform_conio.h"
#include <stdlib.h>
#include "vector_.h"
#include "gameplay.h"
// when making your own data structures:
// 1) make it without templates or const correctness, just to make it.
// 2) once it works just like you want, then add const templates/correctness


typedef Array_<int> Arrayi; // compiler level - typesafe (will generate more errors, a good thing)
// #define Arrayi Array_<int> // preprocessor level - less constrained (more dangerous)


// (game engine)
//	gameplay (Gameplay)
//			world (Rect)
//				size (Vec2i)
//				icon (Icon)
//			player (Entity)
//				location (Vec2i)
//				icon (Icon)
//			random key to hit (char)
//	draw
//	user input
//	update

class Printable {
public:
	virtual void Print() = 0;
	virtual ~Printable(){}
};

// classes hide complexity with encapsulation
class Icon : public Printable {
private:
	char icon, fcolor, bcolor;
public:
	Icon(char c, int foreColor, int backColor) : icon(c), fcolor(foreColor), bcolor(backColor) {}
	void Print() { platform_setColor(fcolor, bcolor); putchar(icon); }
	Icon & operator=(char c){ icon = c; return *this; }
	Icon(char c) : fcolor(7), bcolor(0) { operator=(c); }
};

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

// public by default. don't support polymorphism! structs should be simple: should not require encapsulation.

class Rect {
public:
	Icon icon;
	Vec2i position, size;
	Rect(Icon icon, Vec2i position, Vec2i size) : icon(icon), position(position), size(size) {}
	void Draw(){
		for (int row = 0; row < size.y; ++row){
			for (int col = 0; col < size.x; ++col){
				platform_move(row, col);
				icon.Print();
			}
		}
	}
};

static AnimatingIcon::Frame playerIconFrames[] = {
	AnimatingIcon::Frame(Icon(2, 8, 0), 150),
	AnimatingIcon::Frame(Icon(2, 7, 0), 50),
	AnimatingIcon::Frame(Icon(2, 15, 0), 100),
	AnimatingIcon::Frame(Icon(2, 7, 0), 50),
};


// TODO http://patorjk.com/software/taag/#p=display&f=Univers&t=AaBbCcDdEeFfGgHhIiJj%0AKkLlMmNnOoPpQqRrSsTt%0AUuVvWwXxYyZz%5B%5D%7B%7D%5C%7C%2F-%2B%3D%0A012345678!%40%23%24%25%5E%26*()_


#include <ctime>
#include "platform_random.h"

void printInt(int num) {
	for (int i = 31; i >= 0; --i){
		putchar(((num&(1 << i)) != 0) ? '1' : '0');
	}
}

int main() {
	srand((unsigned int)time(NULL));
	Gameplay g;
	clock_t now, previous = clock(), deltaTime, soon;
	int FPS = 30;
	while (g.IsGameRunning()) { // game loop
		// timing code
		now = clock();
		deltaTime = now - previous;
		previous = now;

		// game loop code
		g.Draw();
		if (platform_kbhit()) {
			g.UserInput(platform_getch());
		}
		g.Update(deltaTime);

		// throttle code
		soon = now + (1000 / FPS);
		while (platform_upTimeMS() < soon && !platform_kbhit()) { Sleep(1); }
	}
	return 0;
}

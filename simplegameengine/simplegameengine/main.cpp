#include "platform_conio.h"
#include <stdlib.h>
<<<<<<< HEAD
#include "vector_.h"
#include "gameplay.h"
=======

<<<<<<< HEAD

#include "printable.h"
#include "rect.h"
#include "icon.h"
#include "animatingicon.h"


>>>>>>> origin/master
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


// public by default. don't support polymorphism! structs should be simple: should not require encapsulation.
<<<<<<< HEAD

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
=======
struct Vec2i {
	int x, y;
	Vec2i() : x(0), y(0) {}
	Vec2i(int x, int y) :x(x), y(y) {}
};
class Entity {
	bool iconAnimates;
public:
	Printable * icon;
	Vec2i position;
	Entity(Printable * icon, Vec2i position) : icon(icon), position(position) {
		iconAnimates = dynamic_cast<AnimatingIcon*>(icon) != nullptr;
	}
	void Draw() {
		platform_move(position.y, position.x); icon->Print();
	}
	void Update(int ms){
		if (iconAnimates) {
			((AnimatingIcon*)icon)->Update(ms);
		}
	}
};

>>>>>>> origin/master

static AnimatingIcon::Frame playerIconFrames[] = {
	AnimatingIcon::Frame(Icon(2, 8, 0), 150),
	AnimatingIcon::Frame(Icon(2, 7, 0), 50),
	AnimatingIcon::Frame(Icon(2, 15, 0), 100),
	AnimatingIcon::Frame(Icon(2, 7, 0), 50),
};


// TODO http://patorjk.com/software/taag/#p=display&f=Univers&t=AaBbCcDdEeFfGgHhIiJj%0AKkLlMmNnOoPpQqRrSsTt%0AUuVvWwXxYyZz%5B%5D%7B%7D%5C%7C%2F-%2B%3D%0A012345678!%40%23%24%25%5E%26*()_

=======
#include "array_.h"
#include "vector_.h"
#include "vec2i.h"
#include "entity.h"
#include "rect.h"

#include "printable.h"
#include "icon.h"
#include "animatingicon.h"
#include "gameplay.h"
>>>>>>> origin/master

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

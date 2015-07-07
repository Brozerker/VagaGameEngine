#include "platform_conio.h"
#include <stdlib.h>

#include "array_.h"
#include "vector_.h"
#include "vec2i.h"
#include "entity.h"
#include "rect.h"

#include "printable.h"
#include "icon.h"
#include "animatingicon.h"
#include "gameplay.h"

#include <ctime>
#include "platform_random.h"

#include "map2d.h"
int main() {
	printf("%d", sizeof(Enemy));
	platform_getch();
	srand((unsigned int)time(NULL));
	Gameplay g(10);
	clock_t now, previous = clock(), deltaTime, soon;
	int FPS = 20;
	Map2D screen;
	const char * screenBufferInitialization =
		"............................................................"
		"............................................................"
		"............................................................"
		"............................................................"
		"............................................................"
		"............................................................"
		"............................................................"
		"............................................................"
		"............................................................"
		"............................................................"
		"............................................................"
		"............................................................"
		"............................................................"
		"............................................................"
		"............................................................"
		"............................................................";
	screen.SetData(screenBufferInitialization, 16, 60);
	while (g.IsGameRunning()) { // game loop
		// timing code
		now = clock();
		deltaTime = now - previous;
		previous = now;

		// game loop code
		g.Draw(&screen);                     // fill the off-screen buffer
		platform_move(0, 0); screen.Draw();  // present the off-screen buffer, also called "flip"
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

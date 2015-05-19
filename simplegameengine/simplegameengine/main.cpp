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

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

#include "timerqueue.h"
int main() {
	// toying with functional programming
	/*
	struct Item {
	const char * name;
	std::function<void()> use;
	Item(const char * name, std::function<void()> use) :name(name), use(use){}
	};

	Item items[] = {
		Item("potion", []()->void{ printf("glug glug glug"); }),
		Item("sword", []()->void{ printf("**swing**"); }),
		Item("laser", []()->void{ printf("<<pew-pew>>"); }),
		Item("myterious token", []()->void{ printf("<teleports you to another dimension>"); }),
	};
	int itemCount = sizeof(items) / sizeof(items[0]);

	for (int i = 0; i < itemCount; ++i){
		printf("\n\nusing %s...\n", items[i]);
		items[i].use();
	}
	platform_getch();
	*/
	srand((unsigned int)time(NULL));
	Gameplay g;
	clock_t now, previous = clock(), deltaTime, soon;
	int FPS = 30;
	TimerQueue tq;
	int index = 0;
	// capture list, the argument list in the square braces
	// because the capture list uses variables from the scope that this function was define in, this is called a
	// "closure"
	std::function<void()> thingToDo = [&index,&tq,&thingToDo]()->void {
		platform_move(22, 22+index);
		const char * message = "hi. what is your name?";
		putchar(message[index]);
		index++;
		if (message[index] != '\0')
			tq.Add(100, thingToDo);
	};
	tq.Add(2000, thingToDo);
	tq.Add(1000, []()->void{
		platform_getch();
	});

	std::function<int(int, int)> add = [](int a, int b)->int {
		return a + b;
	};
	std::function<int(int, int)> subtract = [](int a, int b)->int {
		return a - b;
	};
	std::function<int(int, int)> f;
	f = add;
	printf("1 + 2 = %d\n", f(1,2));
	f = subtract;
	printf("1 - 2 = %d\n", f(1, 2));
	platform_getch();

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
		tq.Update(deltaTime);

		// throttle code
		soon = now + (1000 / FPS);
		while (platform_upTimeMS() < soon && !platform_kbhit()) { Sleep(1); }
	}
	return 0;
}

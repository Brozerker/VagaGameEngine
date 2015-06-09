#pragma once
#include "entity.h"
#include "rect.h"
#include "vector_.h"
#include "vec2i.h"
#include "animatingicon.h"
#include "map2d.h"

// this should be
static AnimatingIcon::Frame playerIconFrames[] = {
	AnimatingIcon::Frame(Pixel(2, 8, 0), 150),
	AnimatingIcon::Frame(Pixel(2, 7, 0), 50),
	AnimatingIcon::Frame(Pixel(2, 15, 0), 100),
	AnimatingIcon::Frame(Pixel(2, 7, 0), 50),
};
const int playerIconFrames_count = sizeof(playerIconFrames) / sizeof(playerIconFrames[0]);

class Gameplay {

//	Entity player;
	Vector_<Entity> players;
	Rect world;
	long keyToHit;
	bool gameRunning;
	long userInput;
	Vector_<int> keypresses;
	bool cheatMode;
	AnimatingIcon animation;
public:
	Gameplay(int playerCount) :
//		player(new AnimatingIcon(playerIconFrames, sizeof(playerIconFrames) / sizeof(playerIconFrames[0])), Vec2i(4, 0)),
		world(Pixel('.', 7, 15), Vec2i(), Vec2i(20, 15)),
		gameRunning(true), userInput(-1), cheatMode(false)
	{
		for (int i = 0; i < playerCount; ++i) {
			players.Add(Entity(new AnimatingIcon(playerIconFrames, playerIconFrames_count), Vec2i(4+i, i)));
		}
		keyToHit = (rand() % 26) + 'a';
		static AnimatingIcon::Frame frames[] = {
			AnimatingIcon::Frame(Pixel('|'), 100),
			AnimatingIcon::Frame(Pixel('/'), 100),
			AnimatingIcon::Frame(Pixel('-'), 100),
			AnimatingIcon::Frame(Pixel('\\'), 100),
		};
		animation.SetFrames(frames, sizeof(frames) / sizeof(frames[0]));
	}
	bool IsGameRunning() { return gameRunning; }
	void Draw(Map2D * graphicsContext) {
		world.Draw(graphicsContext);
//		player.Draw();
		for (int i = 0; i < players.Count(); ++i) {
			players[i].Draw(graphicsContext);
		}
		platform_move(world.size.y + 1, 0);
		platform_setColor(7, 0);  printf("\npress ");
		platform_setColor(13, 0); printf("%c", keyToHit);
		animation.Print();
	}
	void UserInput(long inputFromSystem) { userInput = inputFromSystem; }
	void Update(clock_t deltaTimeMS) {
		animation.Update(deltaTimeMS);
//		player.Update(deltaTimeMS);
		for (int i = 0; i < players.Count(); ++i) {
			players[i].Update(deltaTimeMS);
		}
		Entity * currentP = &players[0];
		switch (userInput){
		case 'w':	currentP->position.y--;	break;
		case 'a':	currentP->position.x--;	break;
		case 's':	currentP->position.y++;	break;
		case 'd':	currentP->position.x++;	break;
		}
		//// update based on time, user input, game state (variables of the game)
		//if (userInput == keyToHit){
		//	player.position.x++;
		//	int next;
		//	// make it so the same key won't show up twice.
		//	do{ next = (rand() % 26) + 'a'; } while (next == keyToHit);
		//	keyToHit = next;
		//	if (cheatMode) keyToHit = 'q';
		//} else {
		//	if (userInput >= 'a' && userInput <= 'z')
		//		player.position.x--;
		//}
		//if (player.position.x >= world.size.x) {
		//	gameRunning = false;
		//	platform_setColor(10, 0); printf("WINNAR!\n");
		//} else if (player.position.x < 0) {
		//	gameRunning = false;
		//	platform_setColor(12, 0); printf("You Lose.\n");
		//}
		if (userInput == '\r' || userInput == '\n') {
			int testSequence[] = { PLATFORM_KEY_UP, PLATFORM_KEY_DOWN, PLATFORM_KEY_LEFT, PLATFORM_KEY_RIGHT, ' ' };
			if (keypresses.IsEqual(testSequence, sizeof(testSequence) / sizeof(testSequence[0]))) {
				// God mode activate?
				cheatMode = true;
				keyToHit = 'q';
			}
			keypresses.Clear();
		}
		else if(userInput != -1) {
			keypresses.Add(userInput);
			//printf("\n");
			//for (int i = 0; i < keypresses.Count(); ++i) {
			//	printf("[%c] (%d)\n", keypresses[i], keypresses[i]);
			//}
			//printf("-----------");
		}
		userInput = -1;
	}
};
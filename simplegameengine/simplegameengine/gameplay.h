#pragma once
#include "entity.h"
#include "rect.h"
#include "vector_.h"
#include "vec2i.h"
#include "animatingicon.h"
#include "map2d.h"
#include "timerqueue.h"

// this should be
static AnimatingIcon::Frame playerIconFrames[] = {
	AnimatingIcon::Frame(Pixel(2, 8, 0), 150),
	AnimatingIcon::Frame(Pixel(2, 7, 0), 50),
	AnimatingIcon::Frame(Pixel(2, 15, 0), 100),
	AnimatingIcon::Frame(Pixel(2, 7, 0), 50),
};
const int playerIconFrames_count = sizeof(playerIconFrames) / sizeof(playerIconFrames[0]);

class Gameplay {
	TimerQueue tq;
	Vector_<Entity> players;
	Rect world;
	bool gameRunning;
	long userInput;
	Vector_<int> keypresses;
	bool cheatMode;
	AnimatingIcon animation;
public:
	Gameplay(int playerCount) :
		world(Vec2i(), Vec2i(20, 15)),
		gameRunning(true), userInput(-1), cheatMode(false)
	{
		for (int i = 0; i < playerCount; ++i) {
			players.Add(Entity(new AnimatingIcon(playerIconFrames, playerIconFrames_count), Vec2i(4+i, i)));
		}
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
		world.Draw(graphicsContext, Pixel('.'));
		for (int i = 0; i < players.Count(); ++i) {
			//if (world.Contains(players[i].position)) {
			if (graphicsContext->GetRect().Contains(players[i].position)) {
				players[i].Draw(graphicsContext);
			}
		}
		animation.Print();
	}
	void UserInput(long inputFromSystem) { userInput = inputFromSystem; }
	void Update(clock_t deltaTimeMS) {
		tq.Update(deltaTimeMS);
		animation.Update(deltaTimeMS);
		for (int i = 0; i < players.Count(); ++i) {
			players[i].Update(deltaTimeMS);
		}
		Entity * currentP = &players[0];
		switch (userInput){
		case 'w':	case PLATFORM_KEY_UP: currentP->position.y--;	break;
		case 'a':	case PLATFORM_KEY_LEFT: currentP->position.x--;	break;
		case 's':	case PLATFORM_KEY_DOWN: currentP->position.y++;	break;
		case 'd':	case PLATFORM_KEY_RIGHT: currentP->position.x++;	break;
		}
		if (userInput == '\r' || userInput == '\n') {
			int testSequence[] = { PLATFORM_KEY_UP, PLATFORM_KEY_DOWN, PLATFORM_KEY_LEFT, PLATFORM_KEY_RIGHT, ' ' };
			if (keypresses.IsEqual(testSequence, sizeof(testSequence) / sizeof(testSequence[0]))) {
				// God mode activate?
				cheatMode = true;
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

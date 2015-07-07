#pragma once
#include "entity.h"
#include "rect.h"
#include "vector_.h"
#include "vec2i.h"
#include "animatingicon.h"
#include "map2d.h"
#include "timerqueue.h"
#include "enemy.h"

class Gameplay {
	TimerQueue tq;
	Vector_<Entity*> players;
	Rect world;
	bool gameRunning;
	long userInput;
	Vector_<int> keypresses;
	bool cheatMode;
	AnimatingIcon animation;
	Map2D map;
public:
	Gameplay(int playerCount) :
		world(Vec2i(), Vec2i(20, 15)),
		gameRunning(true), userInput(-1), cheatMode(false)
	{
		const char * maze =
			"####################"
			"#         #        #"
			"#         #    #   #"
			"#         #    ## ##"
			"#         ######   #"
			"#                  #"
			"#   #              #"
			"#    #             #"
			"#     #            #"
			"#      #           #"
			"#       ############"
			"#                  #"
			"#                  #"
			"#                  #"
			"####################";
		map.SetData(maze, 15, 20);
		// the static keyword means that playerIconFrames continue to exist without the function that contains it
		static AnimatingIcon::Frame playerIconFrames[] = {
			AnimatingIcon::Frame(Pixel(2, 8, 0), 150),
			AnimatingIcon::Frame(Pixel(2, 7, 0), 50),
			AnimatingIcon::Frame(Pixel(2, 15, 0), 100),
			AnimatingIcon::Frame(Pixel(2, 7, 0), 50),
		};
		const int playerIconFrames_count = sizeof(playerIconFrames) / sizeof(playerIconFrames[0]);
		players.Add(new Entity(new AnimatingIcon(playerIconFrames, playerIconFrames_count), Vec2i(3, 0))); // player
		for (int i = 0; i < playerCount; ++i) {
			players.Add(new Enemy(new AnimatingIcon(playerIconFrames, playerIconFrames_count), Vec2i(4+i, i), 200)); // enemies
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
		//world.Draw(graphicsContext, Pixel(' '));
		graphicsContext->Set(0, 0, map);
		for (int i = 0; i < players.Count(); ++i) {
			//if (world.Contains(players[i].position)) {
			//if (graphicsContext->GetRect().Contains(players[i].position)) {
				players[i]->Draw(graphicsContext);
			//}
		}
		animation.Print();
	}
	void UserInput(long inputFromSystem) { userInput = inputFromSystem; }
	void Update(clock_t deltaTimeMS) {
		tq.Update(deltaTimeMS);
		animation.Update(deltaTimeMS);
		for (int i = 0; i < players.Count(); ++i) {

	// asking for forgiveness
			// keeps track of where the player was, as a responsible peices of data that can fix a position problem later
			Vec2i oldPosition = players[i]->position;

			players[i]->Update(deltaTimeMS);

			Vec2i p = players[i]->position;

			// if the player's new position is wrong (out of range, or inside of a wall)
			if(p.x < 0                     || p.y < 0 
			|| p.x >= map.GetRect().size.x || p.y >= map.GetRect().size.y 
			|| map.Get(p.y, p.x).c == '#') {
				// go back to the old position
				players[i]->position = oldPosition;
			}

		}
		Entity * currentP = players[0];
		currentP->moveByKeyPress(userInput);
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

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
	Entity * win;
public:
	Gameplay(int playerCount) :
		world(Vec2i(), Vec2i(20, 15)),
		gameRunning(true), userInput(-1), cheatMode(false)
	{
		map.LoadFile("level1.txt");
		AnimatingIcon * playerSkin = new AnimatingIcon("playerSkin.txt");
		Entity * player = new Entity(playerSkin, Vec2i(2, 13), "player");
		player->SetWhenColliding([&](Entity* self, Entity * e)->void {
			if (e->IsType("enemy")) {
				if (!self->GetInvincible()){
					gameRunning = false;
					printf("You Lose!\nPress SPACE to continue\n"); while (platform_getch() != ' ');
				}
			} else if (e->IsType("win")){
				gameRunning = false;
				printf("You WIN!\nPress SPACE to continue\n"); while (platform_getch() != ' ');
			} else if (e->IsType("power")) {
				static AnimatingIcon::Frame powerIcon[] = {
					AnimatingIcon::Frame(Pixel(2, FOREGROUND_BLUE, 0), 150),
					AnimatingIcon::Frame(Pixel(2, FOREGROUND_GREEN|FOREGROUND_BLUE, 0), 150),
					AnimatingIcon::Frame(Pixel(2, FOREGROUND_GREEN|FOREGROUND_BLUE | FOREGROUND_INTENSITY, 0), 200),
					AnimatingIcon::Frame(Pixel(2, FOREGROUND_GREEN|FOREGROUND_BLUE, 0), 150),
				};
				delete players[0]->icon;
				self->icon = new AnimatingIcon(powerIcon, 4);
				self->SetInvincible(true);
				tq.Add(2000, [=]()->void { 
					// '=' passes closure variables by value, because 'self' is a temporary stack variable. 
					// a copy of the value will not be modified.
					delete self->icon;
					AnimatingIcon * playerSkin = new AnimatingIcon("playerSkin.txt");
					self->icon = playerSkin;
					self->SetInvincible(false);
				});
			}
		});
		players.Add(player);
		std::function<void(Entity*,Entity*)> whatToDoWhenEnemyTouchesPlayer = [&](Entity *self, Entity * e)->void {
			if (e->IsType("player")) {
				if (!e->GetInvincible()){
					gameRunning = false;
					printf("You Lose!!!!!\nPress SPACE to continue\n"); while (platform_getch() != ' ');
				}
			}
		};
		AnimatingIcon * enemySkin = new AnimatingIcon("enemySkin.txt");
		for (int i = 0; i < playerCount; ++i) {
			Entity * e = new Enemy(enemySkin->Clone(), Vec2i(4 + i, i), 1000, "enemy");
			e->SetWhenColliding(whatToDoWhenEnemyTouchesPlayer);
			players.Add(e);
		}
		static AnimatingIcon::Frame frames[] = {
			AnimatingIcon::Frame(Pixel('|'), 100),
			AnimatingIcon::Frame(Pixel('/'), 100),
			AnimatingIcon::Frame(Pixel('-'), 100),
			AnimatingIcon::Frame(Pixel('\\'), 100),
		};
		animation.SetFrames(frames, sizeof(frames) / sizeof(frames[0]));
		//tq.Add(1000, []()->void {
		//	printf("He look at me!");
		//	platform_getch();
		//});
		win = new Entity(new AnimatingIcon(animation), Vec2i(12, 2), "win");
		win->SetBlocking(false);
		players.Add(win);
		Entity * power = new Entity(new AnimatingIcon(animation), Vec2i(15, 13), "power");
		power->SetBlocking(false);
		players.Add(power);
	}
	bool IsGameRunning() { return gameRunning; }
	void Draw(Map2D * graphicsContext) {
		graphicsContext->Set(0, 0, map);
		for (int i = 0; i < players.Count(); ++i) {
			players[i]->Draw(graphicsContext);
		}
		animation.Print();
	}
	void UserInput(long inputFromSystem) { userInput = inputFromSystem; }

	Entity * GetOtherEntityInSpot(Vec2i spot, Entity * ignore) {
		for (int i = 0; i < players.Count(); ++i) {
			if (players[i] != ignore && players[i]->position == spot) {
				return players[i];
			}
		}
		return nullptr;
	}

	void Update(clock_t deltaTimeMS) {
		tq.Update(deltaTimeMS); // service the update queue
		animation.Update(deltaTimeMS);

		players[0]->SetUserInput(userInput);

		// update AND collision detection
		for (int i = 0; i < players.Count(); ++i) {
			// keeps track of where the player was, as a responsible peices of data that can fix a position problem later
			Vec2i oldPosition = players[i]->position;
			players[i]->Update(deltaTimeMS);
			Vec2i p = players[i]->position;
			Entity * colliding = GetOtherEntityInSpot(p, players[i]);
			// if the player's new position is wrong (out of range, or inside of a wall)
			if (p.x < 0 || p.y < 0
				|| p.x >= map.GetRect().size.x || p.y >= map.GetRect().size.y
				|| map.Get(p.y, p.x).c == '#' || (colliding != nullptr && colliding->IsBlocking()) ) {
				// go back to the old position
				players[i]->position = oldPosition;
			}
			// do collision code even if the object colliding is not blocking
			if (colliding != nullptr) {
				players[i]->CollideWith(colliding);
			}
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
		}
		userInput = -1;
	}
};

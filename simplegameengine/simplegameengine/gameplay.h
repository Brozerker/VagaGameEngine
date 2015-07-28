#pragma once
#include "entity.h"
#include "rect.h"
#include "vector_.h"
#include "vec2i.h"
#include "animatingicon.h"
#include "map2d.h"
#include "timerqueue.h"
#include "enemy.h"
#include "resource.h"
#include "memorypool.h"
class Gameplay {
	TimerQueue tq;
	// list of all the entities that will be drawn and updated
	Vector_<Entity*> entities;
	// memory pool for entities : smarter allcoation/deallocation
	Mempool<Entity> players;
	Mempool<Enemy> enemies;
	Rect world;
	bool gameRunning;
	long userInput;
	Vector_<int> keypresses;
	bool cheatMode;
	Map2D * map;
	Entity * win;
	Resource res;
public:
	Gameplay(int playerCount) :
		world(Vec2i(), Vec2i(20, 15)),
		gameRunning(true), userInput(-1), cheatMode(false)
	{
		players.SetCreateNew([](){ return new Entity(); });
		players.SetDestroy([](Entity * e){ delete e; });
		std::function<void(Entity*, Entity*)> whatToDoWhenEnemyTouchesPlayer = [&](Entity *self, Entity * e)->void {
			if (!e->GetInvincible()){
				gameRunning = false;
				printf("You Lose!!!!!\nPress SPACE to continue\n"); while (platform_getch() != ' ');
			}
		};
		map = res.GetMap2D("level1.txt");
		Entity * player = players.Get()->Init(res.GetAnimation("playerSkin.txt")->Clone(), Vec2i(2, 13), "player");
		// define rules for the player
		player->AddBehavior("enemy", whatToDoWhenEnemyTouchesPlayer);
		player->AddBehavior("win", [&](Entity* self, Entity * e)->void {
			gameRunning = false;
			printf("You WIN!\nPress SPACE to continue\n"); while (platform_getch() != ' ');
		});
		player->AddBehavior("power", [&](Entity* self, Entity * e)->void {
			delete self->icon;
			self->icon = res.GetAnimation("powerSkin.txt")->Clone();
			self->SetInvincible(true);
			tq.Add(2000, [=]()->void { 
				// '=' passes closure variables by value, because 'self' is a temporary stack variable. 
				// a copy of the value will not be modified.
				delete self->icon;
				self->icon = res.GetAnimation("playerSkin.txt")->Clone();// playerSkin;
				self->SetInvincible(false);
			});
		});
		enemies.SetCreateNew([&](){ 
			Enemy * e = new Enemy(res.GetAnimation("enemySkin.txt")->Clone(), Vec2i(), 1000, "enemy");
			e->AddBehavior("player", whatToDoWhenEnemyTouchesPlayer);
			return e;
		});
		enemies.SetDestroy([](Enemy * e){ delete e; });
		for (int i = 0; i < playerCount; ++i) {
			Enemy * e = enemies.Get();
			e->position = Vec2i(i + 3, i + 1);
		}
		//tq.Add(1000, []()->void {
		//	printf("He look at me!");
		//	platform_getch();
		//});
		// TODO mempool for items
		win = players.Get()->Init(res.GetAnimation("spinnyPipe.txt")->Clone(), Vec2i(12, 2), "win");
		win->SetBlocking(false);
		Entity * power = players.Get()->Init(res.GetAnimation("spinnyPipe.txt")->Clone(), Vec2i(15, 13), "power");
		power->SetBlocking(false);

		for (int i = 0; i < players.size(); ++i) { entities.Add(players[i]); }
		for (int i = 0; i < enemies.size(); ++i) { entities.Add(enemies[i]); }
	}
	bool IsGameRunning() { return gameRunning; }
	void Draw(Map2D * graphicsContext) {
		graphicsContext->Set(0, 0, *map);
		for (int i = 0; i < entities.Count(); ++i) {
			entities[i]->Draw(graphicsContext);
		}
	}
	void UserInput(long inputFromSystem) { userInput = inputFromSystem; }

	Entity * GetOtherEntityInSpot(Vec2i spot, Entity * ignore) {
		for (int i = 0; i < entities.Count(); ++i) {
			if (entities[i] != ignore && entities[i]->position == spot) {
				return entities[i];
			}
		}
		return nullptr;
	}

	void Update(clock_t deltaTimeMS) {
		tq.Update(deltaTimeMS); // service the update queue

		players[0]->SetUserInput(userInput);

		// update AND collision detection
		for (int i = 0; i < entities.Count(); ++i) {
			// keeps track of where the player was, as a responsible peices of data that can fix a position problem later
			Vec2i oldPosition = entities[i]->position;
			entities[i]->Update(deltaTimeMS);
			Vec2i p = entities[i]->position;
			Entity * colliding = GetOtherEntityInSpot(p, entities[i]);
			// if the player's new position is wrong (out of range, or inside of a wall)
			if (p.x < 0 || p.y < 0
			|| p.x >= map->GetRect().size.x || p.y >= map->GetRect().size.y
			|| map->Get(p.y, p.x).c == '#' || (colliding != nullptr && colliding->IsBlocking()) ) {
				// go back to the old position
				entities[i]->position = oldPosition;
			}
			// do collision code even if the object colliding is not blocking
			if (colliding != nullptr) {
				entities[i]->Behave(colliding->GetType(), colliding);// CollideWith(colliding);
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

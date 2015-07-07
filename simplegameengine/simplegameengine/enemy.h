#pragma once
#include "entity.h"

class Enemy : public Entity {
public:
	// how long to wait between moves, in milliseconds
	int intervalBetweenMoves;
	// how long has passed since the last move
	int timer;

	Enemy(Printable * icon, Vec2i position, int moveDelay) : Entity(icon, position), intervalBetweenMoves(moveDelay), timer(0)
	{}

	void Update(int msPassed){
		Entity::Update(msPassed);
		timer += msPassed;
		if (timer >= intervalBetweenMoves) {
			// do random move here
			char * possibleMoves = "wasd";
			int possibleMoves_count = strlen(possibleMoves);
			int randomNumber = rand() % possibleMoves_count;
			int randomMove = (int)possibleMoves[randomNumber];
			moveByKeyPress(randomMove);
			//moveByKeyPress(possibleMoves[rand() % possibleMoves_count]);

			timer -= intervalBetweenMoves;
		}
	}
};

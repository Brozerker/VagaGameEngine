#pragma once
#include "entity.h"

class Enemy : public Entity {
private:
	void SetUserInput(int){}
public:
	// how long to wait between moves, in milliseconds
	int intervalBetweenMoves;
	// how long has passed since the last move
	int timer;

	Enemy(Printable * icon, Vec2i position, int moveDelay, const char * type) 
		: Entity(icon, position, type), intervalBetweenMoves(moveDelay), timer(0)
	{}


	void Update(int msPassed){
		timer += msPassed;
		if (timer >= intervalBetweenMoves) {
			// do random move here
			char * possibleMoves = "wasd";
			int possibleMoves_count = strlen(possibleMoves);
			int randomNumber = rand() % possibleMoves_count;
			userInput = (int)possibleMoves[randomNumber];
			timer -= intervalBetweenMoves;
		} else {
			userInput = 0;
		}
		Entity::Update(msPassed);
	}
};

#pragma once
#include "vec2i.h"
#include "printable.h"
#include "animatingicon.h"
#include <functional>
#include <vector>

class Entity {
	bool isBlocking;
	bool iconAnimates;
	bool invincible;
//	std::function<void(Entity*,Entity*)> whenColliding;

	struct CollisionBehavior {
		const char * who;
		std::function<void(Entity*, Entity*)> how;
		CollisionBehavior(const char * who, std::function<void(Entity*, Entity*)> how) : who(who), how(how) {}
	};
	std::vector<CollisionBehavior> behaviors;
protected:
	int userInput;
	const char * type;
public:
	void AddBehavior(const char * who, std::function<void(Entity*, Entity*)> how) {
		behaviors.push_back(CollisionBehavior(who, how));
	}
	void Behave(const char * name, Entity * entity) {
		for (int i = 0; i < behaviors.size(); ++i){
			if (strcmp(behaviors[i].who, name) == 0) {
				behaviors[i].how(this, entity);
			}
		}
	}

	void SetUserInput(int input){ userInput = input; }
	int GetUserInput() { return userInput; }

	void SetBlocking(bool c) { isBlocking = c; }
	bool IsBlocking() { return isBlocking; }

	void SetInvincible(bool i) { invincible = i; }
	bool GetInvincible() { return invincible; }

	const char * GetType() { return type; }

	bool IsType(const char * type) { return strcmp(this->type, type) == 0; }

	//// the first entity is self, the second is the colliding entity
	//void SetWhenColliding(std::function<void(Entity*,Entity*)> whenColliding) {
	//	this->whenColliding = whenColliding;
	//}
	//void CollideWith(Entity * e){
	//	if (whenColliding != nullptr){
	//		whenColliding(this, e);
	//	}
	//}

	Printable * icon;
	Vec2i position;
	Entity() : icon(nullptr), iconAnimates(false), isBlocking(true) {}
	Entity(Printable * icon, Vec2i position, const char * type)
		: icon(icon), position(position), type(type), isBlocking(true), invincible(false)//, whenColliding(nullptr)
	{
		iconAnimates = dynamic_cast<AnimatingIcon*>(icon) != nullptr;
	}

	Entity * Init(Printable * icon, Vec2i position, const char * type) {
		this->icon = icon; this->position = position; this->type = type; isBlocking = true; invincible = false;
		iconAnimates = dynamic_cast<AnimatingIcon*>(icon) != nullptr;
		return this;
	}

	void Draw(Map2D * graphicsContext) {
		if (icon != nullptr) {
			icon->Print(graphicsContext, position);
			//platform_move(position.y, position.x); icon->Print();
		}
	}
	virtual void Update(int ms){
		if (iconAnimates) {
			((AnimatingIcon*)icon)->Update(ms);
		}
		moveByKeyPress(userInput);
	}

	void moveByKeyPress(int userInput) {
		switch (userInput){
		case 'w':	case PLATFORM_KEY_UP: position.y--;	break;
		case 'a':	case PLATFORM_KEY_LEFT: position.x--;	break;
		case 's':	case PLATFORM_KEY_DOWN: position.y++;	break;
		case 'd':	case PLATFORM_KEY_RIGHT: position.x++;	break;
		}
	}
};

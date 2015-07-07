#pragma once
#include "vec2i.h"
#include "printable.h"
#include "animatingicon.h"

class Entity {
	bool iconAnimates;
public:
	Printable * icon;
	Vec2i position;
	Entity() : icon(nullptr), iconAnimates(false) {}
	Entity(Printable * icon, Vec2i position) : icon(icon), position(position) {
		iconAnimates = dynamic_cast<AnimatingIcon*>(icon) != nullptr;
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
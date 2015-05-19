#pragma once
#include "vec2i.h"
#include "printable.h"
#include "animatingicon.h"

class Entity {
	bool iconAnimates;
public:
	Printable * icon;
	Vec2i position;
	Entity(Printable * icon, Vec2i position) : icon(icon), position(position) {
		iconAnimates = dynamic_cast<AnimatingIcon*>(icon) != nullptr;
	}
	void Draw() {
		platform_move(position.y, position.x); icon->Print();
	}
	void Update(int ms){
		if (iconAnimates) {
			((AnimatingIcon*)icon)->Update(ms);
		}
	}
};
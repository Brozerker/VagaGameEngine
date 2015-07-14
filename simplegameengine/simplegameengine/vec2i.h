#pragma once

struct Vec2i {
	int x, y;
	Vec2i() : x(0), y(0) {}
	Vec2i(int x, int y) :x(x), y(y) {}

	bool operator==(Vec2i & other) {
		return x == other.x && y == other.y;
	}
};
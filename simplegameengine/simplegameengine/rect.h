#pragma once

#include "icon.h"
#include "pixel.h"
#include "vec2i.h"

// promise to the compiler that this class will exist.
class Map2D; // forward class declaration, so it can be used as function input

class Rect {
public:
	Vec2i position, size;
	Rect(Vec2i position, Vec2i size) : position(position), size(size) {}
	bool Contains(Vec2i p) {
		return p.x >= position.x        && p.y >= position.y 
			&& p.x <  position.x+size.x && p.y <  position.y+size.y;
	}

	// promise that Draw will exist. Map2d is used as an argument
	void Draw(Map2D * graphicsContext, Pixel p);
};
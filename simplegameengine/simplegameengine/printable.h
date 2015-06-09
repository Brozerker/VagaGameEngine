#pragma once
#include "vec2i.h"

class Map2D; // class prototype, which will allow use of the class in methods BEFORE Map2D is defined

class Printable {
public:
	virtual void Print() = 0;
	virtual void Print(Map2D * graphicsContext, Vec2i offset) = 0;
	virtual ~Printable(){}
};

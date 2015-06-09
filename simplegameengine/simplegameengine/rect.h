#pragma once

#include "icon.h"
#include "map2d.h"

class Rect {
public:
	Pixel icon;
	Vec2i position, size;
	Rect(Pixel icon, Vec2i position, Vec2i size) : icon(icon), position(position), size(size) {}
	void Draw(Map2D * graphicsContext){
		for (int row = 0; row < size.y; ++row){
			for (int col = 0; col < size.x; ++col){
				//platform_move(row, col);
				//icon.Print();
				graphicsContext->Set(row, col, icon);
			}
		}
	}
};
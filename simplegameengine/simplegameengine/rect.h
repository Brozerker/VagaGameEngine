#pragma once

#include "icon.h"

class Rect {
public:
	Icon icon;
	Vec2i position, size;
	Rect(Icon icon, Vec2i position, Vec2i size) : icon(icon), position(position), size(size) {}
	void Draw(){
		for (int row = 0; row < size.y; ++row){
			for (int col = 0; col < size.x; ++col){
				platform_move(row, col);
				icon.Print();
			}
		}
	}
};
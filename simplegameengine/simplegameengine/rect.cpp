#include "rect.h"
#include "map2d.h"

void Rect::Draw(Map2D * graphicsContext, Pixel p){
	for (int row = 0; row < size.y; ++row){
		for (int col = 0; col < size.x; ++col){
			//platform_move(row, col);
			//icon.Print();
			graphicsContext->Set(row, col, p);
		}
	}
}
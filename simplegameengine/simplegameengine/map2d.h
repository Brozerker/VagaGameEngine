#pragma once
#include "array_.h"
#include "platform_conio.h"
#include "rect.h"
#include "pixel.h"

class Map2D {
protected:
	int width, height;
	Array_<Pixel> data;
public:
	// return true if the point is withing the rectangular boundary of this map
	Map2D() : width(0), height(0) {}
	void SetSize(int width, int height){
		this->width = width;
		this->height = height;
		data.SetAllocatedSize(width * height);
	}
	Rect GetRect() {
		return Rect(Vec2i(0, 0), Vec2i(width, height));
	}
	Pixel Get(int row, int col) {
		if (row < 0 || row >= height || col < 0 || col >= width) {
			int i = 0; i = 1 / i; // OUT OF BOUNDS CRASH
		}
		return data[row * width + col];
	}
	void Set(int row, int col, Pixel value){
		if (row < 0 || row >= height || col < 0 || col >= width) {
			int i = 0; i = 1 / i; // OUT OF BOUNDS CRASH
		}
		data[row * width + col] = value;
	}
	// allows access of the map using double square brackets, but without any bounds checking
	Pixel * operator[](int index){ return &data[index * width]; }
	void Draw() {
		for (int row = 0; row < height; ++row) {
			for (int col = 0; col < width; ++col) {
				Get(row, col).Print();
			}
			putchar('\n');
		}
	}
	void SetData(const char * str, int rows, int cols) {
		SetSize(cols, rows);
		int row = 0, col = 0, index = 0;
		while (str[index] != '\0') {
			char c = str[index++];
			switch (c){
			case '\n': case '\r': break;
			default:
				Set(row, col, c);
				col++;
				if (col >= width){
					col = 0;
					row++;
				}
			}
		}
	}
};


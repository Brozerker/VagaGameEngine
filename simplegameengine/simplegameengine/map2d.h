#pragma once
#include "array_.h"
#include "platform_conio.h"

struct Pixel
{
	static const int FORE = 7, BACK = 0;
	char c;
	char fcolor;
	char bcolor;
	char __buffer;
	Pixel() : c('?'), fcolor(FORE), bcolor(BACK) {}
	Pixel(char c) : c(c), fcolor(FORE), bcolor(BACK) {}
	Pixel(char c, int f, int b) : c(c), fcolor(f), bcolor(b) {}
	void Print() {
		platform_setColor(fcolor, bcolor); putchar(c);
	}
};

class Map2D {
protected:
	int width, height;
	Array_<Pixel> data;
public:
	Map2D() : width(0), height(0) {}
	void SetSize(int width, int height){
		this->width = width;
		this->height = height;
		data.SetAllocatedSize(width * height);
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


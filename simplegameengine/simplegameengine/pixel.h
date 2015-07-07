#pragma once
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
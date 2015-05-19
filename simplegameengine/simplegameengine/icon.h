#pragma once

// classes hide complexity with encapsulation
class Icon : public Printable {
private:
	char icon, fcolor, bcolor;
public:
	Icon(char c, int foreColor, int backColor) : icon(c), fcolor(foreColor), bcolor(backColor) {}
	void Print() { platform_setColor(fcolor, bcolor); putchar(icon); }
	Icon & operator=(char c){ icon = c; return *this; }
	Icon(char c) : fcolor(7), bcolor(0) { operator=(c); }
};

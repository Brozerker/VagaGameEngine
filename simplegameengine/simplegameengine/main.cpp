#include "platform_conio.h"
#include <stdlib.h>

// when making your own data structures:
// 1) make it without templates or const correctness, just to make it.
// 2) once it works just like you want, then add const templates/correctness

template <typename T>
class Array_ {
protected:
	T * data;
	int allocatedSize;
public:
	int AllocatedSize() const { return allocatedSize; }
	T Get(const int index) const { return data[index]; }
	void Set(const int index, const T value) { data[index] = value; }
	void SetAllocatedSize(const int newSize){
		T * newData = new T[newSize];
		if (data) {
			int limit = ((allocatedSize < newSize) ? allocatedSize : newSize); //newSize; if (allcoatedSize < newSize) limit = allocatedSize;
			for (int i = 0; i < limit; ++i) {
				newData[i] = data[i];
			}
			delete[] data;
		}
		data = newData;
		allocatedSize = newSize;
	}
	Array_() :data(nullptr), allocatedSize(0) {}
	Array_(const Array_ & other) :data(nullptr), allocatedSize(0) { Copy(other); }
	void Copy(const Array_ & other) {
		SetSize(other.allocatedSize);
		for (int i = 0; i < allocatedSize; ++i){
			data[i] = other.data[i];
		}
	}
	void Release() { if (data) { delete[] data; data = nullptr; } allocatedSize = 0; }
	~Array_(){ Release(); }
	T & operator[](const int index) { 
		if (index < 0 || index >= allocatedSize) { int i = 0; i = 1 / i; } // explode with OOB
		return data[index];
	}
	Array_(const T * arr, const int size) : data(nullptr), allocatedSize(0) {
		SetAllocatedSize(size);
		for (int i = 0; i < size; ++i){
			data[i] = arr[i];
		}
	}
	bool operator==(const Array_ & other) const { return IsEqual(other); }
	bool IsEqual(const Array_ & other) const {
		if (other.allocatedSize != allocatedSize) return false;
		for (int i = 0; i < allocatedSize; ++i){
			if (data[i] != other.data[i]) return false;
		}
		return true;
	}
};
typedef Array_<int> Arrayi; // compiler level - typesafe (will generate more errors, a good thing)
// #define Arrayi Array_<int> // preprocessor level - less constrained (more dangerous)

template <typename T>
class Vector_ : Array_ < T > {
protected:
	int count;
public:
	static const int GROWTH_INCREMENT = 10; // growth by static size allows memory hole to be filled in during resize
	Vector_() : Array_(), count(0) {}
	Vector_(const Vector_ & other) : Array_() {
		Copy(other);
		count = other.count;
	}
	~Vector_() { /*Release(); ~Array_() is called automatically */ count = 0; }
	int Count() const { return count; }
	T & operator[](const int index) {
		if (index < 0 || index >= count) { int i = 0; i = 1 / i; } // explode with OOB
		return data[index];
	}
	void Add(const T & value) {
		if (count >= allocatedSize) {
			SetAllocatedSize(allocatedSize + GROWTH_INCREMENT);
		}
		data[count++] = value;
	}
	void Clear() { count = 0; }
	// TODO Insert(index, value), RemoveAt(index), Remove(const T & value)

	/** @return if this Vector_ stores the same members as the given array */
	bool IsEqual(const T * arr, const int size) const {
		if (size != count) return false;
		for (int i = 0; i < count; ++i){
			if (data[i] != arr[i]) return false;
		}
		return true;
	}
};

// (game engine)
//	gameplay (Gameplay)
//			world (Rect)
//				size (Vec2i)
//				icon (Icon)
//			player (Entity)
//				location (Vec2i)
//				icon (Icon)
//			random key to hit (char)
//	draw
//	user input
//	update

class Printable {
public:
	virtual void Print() = 0;
	virtual ~Printable(){}
};

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

class AnimatingIcon : public Printable {
public:
	struct Frame {
		Icon icon;
		int time;
		Frame(Icon icon, int time) : icon(icon), time(time){}
	};
private:
	Frame * frames;
	int count;
	int index, timer;
public:
	AnimatingIcon() : frames(nullptr), count(0), index(0), timer(0) {}
	AnimatingIcon(Frame * frames, const int count) : frames(frames), count(count), index(0), timer(0) {}
	void SetFrames(Frame * frames, const int count) { this->frames = frames; this->count = count; }
	void Print() { frames[index].icon.Print(); }
	void Update(int ms) {
		timer += ms;
		if (timer >= frames[index].time) {
			timer -= frames[index].time;
			index++;
			if (index >= count){
				index = 0;
			}
		}
	}
};

// public by default. don't support polymorphism! structs should be simple: should not require encapsulation.
struct Vec2i {
	int x, y;
	Vec2i() : x(0), y(0) {}
	Vec2i(int x, int y) :x(x), y(y) {}
};
class Entity {
	bool iconAnimates;
public:
	Printable * icon;
	Vec2i position;
	Entity(Printable * icon, Vec2i position) : icon(icon), position(position) {
		iconAnimates = dynamic_cast<AnimatingIcon*>(icon) != nullptr;
	}
	void Draw() {
		platform_move(position.y, position.x); icon->Print();
	}
	void Update(int ms){
		if (iconAnimates) {
			((AnimatingIcon*)icon)->Update(ms);
		}
	}
};
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

static AnimatingIcon::Frame playerIconFrames[] = {
	AnimatingIcon::Frame(Icon(2, 8, 0), 150),
	AnimatingIcon::Frame(Icon(2, 7, 0), 50),
	AnimatingIcon::Frame(Icon(2, 15, 0), 100),
	AnimatingIcon::Frame(Icon(2, 7, 0), 50),
};

class Gameplay {
	Entity player;
	Rect world;
	long keyToHit;
	bool gameRunning;
	long userInput;
	Vector_<int> keypresses;
	bool cheatMode;
	AnimatingIcon animation;
public:
	Gameplay() :
		player(new AnimatingIcon(playerIconFrames, sizeof(playerIconFrames) / sizeof(playerIconFrames[0])), Vec2i(4, 0)),
		world(Icon('.', 7, 15), Vec2i(), Vec2i(10, 1)),
		gameRunning(true), userInput(-1), cheatMode(false)
	{
		keyToHit = (rand() % 26) + 'a';
		static AnimatingIcon::Frame frames[] = {
			AnimatingIcon::Frame(Icon('|') , 100),
			AnimatingIcon::Frame(Icon('/'), 100),
			AnimatingIcon::Frame(Icon('-'), 100),
			AnimatingIcon::Frame(Icon('\\'), 100),
		};
		animation.SetFrames(frames, sizeof(frames) / sizeof(frames[0]));
	}
	bool IsGameRunning() { return gameRunning; }
	void Draw() {
		world.Draw();
		player.Draw();
		platform_setColor(7, 0);  printf("\npress ");
		platform_setColor(13, 0); printf("%c", keyToHit);
		animation.Print();
	}
	void UserInput(long inputFromSystem) { userInput = inputFromSystem; }
	void Update(clock_t deltaTimeMS) {
		animation.Update(deltaTimeMS);
		player.Update(deltaTimeMS);
		// update based on time, user input, game state (variables of the game)
		if (userInput == keyToHit){
			player.position.x++;
			int next;
			// make it so the same key won't show up twice.
			do{ next = (rand() % 26) + 'a'; } while (next == keyToHit);
			keyToHit = next;
			if (cheatMode) keyToHit = 'q';
		} else {
			if (userInput >= 'a' && userInput <= 'z')
				player.position.x--;
		}
		if (player.position.x >= world.size.x) {
			gameRunning = false;
			platform_setColor(10, 0); printf("WINNAR!\n");
		} else if (player.position.x < 0) {
			gameRunning = false;
			platform_setColor(12, 0); printf("You Lose.\n");
		}
		if (userInput == '\r' || userInput == '\n') {
			int testSequence[] = { PLATFORM_KEY_UP, PLATFORM_KEY_DOWN, PLATFORM_KEY_LEFT, PLATFORM_KEY_RIGHT, ' ' };
			if (keypresses.IsEqual(testSequence, sizeof(testSequence) / sizeof(testSequence[0]))) {
				// God mode activate?
				cheatMode = true;
				keyToHit = 'q';
			}
			keypresses.Clear();
		}
		else if(userInput != -1) {
			keypresses.Add(userInput);
			//printf("\n");
			//for (int i = 0; i < keypresses.Count(); ++i) {
			//	printf("[%c] (%d)\n", keypresses[i], keypresses[i]);
			//}
			//printf("-----------");
		}
		userInput = -1;
	}
};

// TODO http://patorjk.com/software/taag/#p=display&f=Univers&t=AaBbCcDdEeFfGgHhIiJj%0AKkLlMmNnOoPpQqRrSsTt%0AUuVvWwXxYyZz%5B%5D%7B%7D%5C%7C%2F-%2B%3D%0A012345678!%40%23%24%25%5E%26*()_


#include <ctime>
#include "platform_random.h"

void printInt(int num) {
	for (int i = 31; i >= 0; --i){
		putchar(((num&(1 << i)) != 0) ? '1' : '0');
	}
}

int main() {
	srand((unsigned int)time(NULL));
	Gameplay g;
	clock_t now, previous = clock(), deltaTime, soon;
	int FPS = 30;
	while (g.IsGameRunning()) { // game loop
		// timing code
		now = clock();
		deltaTime = now - previous;
		previous = now;

		// game loop code
		g.Draw();
		if (platform_kbhit()) {
			g.UserInput(platform_getch());
		}
		g.Update(deltaTime);

		// throttle code
		soon = now + (1000 / FPS);
		while (platform_upTimeMS() < soon && !platform_kbhit()) { Sleep(1); }
	}
	return 0;
}

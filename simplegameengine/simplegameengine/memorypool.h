#pragma once

//#include <vector>
#include <list>
#include <functional>

template <typename MEM>
class Mempool {
	std::list<MEM*> mem;
	std::list<MEM*> freed;
	std::function<MEM*()> createNew;
	std::function<void(MEM*)> destroy;
public:
	void SetCreateNew(std::function<MEM*()> createNew) {
		this->createNew = createNew;
	}
	void SetDestroy(std::function<void(MEM*)> destroy) {
		this->destroy = destroy;
	}
	MEM* Get() {
		if (freed.size() > 0) {
			MEM* front = freed.front();
			freed.pop_front();
			return front;
		}
		MEM* newOne = createNew();
		mem.push_back(newOne);
		return newOne;
	}
	void Release(MEM* entry) {
		mem.remove(entry);
		freed.pop_back(entry);
	}
	~Mempool() {
		while (freed.size() > 0) { destroy(freed.front()); freed.pop_front(); }
		while (mem.size() > 0) { destroy(mem.front()); mem.pop_front(); }
	}
	MEM* operator[](int const index) {
		int count = 0;
		for (std::list<MEM*>::iterator iter = mem.begin(); iter != mem.end(); iter++, count++) {
			if (count == index) return *iter;
		}
		return nullptr;
	}
	int size() const { return mem.size(); }
	int Count() const { return size(); }
};
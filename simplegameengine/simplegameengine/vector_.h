#pragma once
#include "array_.h"

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
#pragma once

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
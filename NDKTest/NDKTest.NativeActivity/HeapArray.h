#pragma once

#include "Array.h"

template <typename T>
struct HeapArray : public Array<T, 0>
{
	HeapArray(size_t size);
	HeapArray(size_t count, const T& value);
	HeapArray(const HeapArray& other);
	HeapArray(HeapArray&& other);
	HeapArray(const HeapArray& other, size_t sizeIn);
	HeapArray(HeapArray&& other, size_t sizeIn);
	HeapArray& operator=(const HeapArray& other);
	HeapArray& operator=(HeapArray&& other);

	bool operator==(const HeapArray& rhs) const;
	bool operator!=(const HeapArray& rhs) const;
};

template <typename T>
inline HeapArray<T>::HeapArray(size_t size) : Array<T, 0>{ { .heapArray.p = (T*)malloc(sizeof(T) * size), .heapArray.capacity = size } }
{
}

template <typename T>
inline HeapArray<T>::HeapArray(size_t count, const T & value) : HeapArray(count)
{
	for (size_t i = 0; i < count; ++i)
	{
		this->arrayUnion.heapArray.p[i] = value;
	}
	this->arraySize = count;
}

template <typename T>
inline HeapArray<T>::HeapArray(const HeapArray & other) : HeapArray(other, other.arrayUnion.heapArray.capacity)
{
}

template <typename T>
inline HeapArray<T>::HeapArray(HeapArray && other) : HeapArray(std::move(other), other.arrayUnion.heapArray.capacity)
{
}

template <typename T>
inline HeapArray<T>::HeapArray(const HeapArray & other, size_t capacityIn) : HeapArray(capacityIn)
{
	size_t size;
	if (other.arraySize < capacityIn)
		size = other.arraySize;
	else
		size = capacityIn;

	for (size_t i = 0; i < size; ++i)
	{
		this->arrayUnion.heapArray.p[i] = other.arrayUnion.heapArray.p[i];
	}
	this->arraySize = size;
}

template <typename T>
inline HeapArray<T>::HeapArray(HeapArray && other, size_t capacityIn) : HeapArray(capacityIn)
{
	if (other.arrayUnion.heapArray.capacity == capacityIn)
		this->arrayUnion.heapArray.p = std::exchange(other.arrayUnion.heapArray.p, nullptr);
	else
	{
		if (capacityIn < other.arraySize)
		{
			for (size_t i = capacityIn; i < other.arraySize; ++i)
				other.arrayUnion.heapArray.p[i].~T();
		}

		for (size_t i = 0; i < capacityIn; ++i)
			new (&this->arrayUnion.heapArray.p[i]) T(std::move(other.arrayUnion.heapArray.p[i]));

		free(other.arrayUnion.heapArray.p);
		other.arrayUnion.heapArray.p = nullptr;
	}

	this->arraySize = other.arraySize;
	other.arrayUnion.heapArray.capacity = 0;
	other.arraySize = 0;
}

template <typename T>
inline HeapArray<T> & HeapArray<T>::operator=(const HeapArray<T> & other)
{
	this->~Array<T, 0>();

	new (this) HeapArray(other);

	return *this;
}

template <typename T>
inline HeapArray<T> & HeapArray<T>::operator=(HeapArray<T> && other)
{
	this->~Array<T, 0>();

	new (this) HeapArray(std::move(other));

	return *this;
}

template<typename T>
inline bool HeapArray<T>::operator==(const HeapArray & rhs) const
{
	if (this->arrayUnion.arraySize != rhs.arraySize)
		return false;

	for (size_t i = 0; i < this->arraySize; ++i)
	{
		if (this->arrayUnion.heapArray.p[i] != rhs.arrayUnion.heapArray.p[i])
			return false;
	}

	return true;
}

template<typename T>
inline bool HeapArray<T>::operator!=(const HeapArray & rhs) const
{
	return (!(this->operator==(rhs)));
}

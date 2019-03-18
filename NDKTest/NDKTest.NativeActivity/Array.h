#pragma once

//NOTE: For std::move
#include <utility>
#include "Utils.h"

#ifndef NDEBUG
template <typename T>
class Iterator
{
	size_t itIndex;
	size_t itSize;
	T* itData;
public:
	Iterator(size_t indexIn, size_t sizeIn, T* dataIn) : itIndex(indexIn), itSize(sizeIn), itData(dataIn) {}

	Iterator& operator++()
	{
		++itIndex;
		assert(itIndex <= itSize);
		return *this;
	}
	Iterator operator++(int)
	{
		Iterator temp(*this);
		operator++();
		return temp;
	}
	Iterator& operator--()
	{
		--itIndex;
		assert(itIndex >= 0);
		return *this;
	}
	Iterator operator--(int)
	{
		Iterator temp(*this);
		operator--();
		return temp;
	}
	Iterator& operator+=(const Iterator& rhs)
	{
		itIndex += rhs.itIndex;
		assert((itData == rhs.itData) && (itIndex <= itSize));
		return *this;
	}
	Iterator& operator+=(size_t rhs)
	{
		itIndex += rhs;
		assert(itIndex <= itSize);
		return *this;
	}
	Iterator& operator-=(const Iterator& rhs)
	{
		itIndex -= rhs.itIndex;
		assert((itIndex >= 0) && (itData == rhs.itData));
		return *this;
	}
	Iterator& operator-=(size_t rhs)
	{
		itIndex -= rhs;
		assert(itIndex >= 0);
		return *this;
	}
	Iterator operator+(const Iterator& rhs)
	{
		Iterator lhs = *this;
		lhs += rhs;
		return lhs;
	}
	Iterator operator+(int rhs)
	{
		Iterator lhs = *this;
		lhs += rhs;
		return lhs;
	}
	friend Iterator operator+(int lhs, Iterator rhs)
	{
		rhs += lhs;
		return rhs;
	}
	Iterator operator-(const Iterator& rhs)
	{
		Iterator lhs = *this;
		lhs -= rhs;
		return lhs;
	}
	Iterator operator-(int rhs)
	{
		Iterator lhs = *this;
		lhs -= rhs;
		return lhs;
	}
	friend Iterator operator-(int lhs, Iterator rhs)
	{
		rhs -= lhs;
		return rhs;
	}
	T operator*()
	{
		assert((itIndex < itSize) && (itData != nullptr));
		return itData[itIndex];
	}
	T operator[](size_t indexIn)
	{
		return *(this->operator+(indexIn));
	}
	friend bool operator==(const Iterator& lhs, const Iterator& rhs)
	{
		assert(lhs.itData == rhs.itData);
		return (lhs.itIndex == rhs.itIndex);
	}
	friend bool operator!=(const Iterator& lhs, const Iterator& rhs)
	{
		return !(lhs == rhs);
	}
	//NOTE: If you need other compares, make them!
};

template <typename T>
class ConstIterator
{
	size_t itIndex;
	size_t itSize;
	T* itData;
public:
	ConstIterator(size_t indexIn, size_t sizeIn, T* dataIn) : itIndex(indexIn), itSize(sizeIn), itData(dataIn) {}

	ConstIterator& operator++()
	{
		++itIndex;
		assert(itIndex <= itSize);
		return *this;
	}
	ConstIterator operator++(int)
	{
		ConstIterator temp(*this);
		operator++();
		return temp;
	}
	ConstIterator& operator--()
	{
		--itIndex;
		assert(itIndex >= 0);
		return *this;
	}
	ConstIterator operator--(int)
	{
		ConstIterator temp(*this);
		operator--();
		return temp;
	}
	ConstIterator& operator+=(const ConstIterator& rhs)
	{
		itIndex += rhs.itIndex;
		assert((itData == rhs.itData) && (itIndex <= itSize));
		return *this;
	}
	ConstIterator& operator+=(size_t rhs)
	{
		itIndex += rhs;
		assert(itIndex <= itSize);
		return *this;
	}
	ConstIterator& operator-=(const ConstIterator& rhs)
	{
		itIndex -= rhs.itIndex;
		assert((itIndex >= 0) && (itData == rhs.itData));
		return *this;
	}
	ConstIterator& operator-=(size_t rhs)
	{
		itIndex -= rhs;
		assert(itIndex >= 0);
		return *this;
	}
	ConstIterator operator+(const ConstIterator& rhs)
	{
		ConstIterator lhs = *this;
		lhs += rhs;
		return lhs;
	}
	ConstIterator operator+(int rhs)
	{
		ConstIterator lhs = *this;
		lhs += rhs;
		return lhs;
	}
	friend ConstIterator operator+(int lhs, ConstIterator rhs)
	{
		rhs += lhs;
		return rhs;
	}
	ConstIterator operator-(const ConstIterator& rhs)
	{
		ConstIterator lhs = *this;
		lhs -= rhs;
		return lhs;
	}
	ConstIterator operator-(int rhs)
	{
		ConstIterator lhs = *this;
		lhs -= rhs;
		return lhs;
	}
	friend ConstIterator operator-(int lhs, ConstIterator rhs)
	{
		rhs -= lhs;
		return rhs;
	}
	const T operator*()
	{
		assert((itIndex < itSize) && (itData != nullptr));
		return itData[itIndex];
	}
	const T operator[](size_t indexIn)
	{
		return *(this->operator+(indexIn));
	}
	friend bool operator==(const ConstIterator& lhs, const ConstIterator& rhs)
	{
		assert(lhs.itData == rhs.itData);
		return (lhs.itIndex == rhs.itIndex);
	}
	friend bool operator!=(const ConstIterator& lhs, const ConstIterator& rhs)
	{
		return !(lhs == rhs);
	}
	//NOTE: If you need other compares, make them!
};
#else
//TODO: Just return the pointer in release, or?!
#endif

#define DEFINE_ARRAY_DATA auto arrayData = getRightPointer(); assert(arrayData != nullptr)

template <typename T, size_t N>
struct Array
{
	union
	{
		T arrayValue[N] = { 0 };
		struct
		{
			T* p = nullptr;
			size_t capacity = 0;
		} heapArray;
	} arrayUnion;

	size_t arraySize = 0;
private:
	constexpr T* getRightPointer();
public:
	~Array();

	T& at(size_t pos);
	const T& at(size_t pos) const;
	T& operator[](size_t pos);
	const T& operator[](size_t pos) const;

	T& front();
	const T& front() const;
	T& back();
	const T& back() const;
	T* data();
	const T* data() const;

	Iterator<T> begin();
	Iterator<T> end();

	ConstIterator<T> begin() const;
	ConstIterator<T> end() const;

	size_t capacity() const;
	size_t size() const;
	bool empty() const;

	void clear();
	Iterator<T> insert(size_t pos, const T& value);
	Iterator<T> insert(const Iterator<T>& pos, const T& value);
	Iterator<T> insert(size_t pos, size_t count, const T& value);
	Iterator<T> insert(size_t pos, T&& value);
	Iterator<T> insert(const Iterator<T>& pos, size_t count, const T& value);
	Iterator<T> insert(const Iterator<T>& pos, T&& value);
	//NOTE: Do theses functions even make sense??
	Iterator<T> insertPush_back(size_t pos, const T& value);
	Iterator<T> insertPush_back(const Iterator<T>& pos, const T& value);
	Iterator<T> insertPush_back(size_t pos, T&& value);
	Iterator<T> insertPush_back(const Iterator<T>& pos, T&& value);
	Iterator<T> erase(size_t pos);
	Iterator<T> erase(const Iterator<T>& pos);
	Iterator<T> erase(size_t first, size_t last);
	Iterator<T> erase(const Iterator<T>& first, const Iterator<T>& last);
	Iterator<T> erasePop_back(size_t pos);
	Iterator<T> erasePop_back(const Iterator<T>& pos);
	void push_back(const T& value);
	void push_back(T&& value);
	void pop_back();

	//TODO: Think about how to implement compares!
};

template<typename T, size_t N>
inline T & Array<T, N>::at(size_t pos)
{
	DEFINE_ARRAY_DATA;

	assert((pos < arraySize) && (pos >= 0));
	return arrayData[pos];
}

template<typename T, size_t N>
inline const T & Array<T, N>::at(size_t pos) const
{
	DEFINE_ARRAY_DATA;

	assert((pos < arraySize) && (pos >= 0));
	return arrayData[pos];
}

template<typename T, size_t N>
inline T & Array<T, N>::operator[](size_t pos)
{
	return at(pos);
}

template<typename T, size_t N>
inline const T & Array<T, N>::operator[](size_t pos) const
{
	return at(pos);
}

template<typename T, size_t N>
inline T & Array<T, N>::front()
{
	DEFINE_ARRAY_DATA;

	assert(arraySize >= 1);
	return arrayData[0];
}

template <typename T, size_t N>
inline const T & Array<T, N>::front() const
{
	DEFINE_ARRAY_DATA;

	assert(arraySize >= 1);
	return arrayData[0];
}

template <typename T, size_t N>
inline T & Array<T, N>::back()
{
	DEFINE_ARRAY_DATA;

	assert(arraySize >= 1);
	return arrayData[arraySize - 1];
}

template <typename T, size_t N>
inline const T & Array<T, N>::back() const
{
	DEFINE_ARRAY_DATA;

	assert(arraySize >= 1);
	return arrayData[arraySize - 1];
}

template <typename T, size_t N>
inline T * Array<T, N>::data()
{
	DEFINE_ARRAY_DATA;

	return arrayData;
}

template <typename T, size_t N>
inline const T * Array<T, N>::data() const
{
	DEFINE_ARRAY_DATA;

	return arrayData;
}

template <typename T, size_t N>
inline Iterator<T> Array<T, N>::begin()
{
	DEFINE_ARRAY_DATA;

	return Iterator<T>{ 0, arraySize, arrayData };
}

template <typename T, size_t N>
inline Iterator<T> Array<T, N>::end()
{
	DEFINE_ARRAY_DATA;

	return Iterator<T>{ arraySize, arraySize, arrayData };
}

template <typename T, size_t N>
inline ConstIterator<T> Array<T, N>::begin() const
{
	DEFINE_ARRAY_DATA;

	return ConstIterator<T>{ 0, arraySize, arrayData };
}

template <typename T, size_t N>
inline ConstIterator<T> Array<T, N>::end() const
{
	DEFINE_ARRAY_DATA;

	return ConstIterator<T>{ arraySize, arraySize, arrayData };
}

template <typename T, size_t N>
inline bool Array<T, N>::empty() const
{
	return (arraySize == 0);
}

template <typename T, size_t N>
inline size_t Array<T, N>::size() const
{
	return arraySize;
}

template <typename T, size_t N>
inline size_t Array<T, N>::capacity() const
{
	if constexpr (N == 0)
		return arrayUnion.heapArray.capacity;
	else
		return N;
}

template <typename T, size_t N>
inline void Array<T, N>::clear()
{
	DEFINE_ARRAY_DATA;

	for (size_t i = 0; i < arraySize; ++i)
	{
		arrayData[i].~T();
	}
}

template <typename T, size_t N>
inline Iterator<T> Array<T, N>::insert(size_t pos, const T & value)
{
	DEFINE_ARRAY_DATA;

	assert((pos <= arraySize) && (pos < capacity()) && (pos >= 0));

	++arraySize;
	if (pos != (arraySize - 1))
	{
		new (&arrayData[arraySize - 1]) T(std::move(arrayData[arraySize - 2]));
		for (size_t i = (arraySize - 2); i > pos; --i)
		{
			arrayData[i] = std::move(arrayData[i - 1]);
		}
	}
	arrayData[pos] = value;

	return Iterator<T>{ pos, arraySize, arrayData };
}

template <typename T, size_t N>
inline Iterator<T> Array<T, N>::insert(const Iterator<T> & pos, const T & value)
{
	return insert(pos.itIndex, value);
}

template <typename T, size_t N>
inline Iterator<T> Array<T, N>::insert(size_t pos, T && value)
{
	DEFINE_ARRAY_DATA;

	assert((pos <= arraySize) && (pos < capacity()) && (pos >= 0));

	++arraySize;
	if (pos != (arraySize - 1))
	{
		new (&arrayData[arraySize - 1]) T(std::move(arrayData[arraySize - 2]));
		for (size_t i = (arraySize - 2); i > pos; --i)
		{
			arrayData[i] = std::move(arrayData[i - 1]);
		}
	}
	arrayData[pos] = std::move(value);

	return Iterator<T>{ pos, arraySize, arrayData };
}

template <typename T, size_t N>
inline Iterator<T> Array<T, N>::insert(const Iterator<T> & pos, T && value)
{
	return insert(pos.itIndex, std::move(value));
}

template <typename T, size_t N>
inline Iterator<T> Array<T, N>::insert(size_t pos, size_t count, const T & value)
{
	DEFINE_ARRAY_DATA;

	assert((pos <= arraySize) && (count > 0) && ((pos + (count - 1)) < capacity()) && (pos >= 0));

	if (pos != arraySize)
	{
		//TODO: Isn`t is better if you shift the buckets up by there amount, so that I do not get O(n²)?
		for (size_t j = 0; j < count; ++j, ++pos, ++arraySize)
		{
			new (&arrayData[arraySize]) T(std::move(arrayData[arraySize - 1]));
			for (size_t i = (arraySize - 1); i > pos; --i)
			{
				arrayData[i] = std::move(arrayData[i - 1]);
			}
		}
	}
	else
	{
		pos += count;
		arraySize += count;
	}

	for (size_t i = (pos - count); i < pos; ++i)
	{
		arrayData[i] = value;
	}

	return Iterator<T>{ pos - count, arraySize, arrayData };
}

template <typename T, size_t N>
inline Iterator<T> Array<T, N>::insert(const Iterator<T> & pos, size_t count, const T & value)
{
	return insert(pos.itIndex, count, value);
}

template <typename T, size_t N>
inline Iterator<T> Array<T, N>::insertPush_back(size_t pos, const T & value)
{
	DEFINE_ARRAY_DATA;

	assert((pos <= arraySize) && (pos < capacity()) && (pos >= 0));

	new (&arrayData[arraySize]) T(std::move(arrayData[pos]));
	arrayData[pos] = value;
	++arraySize;

	return Iterator<T>(pos, arraySize, arrayData);
}

template <typename T, size_t N>
inline Iterator<T> Array<T, N>::insertPush_back(const Iterator<T> & pos, const T & value)
{
	return insertPush_back(pos.itIndex, value);
}

template <typename T, size_t N>
inline Iterator<T> Array<T, N>::insertPush_back(size_t pos, T && value)
{
	DEFINE_ARRAY_DATA;

	assert((pos <= arraySize) && (pos < capacity()) && (pos >= 0));

	new (&arrayData[arraySize]) T(std::move(arrayData[pos]));
	arrayData[pos] = std::move(value);
	++arraySize;

	return Iterator<T>(pos, arraySize, arrayData);
}

template <typename T, size_t N>
inline Iterator<T> Array<T, N>::insertPush_back(const Iterator<T> & pos, T && value)
{
	return insertPush_back(pos.itIndex, std::move(value));
}

template <typename T, size_t N>
inline Iterator<T> Array<T, N>::erase(size_t pos)
{
	DEFINE_ARRAY_DATA;

	assert((pos < arraySize) && (pos < capacity()) && (pos >= 0));

	arrayData[pos].~T();
	for (size_t i = (pos + 1); i < arraySize; ++i)
	{
		arrayData[i - 1] = std::move(arrayData[i]);
	}

	--arraySize;

	return Iterator<T>{ pos, arraySize, arrayData };
}

template <typename T, size_t N>
inline Iterator<T> Array<T, N>::erase(const Iterator<T> & pos)
{
	return erase(pos.itIndex);
}

template <typename T, size_t N>
inline Iterator<T> Array<T, N>::erase(size_t first, size_t last)
{
	DEFINE_ARRAY_DATA;

	assert((first < arraySize) && (last <= arraySize) && (first >= 0) && (last >= 0) && (first < last));

	for (size_t i = first; i < last; ++i)
	{
		arrayData[i].~T();
	}
	size_t newarraySize = arraySize;
	//TODO: Isn`t is better if you shift the buckets down by there amount, so that I do not get O(n²)?
	for (size_t j = last; j > first; --j, --newarraySize)
	{
		for (size_t i = j; i < newarraySize; ++i)
		{
			arrayData[i - 1] = arrayData[i];
		}
	}

	arraySize = newarraySize;

	return Iterator<T>{ first, arraySize, arrayData };
}

template <typename T, size_t N>
inline Iterator<T> Array<T, N>::erase(const Iterator<T> & first, const Iterator<T> & last)
{
	return erase(first.itIndex, last.itIndex);
}

template <typename T, size_t N>
inline Iterator<T> Array<T, N>::erasePop_back(size_t pos)
{
	DEFINE_ARRAY_DATA;

	assert((pos < arraySize) && (pos >= 0));

	arrayData[pos].~T();
	arrayData[pos] = std::move(arrayData[arraySize - 1]);

	--arraySize;

	return Iterator<T>{ pos, arraySize, arrayData };
}

template <typename T, size_t N>
inline Iterator<T> Array<T, N>::erasePop_back(const Iterator<T> & pos)
{
	return erasePop_back(pos.itIndex);
}

template <typename T, size_t N>
inline void Array<T, N>::push_back(const T & value)
{
	DEFINE_ARRAY_DATA;

	assert((arraySize + 1) < capacity());

	new (&arrayData[arraySize++]) T(value);
}

template <typename T, size_t N>
inline void Array<T, N>::push_back(T && value)
{
	DEFINE_ARRAY_DATA;

	assert((arraySize + 1) < capacity());

	new (&arrayData[arraySize++]) T(std::move(value));
}

template <typename T, size_t N>
inline void Array<T, N>::pop_back()
{
	DEFINE_ARRAY_DATA;

	assert((arraySize - 1) >= 0);

	arrayData[--arraySize].~T();
}

template<typename T, size_t N>
inline Array<T, N>::~Array()
{
	if constexpr (N == 0)
	{
		if (arrayUnion.heapArray.p != nullptr)
		{
			for (size_t i = 0; i < this->arraySize; ++i)
				arrayUnion.heapArray.p[i].~T();

			free(arrayUnion.heapArray.p);
			arrayUnion.heapArray.p = nullptr;
		}
	}
}

template<typename T, size_t N>
inline constexpr T * Array<T, N>::getRightPointer()
{
	if constexpr (N == 0)
		return arrayUnion.heapArray.p;
	else
		return arrayUnion.arrayValue;
}

namespace ArrayTestSuit
{
	void runTestSuit();
}
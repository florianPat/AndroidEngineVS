#pragma once

//Other std implementations TODO: unordered_map, unique_ptr

#include "HeapArray.h"

template <typename T>
class Vector
{
	HeapArray<T> vectorArray;
private:
	void expandOrShrink(size_t newCapacity);

	void checkAndShrink(size_t newSize);
	void checkAndExpand(size_t minNewSize);
public:
	Vector();
	Vector(size_t count);
	Vector(size_t count, const T& value);
	Vector(std::initializer_list<T> initList);
	Vector(const Vector& other);
	Vector& operator=(const Vector& rhs);
	Vector(Vector&& other);
	Vector& operator=(Vector&& rhs);

	inline T& at(size_t pos);
	inline const T& at(size_t pos) const;
	inline T& operator[](size_t pos);
	inline const T& operator[](size_t pos) const;

	inline T& front();
	inline const T& front() const;
	inline T& back();
	inline const T& back() const;
	inline T* data();
	inline const T* data() const;

	inline Iterator<T> begin();
	inline Iterator<T> end();

	inline ConstIterator<T> begin() const;
	inline ConstIterator<T> end() const;

	inline bool empty() const;
	inline size_t size() const;

	void reserve(size_t size);
	inline size_t capacity() const;

	void clear();
	Iterator<T> insert(size_t pos, const T& value);
	Iterator<T> insert(const Iterator<T>& pos, const T& value);
	Iterator<T> insert(size_t pos, T&& value);
	Iterator<T> insert(const Iterator<T>& pos, T&& value);
	Iterator<T> insert(size_t pos, size_t count, const T& value);
	Iterator<T> insert(const Iterator<T>& pos, size_t count, const T& value);
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
	void resize(size_t count);
	void resize(size_t count, const T& value);
	void swap(Vector& other);
	void swap(size_t first, size_t second);
	void swap(const Iterator<T>& first, const Iterator<T>& second);

	bool operator==(const Vector& rhs) const;
	bool operator!=(const Vector& rhs) const;
	//NOTE: If you need other compares, make them!
};

template<typename T>
inline void Vector<T>::expandOrShrink(size_t newCapacity)
{
	HeapArray<T> newVectorArray(std::move(vectorArray), newCapacity);
	vectorArray = std::move(newVectorArray);

	/*T* newData = (T*) malloc(sizeof(T) * vectorCapacity);
	for (size_t i = 0; i < vectorSize; ++i)
	{
		new (&newData[i]) T(std::move(vectorData[i]));
	}
	free(vectorData);
	vectorData = newData;*/
}

template<typename T>
inline void Vector<T>::checkAndShrink(size_t newSize)
{
	size_t vectorCapacity = capacity();

	if ((vectorCapacity / 2) >= (newSize + 2))
	{
		do
		{
			vectorCapacity /= 2;
		} while ((vectorCapacity / 2) >= (newSize + 2));

		expandOrShrink(vectorCapacity);
	}
}

template<typename T>
inline void Vector<T>::checkAndExpand(size_t minNewSize)
{
	size_t vectorCapacity = capacity();

	if (vectorCapacity < minNewSize)
	{
		do
		{
			vectorCapacity *= 2.0f;
		} while (vectorCapacity < minNewSize);

		expandOrShrink(vectorCapacity);
	}
}

template<typename T>
inline Vector<T>::Vector() : vectorArray(2)
{
}

template<typename T>
inline Vector<T>::Vector(size_t count) : vectorArray(count)
{
}

template<typename T>
inline Vector<T>::Vector(size_t count, const T & value) : vectorArray(count, 4, value)
{
}

template<typename T>
inline Vector<T>::Vector(std::initializer_list<T> initList) : vectorArray(initList.size() + 4)
{
	for (auto it = initList.begin(); it != initList.end(); ++it)
	{
		vectorArray.push_back(*it);
	}
}

template<typename T>
inline Vector<T>::Vector(const Vector& other) : vectorArray(other.vectorArray)
{
}

template<typename T>
inline Vector<T>& Vector<T>::operator=(const Vector& rhs)
{
	vectorArray = rhs.vectorArray;

	return *this;
}

template<typename T>
inline Vector<T>::Vector(Vector && other) : vectorArray(std::move(other.vectorArray))
{
}

template<typename T>
inline Vector<T>& Vector<T>::operator=(Vector && rhs)
{
	vectorArray = std::move(rhs.vectorArray);

	return *this;
}

template<typename T>
inline T & Vector<T>::at(size_t pos)
{
	return vectorArray.at(pos);
}

template<typename T>
inline const T & Vector<T>::at(size_t pos) const
{
	return vectorArray.at(pos);
}

template<typename T>
inline T & Vector<T>::operator[](size_t pos)
{
	return vectorArray[pos];
}

template<typename T>
inline const T & Vector<T>::operator[](size_t pos) const
{
	return vectorArray[pos];
}

template<typename T>
inline T & Vector<T>::front()
{
	return vectorArray.front();
}

template<typename T>
inline const T & Vector<T>::front() const
{
	return vectorArray.front();
}

template<typename T>
inline T & Vector<T>::back()
{
	return vectorArray.back();
}

template<typename T>
inline const T & Vector<T>::back() const
{
	return vectorArray.back();
}

template<typename T>
inline T * Vector<T>::data()
{
	return vectorArray.data();
}

template<typename T>
inline const T * Vector<T>::data() const
{
	return vectorArray.data();
}

template<typename T>
inline Iterator<T> Vector<T>::begin()
{
	return vectorArray.begin();
}

template<typename T>
inline Iterator<T> Vector<T>::end()
{
	return vectorArray.end();
}

template<typename T>
inline ConstIterator<T> Vector<T>::begin() const
{
	return vectorArray.begin();
}

template<typename T>
inline ConstIterator<T> Vector<T>::end() const
{
	return vectorArray.end();
}

template<typename T>
inline bool Vector<T>::empty() const
{
	return vectorArray.empty();
}

template<typename T>
inline size_t Vector<T>::size() const
{
	return vectorArray.size();
}

template<typename T>
inline void Vector<T>::reserve(size_t newSize)
{
	//NOTE: Include an upper bound?
	assert(newSize >= 0);

	checkAndExpand(newSize);
}

template<typename T>
inline size_t Vector<T>::capacity() const
{
	return vectorArray.capacity();
}

template<typename T>
inline void Vector<T>::clear()
{
	vectorArray.clear();
}

template<typename T>
inline Iterator<T> Vector<T>::insert(size_t pos, const T & value)
{
	checkAndExpand(size() + 1);

	return vectorArray.insert(pos, value);
}

template<typename T>
inline Iterator<T> Vector<T>::insert(const Iterator<T> & pos, const T & value)
{
	return vectorArray.insert(pos, value);
}

template<typename T>
inline Iterator<T> Vector<T>::insert(size_t pos, T && value)
{
	checkAndExpand(size() + 1);

	return vectorArray.insert(pos, std::move(value));
}

template<typename T>
inline Iterator<T> Vector<T>::insert(const Iterator<T> & pos, T && value)
{
	return vectorArray.insert(pos, std::move(value));
}

template<typename T>
inline Iterator<T> Vector<T>::insert(size_t pos, size_t count, const T & value)
{
	checkAndExpand(size() + count);

	return vectorArray.insert(pos, count, value);
}

template<typename T>
inline Iterator<T> Vector<T>::insert(const Iterator<T> & pos, size_t count, const T & value)
{
	return vectorArray.insert(pos, count, value);
}

template<typename T>
inline Iterator<T> Vector<T>::insertPush_back(size_t pos, const T & value)
{
	checkAndExpand(size() + 1);

	return vectorArray.insertPush_back(pos, value);
}

template<typename T>
inline Iterator<T> Vector<T>::insertPush_back(const Iterator<T> & pos, const T & value)
{
	return vectorArray.insertPush_back(pos, value);
}

template<typename T>
inline Iterator<T> Vector<T>::insertPush_back(size_t pos, T && value)
{
	checkAndExpand(size() + 1);

	return vectorArray.insertPush_back(pos, std::move(value));
}

template<typename T>
inline Iterator<T> Vector<T>::insertPush_back(const Iterator<T> & pos, T && value)
{
	return vectorArray.insertPush_back(pos, std::move(value));
}

template<typename T>
inline Iterator<T> Vector<T>::erase(size_t pos)
{
	auto result = vectorArray.erase(pos);

	checkAndShrink(size());

	return result;
}

template<typename T>
inline Iterator<T> Vector<T>::erase(const Iterator<T> & pos)
{
	return vectorArray.erase(pos);
}

template<typename T>
inline Iterator<T> Vector<T>::erase(size_t first, size_t last)
{
	auto result = vectorArray.erase(first, last);

	checkAndShrink(size());

	return result;
}

template<typename T>
inline Iterator<T> Vector<T>::erase(const Iterator<T> & first, const Iterator<T> & last)
{
	return vectorArray.erase(first, last);
}

template<typename T>
inline Iterator<T> Vector<T>::erasePop_back(size_t pos)
{
	auto result = vectorArray.erasePop_back(pos);

	checkAndShrink(size());

	return result;
}

template<typename T>
inline Iterator<T> Vector<T>::erasePop_back(const Iterator<T> & pos)
{
	return vectorArray.erasePop_back(pos);
}

template<typename T>
inline void Vector<T>::push_back(const T & value)
{
	checkAndExpand(size() + 1);

	vectorArray.push_back(value);
}

template<typename T>
inline void Vector<T>::push_back(T && value)
{
	checkAndExpand(size() + 1);

	vectorArray.push_back(std::move(value));
}

template<typename T>
inline void Vector<T>::pop_back()
{
	vectorArray.pop_back();

	checkAndShrink(size());
}

template<typename T>
inline void Vector<T>::resize(size_t count)
{
	//NOTE: Include an upper bound?
	assert(count >= 0);

	size_t vectorSize = size();

	if (count < vectorSize)
	{
		vectorArray.erase(count, vectorSize);

		checkAndShrink(count);
	}
	else
	{
		checkAndExpand(count);

		vectorArray.insert(vectorSize, (count - vectorSize), T());
	}
}

template<typename T>
inline void Vector<T>::resize(size_t count, const T & value)
{
	//NOTE: Include an upper bound?
	assert(count >= 0);

	size_t vectorSize = size();

	if (count < vectorSize)
	{
		vectorArray.erase(count, vectorSize);

		checkAndShrink(count);
	}
	else
	{
		checkAndExpand(count);

		vectorArray.insert(vectorSize, count, value);
	}
}

template<typename T>
inline void Vector<T>::swap(Vector & other)
{
	Vector<T> temp(std::move(other));
	other = std::move(*this);
	*this = std::move(temp);
}

template<typename T>
inline void Vector<T>::swap(size_t first, size_t second)
{
	assert(first < vectorArray.size() && second < vectorArray.size());

	T&& temp = std::move(vectorArray[second]);
	vectorArray[second] = std::move(vectorArray[first]);
	vectorArray[first] = std::move(temp);
}

template<typename T>
inline void Vector<T>::swap(const Iterator<T>& first, const Iterator<T>& second)
{
	return swap(first.getIndex(), second.getIndex());
}

template<typename T>
inline bool Vector<T>::operator==(const Vector& rhs) const
{
	return (vectorArray == rhs.vectorArray);
}

template<typename T>
inline bool Vector<T>::operator!=(const Vector& rhs) const
{
	return (vectorArray != rhs.vectorArray);
}

namespace VectorTestSuit
{
	void runVectorUnitTest();
	void runStdVectorTest();
}
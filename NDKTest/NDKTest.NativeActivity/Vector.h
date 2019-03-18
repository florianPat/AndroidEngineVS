#pragma once

//Other std implementations TODO: string, unordered_map, unique_ptr, map/multimap

#include <initializer_list>
#include "Utils.h"
//NOTE: For std::move
#include <utility>

template <typename T>
class Vector
{
	/* NOTE: Undefinied behavour if you dereference or do other things if Its are invalidated due to a shrink or expand call
	*  This is also a "safe" iterator (it does bounds checking on current size)
	* NOTE: Maybe add a reference count to vector and fix itData pointers through storing a pp? (Performance further down?)
	*/
	class Iterator
	{
		friend class Vector;

		size_t itIndex;
		size_t itSize;
		T* itData;

		Iterator(size_t indexIn, size_t sizeIn, T* dataIn) : itIndex(indexIn), itSize(sizeIn), itData(dataIn) {}
	public:
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
		friend Iterator operator+(Iterator lhs, const Iterator& rhs)
		{
			lhs += rhs;
			return lhs;
		}
		friend Iterator operator+(Iterator lhs, int rhs)
		{
			lhs += rhs;
			return lhs;
		}
		friend Iterator operator+(int lhs, Iterator rhs)
		{
			rhs += lhs;
			return rhs;
		}
		friend Iterator operator-(Iterator lhs, const Iterator& rhs)
		{
			lhs -= rhs;
			return lhs;
		}
		friend Iterator operator-(Iterator lhs, int rhs)
		{
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
		T* operator->()
		{
			assert((itIndex < itSize) && (itData != nullptr));
			return &itData[itIndex];
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

	class ConstIterator
	{
		friend class Vector;

		size_t itIndex;
		size_t itSize;
		T* itData;

		ConstIterator(size_t indexIn, size_t sizeIn, T* dataIn) : itIndex(indexIn), itSize(sizeIn), itData(dataIn) {}
	public:
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
		friend ConstIterator operator+(ConstIterator lhs, const ConstIterator& rhs)
		{
			lhs += rhs;
			return lhs;
		}
		friend ConstIterator operator+(ConstIterator lhs, int rhs)
		{
			lhs += rhs;
			return lhs;
		}
		friend ConstIterator operator+(int lhs, ConstIterator rhs)
		{
			rhs += lhs;
			return rhs;
		}
		friend ConstIterator operator-(ConstIterator lhs, const ConstIterator& rhs)
		{
			lhs -= rhs;
			return lhs;
		}
		friend ConstIterator operator-(ConstIterator lhs, int rhs)
		{
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
		const T* operator->()
		{
			assert((itIndex < itSize) && (itData != nullptr));
			return &itData[itIndex];
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
private:
	size_t vectorSize = 0;
	size_t vectorCapacity = 2;
	T* vectorData = nullptr;
private:
	void expandOrShrink();

	void checkAndShrink(size_t newSize);
	void checkAndExpand(size_t minNewSize);
public:
	Vector();
	Vector(size_t count, const T& value = T());
	Vector(std::initializer_list<T> initList);
	Vector(const Vector& other);
	Vector& operator=(const Vector& rhs);
	Vector(Vector&& other);
	Vector& operator=(Vector&& rhs);
	~Vector();

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

	Iterator begin();
	Iterator end();

	ConstIterator begin() const;
	ConstIterator end() const;

	bool empty() const;
	size_t size() const;

	void reserve(size_t size);
	size_t capacity() const;

	void clear();
	Iterator insert(size_t pos, const T& value);
	Iterator insert(const Iterator& pos, const T& value);
	Iterator insert(size_t pos, T&& value);
	Iterator insert(const Iterator& pos, T&& value);
	Iterator insert(size_t pos, size_t count, const T& value);
	Iterator insert(const Iterator& pos, size_t count, const T& value);
	//NOTE: Do theses functions even make sense??
	Iterator insertPush_back(size_t pos, const T& value);
	Iterator insertPush_back(const Iterator& pos, const T& value);
	Iterator insertPush_back(size_t pos, T&& value);
	Iterator insertPush_back(const Iterator& pos, T&& value);
	Iterator erase(size_t pos);
	Iterator erase(const Iterator& pos);
	Iterator erase(size_t first, size_t last);
	Iterator erase(const Iterator& first, const Iterator& last);
	Iterator erasePop_back(size_t pos);
	Iterator erasePop_back(const Iterator& pos);
	void push_back(const T& value);
	void push_back(T&& value);
	void pop_back();
	void resize(size_t count);
	void resize(size_t count, const T& value);
	void swap(Vector& other);

	bool operator==(Vector& rhs);
	bool operator!=(Vector& rhs);
	//NOTE: If you need other compares, make them!
};

template<typename T>
inline void Vector<T>::expandOrShrink()
{
	T* newData = (T*) malloc(sizeof(T) * vectorCapacity);
	for (size_t i = 0; i < vectorSize; ++i)
	{
		new (&newData[i]) T(std::move(vectorData[i]));
	}
	free(vectorData);
	vectorData = newData;
}

template<typename T>
inline void Vector<T>::checkAndShrink(size_t newSize)
{
	if ((vectorCapacity / 2) >= (newSize + 2))
	{
		do
		{
			vectorCapacity /= 2;
		} while ((vectorCapacity / 2) >= (newSize + 2));

		expandOrShrink();
	}
}

template<typename T>
inline void Vector<T>::checkAndExpand(size_t minNewSize)
{
	if (vectorCapacity < minNewSize)
	{
		do
		{
			vectorCapacity *= 2.0f;
		} while (vectorCapacity < minNewSize);

		expandOrShrink();
	}
}

template<typename T>
inline Vector<T>::Vector() : vectorData((T*) malloc(sizeof(T) * vectorCapacity))
{
}

template<typename T>
inline Vector<T>::Vector(size_t count, const T & value) : vectorSize(count), vectorCapacity(count + 4),
vectorData((T*) malloc(sizeof(T) * vectorCapacity))
{
	for (size_t i = 0; i < vectorSize; ++i)
	{
		new (&vectorData[i]) T(value);
	}
}

template<typename T>
inline Vector<T>::Vector(std::initializer_list<T> initList) : vectorSize(initList.size()), vectorCapacity(initList.size() + 4),
vectorData((T*) malloc(sizeof(T) * vectorCapacity))
{
	size_t i = 0;
	for (auto it = initList.begin(); it != initList.end(); ++it)
	{
		new (&vectorData[i++]) T(*it);
	}
}

template<typename T>
inline Vector<T>::Vector(const Vector& other) : vectorSize(other.vectorSize), vectorCapacity(other.vectorCapacity),
vectorData((T*) malloc(sizeof(T) * vectorCapacity))
{
	for (size_t i = 0; i < vectorSize; ++i)
		new (&vectorData[i]) T(other.vectorData[i]);
}

template<typename T>
inline Vector<T>& Vector<T>::operator=(const Vector& rhs)
{
	this->~Vector();

	vectorCapacity = rhs.vectorCapacity;
	vectorSize = rhs.vectorSize;
	vectorData = (T*) malloc(sizeof(T) * vectorCapacity);

	for (size_t i = 0; i < vectorSize; ++i)
	{
		new (&vectorData[i]) T(rhs.vectorData[i]);
	}

	return *this;
}

template<typename T>
inline Vector<T>::Vector(Vector && other) : vectorSize(std::exchange(other.vectorSize, 0)), vectorCapacity(std::exchange(other.vectorCapacity, 0)),
vectorData(std::exchange(other.vectorData, nullptr))
{
}

template<typename T>
inline Vector<T>& Vector<T>::operator=(Vector && rhs)
{
	this->~Vector();

	vectorSize = std::exchange(rhs.vectorSize, 0);
	vectorCapacity = std::exchange(rhs.vectorCapacity, 0);
	vectorData = std::exchange(rhs.vectorData, nullptr);

	return *this;
}

template<typename T>
inline Vector<T>::~Vector()
{
	vectorCapacity = 0;
	for (size_t i = 0; i < vectorSize; ++i)
	{
		vectorData[i].~T();
	}
	free(vectorData);
	vectorData = nullptr;
	vectorSize = 0;
}

template<typename T>
inline T & Vector<T>::at(size_t pos)
{
	assert((pos < vectorSize) && (pos >= 0));
	return vectorData[pos];
}

template<typename T>
inline const T & Vector<T>::at(size_t pos) const
{
	assert((pos < vectorSize) && (pos >= 0));
	return vectorData[pos];
}

template<typename T>
inline T & Vector<T>::operator[](size_t pos)
{
	return at(pos);
}

template<typename T>
inline const T & Vector<T>::operator[](size_t pos) const
{
	return at(pos);
}

template<typename T>
inline T & Vector<T>::front()
{
	assert(vectorSize >= 1);
	return vectorData[0];
}

template<typename T>
inline const T & Vector<T>::front() const
{
	assert(vectorSize >= 1);
	return vectorData[0];
}

template<typename T>
inline T & Vector<T>::back()
{
	assert(vectorSize >= 1);
	return vectorData[vectorSize - 1];
}

template<typename T>
inline const T & Vector<T>::back() const
{
	assert(vectorSize >= 1);
	return vectorData[vectorSize - 1];
}

template<typename T>
inline T * Vector<T>::data()
{
	return vectorData;
}

template<typename T>
inline const T * Vector<T>::data() const
{
	return vectorData;
}

template<typename T>
inline typename Vector<T>::Iterator Vector<T>::begin()
{
	return Iterator{ 0, vectorSize, vectorData };
}

template<typename T>
inline typename Vector<T>::Iterator Vector<T>::end()
{
	return Iterator{ vectorSize, vectorSize, vectorData };
}

template<typename T>
inline typename Vector<T>::ConstIterator Vector<T>::begin() const
{
	return ConstIterator{ 0, vectorSize, vectorData };
}

template<typename T>
inline typename Vector<T>::ConstIterator Vector<T>::end() const
{
	return ConstIterator{ vectorSize, vectorSize, vectorData };
}

template<typename T>
inline bool Vector<T>::empty() const
{
	return (vectorSize == 0);
}

template<typename T>
inline size_t Vector<T>::size() const
{
	return vectorSize;
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
	return vectorCapacity;
}

template<typename T>
inline void Vector<T>::clear()
{
	for (size_t i = 0; i < vectorSize; ++i)
	{
		vectorData[i].~T();
	}
}

template<typename T>
inline typename Vector<T>::Iterator Vector<T>::insert(size_t pos, const T & value)
{
	assert((pos <= vectorSize) && (pos >= 0));

	checkAndExpand(vectorSize + 1);

	++vectorSize;
	new (&vectorData[vectorSize - 1]) T(std::move(vectorData[vectorSize - 2]));
	for (size_t i = (vectorSize - 2); i > pos; --i)
	{
		vectorData[i] = std::move(vectorData[i - 1]);
	}
	vectorData[pos] = value;

	return Iterator{ pos, vectorSize, vectorData };
}

template<typename T>
inline typename Vector<T>::Iterator Vector<T>::insert(const Iterator & pos, const T & value)
{
	return insert(pos.itIndex, value);
}

template<typename T>
inline typename Vector<T>::Iterator Vector<T>::insert(size_t pos, T && value)
{
	assert((pos <= vectorSize) && (pos >= 0));

	checkAndExpand(vectorSize + 1);

	++vectorSize;
	new (&vectorData[vectorSize - 1]) T(std::move(vectorData[vectorSize - 2]));
	for (size_t i = (vectorSize - 2); i > pos; --i)
	{
		vectorData[i] = std::move(vectorData[i - 1]);
	}
	vectorData[pos] = std::move(value);

	return Iterator{ pos, vectorSize, vectorData };
}

template<typename T>
inline typename Vector<T>::Iterator Vector<T>::insert(const Iterator & pos, T && value)
{
	return insert(pos.itIndex, std::move(value));
}

template<typename T>
inline typename Vector<T>::Iterator Vector<T>::insert(size_t pos, size_t count, const T & value)
{
	assert((pos <= vectorSize) && (pos >= 0));
	checkAndExpand(vectorSize + count);

	//TODO: Isn`t is better if you shift the buckets up by there amount, so that I do not get O(n²)?
	for (size_t j = 0; j < count; ++j, ++pos, ++vectorSize)
	{
		new (&vectorData[vectorSize]) T(std::move(vectorData[vectorSize - 1]));
		for (size_t i = (vectorSize - 1); i > pos; --i)
		{
			vectorData[i] = std::move(vectorData[i - 1]);
		}
	}
	for (size_t i = (pos - count); i < pos; ++i)
	{
		vectorData[i] = value;
	}

	return Iterator{ pos - count, vectorSize, vectorData };
}

template<typename T>
inline typename Vector<T>::Iterator Vector<T>::insert(const Iterator & pos, size_t count, const T & value)
{
	return insert(pos.itIndex, count, value);
}

template<typename T>
inline typename Vector<T>::Iterator Vector<T>::insertPush_back(size_t pos, const T & value)
{
	assert((pos <= vectorSize) && (pos >= 0));

	checkAndExpand(vectorSize + 1);

	new (&vectorData[vectorSize]) T(std::move(vectorData[pos]));
	vectorData[pos] = value;
	++vectorSize;

	return Iterator(pos, vectorSize, vectorData);
}

template<typename T>
inline typename Vector<T>::Iterator Vector<T>::insertPush_back(const Iterator & pos, const T & value)
{
	return insertPush_back(pos.itIndex, value);
}

template<typename T>
inline typename Vector<T>::Iterator Vector<T>::insertPush_back(size_t pos, T && value)
{
	assert((pos <= vectorSize) && (pos >= 0));

	checkAndExpand(vectorSize + 1);

	new (&vectorData[vectorSize]) T(std::move(vectorData[pos]));
	vectorData[pos] = std::move(value);
	++vectorSize;

	return Iterator(pos, vectorSize, vectorData);
}

template<typename T>
inline typename Vector<T>::Iterator Vector<T>::insertPush_back(const Iterator & pos, T && value)
{
	return insertPush_back(pos.itIndex, std::move(value));
}

template<typename T>
inline typename Vector<T>::Iterator Vector<T>::erase(size_t pos)
{
	assert((pos < vectorSize) && (pos >= 0));

	vectorData[pos].~T();
	for (size_t i = (pos + 1); i < vectorSize; ++i)
	{
		vectorData[i - 1] = std::move(vectorData[i]);
	}

	checkAndShrink(vectorSize - 1);
	--vectorSize;

	return Iterator{ pos, vectorSize, vectorData };
}

template<typename T>
inline typename Vector<T>::Iterator Vector<T>::erase(const Iterator & pos)
{
	return erase(pos.itIndex);
}

template<typename T>
inline typename Vector<T>::Iterator Vector<T>::erase(size_t first, size_t last)
{
	assert((first < vectorSize) && (last <= vectorSize) && (first >= 0) && (last >= 0) && (first < last));

	for (size_t i = first; i < last; ++i)
	{
		vectorData[i].~T();
	}
	size_t newVectorSize = vectorSize;
	//TODO: Isn`t is better if you shift the buckets down by there amount, so that I do not get O(n²)?
	for (size_t j = last; j > first; --j, --newVectorSize)
	{
		for (size_t i = j; i < newVectorSize; ++i)
		{
			vectorData[i - 1] = vectorData[i];
		}
	}
	checkAndShrink(newVectorSize);
	vectorSize = newVectorSize;

	return Iterator{ first, vectorSize, vectorData };
}

template<typename T>
inline typename Vector<T>::Iterator Vector<T>::erase(const Iterator & first, const Iterator & last)
{
	return erase(first.itIndex, last.itIndex);
}

template<typename T>
inline typename Vector<T>::Iterator Vector<T>::erasePop_back(size_t pos)
{
	assert((pos < vectorSize) && (pos >= 0));

	vectorData[pos].~T();
	vectorData[pos] = std::move(vectorData[vectorSize - 1]);

	checkAndShrink(vectorSize - 1);
	--vectorSize;

	return Iterator{ pos, vectorSize, vectorData };
}

template<typename T>
inline typename Vector<T>::Iterator Vector<T>::erasePop_back(const Iterator & pos)
{
	return erasePop_back(pos.itIndex);
}

template<typename T>
inline void Vector<T>::push_back(const T & value)
{
	checkAndExpand(vectorSize + 1);

	new (&vectorData[vectorSize++]) T(value);
}

template<typename T>
inline void Vector<T>::push_back(T && value)
{
	checkAndExpand(vectorSize + 1);

	new (&vectorData[vectorSize++]) T(std::move(value));
}

template<typename T>
inline void Vector<T>::pop_back()
{
	vectorData[vectorSize - 1].~T();

	checkAndShrink(vectorSize - 1);
	--vectorSize;
}

template<typename T>
inline void Vector<T>::resize(size_t count)
{
	//NOTE: Include an upper bound?
	assert(count >= 0);

	if (count < vectorSize)
	{
		for (size_t i = count; i < vectorSize; ++i)
		{
			vectorData[i].~T();
		}

		checkAndShrink(count);
	}
	else
	{
		checkAndExpand(count);

		for (int i = vectorSize; i < count; ++i)
		{
			new (&vectorData[i]) T();
		}
	}

	vectorSize = count;
}

template<typename T>
inline void Vector<T>::resize(size_t count, const T & value)
{
	//NOTE: Include an upper bound?
	assert(count >= 0);

	if (count < vectorSize)
	{
		for (size_t i = count; i < vectorSize; ++i)
		{
			vectorData[i].~T();
		}

		checkAndShrink(count);
	}
	else
	{
		checkAndExpand(count);

		for (int i = vectorSize; i < count; ++i)
		{
			new (&vectorData[i]) T(value);
		}
	}

	vectorSize = count;
}

template<typename T>
inline void Vector<T>::swap(Vector & other)
{
	Vector<T> temp(std::move(other));
	other = std::move(*this);
	*this = std::move(temp);
}

template<typename T>
inline bool Vector<T>::operator==(Vector& rhs)
{
	if (this->vectorSize != rhs.vectorSize)
		return false;

	for (int i = 0; i < vectorSize; ++i)
	{
		if (vectorData[i] != rhs.vectorData[i])
			return false;
	}

	return true;
}

template<typename T>
inline bool Vector<T>::operator!=(Vector& rhs)
{
	return (!(this->operator==(rhs)));
}

namespace VectorTestSuit
{
	void runVectorUnitTest();
}
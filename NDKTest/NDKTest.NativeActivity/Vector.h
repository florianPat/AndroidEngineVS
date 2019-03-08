#pragma once

//Other std implementations to do: unordered_map, string, unique_ptr, std::function, map/multimap
#include "Utils.h"

template <typename T>
class Vector
{
	/* NOTE: Undefinied behavour if you deference or do other things if Its are invalidated due to a shrink or expand call
	*  This is also a "safe" iterator (it does bounds checking on current size)
	*  I do not know if the std iterator is smaller because they pass it by value every time...
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
private:
	size_t vectorSize = 0;
	size_t vectorCapacity = 2;
	T* vectorData = nullptr;
private:
	//TOOD: Think about SSE copy
	void shrink();
	void expand();

	void checkAndShrink(size_t newSize);
	void checkAndExpand(size_t minNewSize);
public:
	Vector();
	Vector(size_t count, const T& value = T());
	Vector(size_t count);
	//NOTE: Maybe I have to create a initializer-list constructor!
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

	bool empty() const;
	size_t size() const;

	void reserve(size_t size);
	size_t capacity() const;
	
	//NOTE: The standard states that the memory should not be deallocated, but maybe we should do kind of that?
	void clear();
	Iterator insert(size_t pos, const T& value);
	Iterator insert(const Iterator& pos, const T& value);
	Iterator insert(size_t pos, T&& value);
	Iterator insert(const Iterator& pos, T&& value);
	Iterator insert(size_t pos, size_t count, const T& value);
	Iterator insert(const Iterator& pos, size_t count, const T& value);
	//NOTE: Does theses functions even make sense??
	Iterator insertPush_back(size_t pos, const T& value);
	Iterator insertPush_back(const Iterator& pos, const T& value);
	Iterator insertPush_back(size_t pos, T&& value);
	Iterator insertPush_back(const Iterator& pos, T&& value);
	//NOTE: I make a pop_back_swap_erase here!
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
	//NOTE: Deleted version with rvalue because I am to dumb at this point in implementing it!
	void resize(size_t count, const T& value);
	void swap(Vector& other) noexcept;

	bool operator==(Vector<T>& rhs);
	bool operator!=(Vector<T>& rhs);
	//NOTE: If you need other compares, make them!
};

template<typename T>
inline void Vector<T>::shrink()
{
	vectorCapacity /= 2;

	T* newData = new T[vectorCapacity];
	for (size_t i = 0; i < vectorCapacity; ++i)
	{
		newData[i] = std::move(vectorData[i]);
	}
	delete[] vectorData;
	vectorData = newData;
}

template<typename T>
inline void Vector<T>::expand()
{
	T* newData = new T[vectorCapacity];
	for (size_t i = 0; i < vectorSize; ++i)
	{
		newData[i] = std::move(vectorData[i]);
	}
	delete[] vectorData;
	vectorData = newData;
}

template<typename T>
inline void Vector<T>::checkAndShrink(size_t newSize)
{
	while ((vectorCapacity / 2) >= (newSize + 2))
		shrink();
}

template<typename T>
inline void Vector<T>::checkAndExpand(size_t minNewSize)
{
	if (vectorCapacity < minNewSize)
	{
		//TODO: Check this if you have large numbers!!
		do
		{
			vectorCapacity *= 2.0f;
		} while (vectorCapacity < minNewSize);

		expand();
	}
}

template<typename T>
inline Vector<T>::Vector() : vectorData(new T[vectorCapacity])
{
}

template<typename T>
inline Vector<T>::Vector(size_t count, const T & value) : vectorSize(count), vectorCapacity(count + 4), vectorData(new T[vectorCapacity])
{
	for (size_t i = 0; i < vectorSize; ++i)
	{
		vectorData[i] = value;
	}
}

template<typename T>
inline Vector<T>::Vector(size_t count) : vectorSize(count), vectorCapacity(count + 4), vectorData(new T[vectorCapacity])
{
	T t = T();
	for (size_t i = 0; i < vectorSize; ++i)
	{
		vectorData[i] = t;
	}
}

template<typename T>
inline Vector<T>::Vector(const Vector& other) : vectorSize(other.vectorSize), vectorCapacity(other.vectorCapacity), vectorData(new T[vectorCapacity])
{
	for (size_t i = 0; i < vectorSize; ++i)
		vectorData[i] = other.vectorData[i];
}

template<typename T>
inline Vector<T>& Vector<T>::operator=(const Vector& rhs)
{
	this->~Vector();

	vectorCapacity = rhs.vectorCapacity;
	vectorSize = rhs.vectorSize;
	vectorData = new T[vectorCapacity];

	for (size_t i = 0; i < vectorSize; ++i)
	{
		vectorData[i] = rhs.vectorData[i];
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

	vectorSize = std::exchange(rhs.vectorSize);
	vectorCapacity= std::exchange(rhs.vectorCapacity, 0);
	vectorData = std::exchange(rhs.data, nullptr);

	return *this;
}

template<typename T>
inline Vector<T>::~Vector()
{
	vectorCapacity = 0;
	vectorSize = 0;
	for (size_t i = 0; i < vectorSize; ++i)
	{
		vectorData[i].~T();
	}
	delete[] vectorData;
	vectorData = nullptr;
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
	assert(vectorSize != 0);
	return vectorData[0];
}

template<typename T>
inline const T & Vector<T>::front() const
{
	assert(vectorSize != 0);
	return vectorData[0];
}

template<typename T>
inline T & Vector<T>::back()
{
	assert(vectorSize != 0);
	return vectorData[vectorSize - 1];
}

template<typename T>
inline const T & Vector<T>::back() const
{
	assert(vectorSize != 0);
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
	return Iterator{ 0, vectorSize, vectorData};
}

template<typename T>
inline typename Vector<T>::Iterator Vector<T>::end()
{
	return Iterator{vectorSize, vectorSize, vectorData};
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
inline void Vector<T>::reserve(size_t newSize) noexcept
{
	//NOTE: Include an upper bound?
	assert(newSize >= 0);

	checkAndExpand(newSize);

	vectorSize = newSize;
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
	for (size_t i = (vectorSize - 1); i > pos; --i)
	{
		vectorData[i] = std::move(vectorData[i - 1]);
	}
	vectorData[pos] = value;

	return Iterator{pos, vectorSize, vectorData};
}

template<typename T>
inline typename Vector<T>::Iterator Vector<T>::insert(const Iterator & pos, const T & value)
{
	return insert(pos.itIndex, std::forward(value));
}

template<typename T>
inline typename Vector<T>::Iterator Vector<T>::insert(size_t pos, T && value)
{
	assert((pos <= vectorSize) && (pos >= 0));

	checkAndExpand(vectorSize + 1);

	++vectorSize;
	for (size_t i = (vectorSize - 1); i > pos; --i)
	{
		vectorData[i] = std::move(vectorData[i - 1]);
	}
	vectorData[pos] = std::move(value);

	return Iterator{ pos, vectorSize, vectorData };
}

template<typename T>
inline typename Vector<T>::Iterator Vector<T>::insert(const Iterator & pos, T && value)
{
	return insert(pos.itIndex, std::forward(value));
}

template<typename T>
inline typename Vector<T>::Iterator Vector<T>::insert(size_t pos, size_t count, const T & value)
{
	assert((pos <= vectorSize) && (pos >= 0));

	checkAndExpand(vectorSize + count);

	//TODO: Isn`t is better if you shift the buckets up by there amount, so that I do not get O(n²)?
	for (size_t j = 0; j < count; ++j, ++pos, ++vectorSize)
	{
		for (size_t i = vectorSize; i > pos; --i)
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
	return insert(pos.itIndex, count, std::forward(value));
}

template<typename T>
inline typename Vector<T>::Iterator Vector<T>::insertPush_back(size_t pos, const T & value)
{
	assert((pos <= vectorSize) && (pos >= 0));

	checkAndExpand(vectorSize + 1);

	vectorData[vectorSize] = std::move(vectorData[pos]);
	vectorData[pos] = value;
	++vectorSize;

	return Iterator(pos, vectorSize, vectorData);
}

template<typename T>
inline typename Vector<T>::Iterator Vector<T>::insertPush_back(const Iterator & pos, const T & value)
{
	return insertPush_back(pos.itIndex, std::forward(value));
}

template<typename T>
inline typename Vector<T>::Iterator Vector<T>::insertPush_back(size_t pos, T && value)
{
	assert((pos <= vectorSize) && (pos >= 0));

	checkAndExpand(vectorSize + 1);

	vectorData[vectorSize] = std::move(vectorData[pos]);
	vectorData[pos] = std::move(value);
	++vectorSize;

	return Iterator(pos, vectorSize, vectorData);
}

template<typename T>
inline typename Vector<T>::Iterator Vector<T>::insertPush_back(const Iterator & pos, T && value)
{
	return insertPush_back(pos.itIndex, std::forward(value));
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
	--vectorSize;

	checkAndShrink(vectorSize);

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
	//TODO: Isn`t is better if you shift the buckets down by there amount, so that I do not get O(n²)?
	for (size_t j = last; j > first; --j)
	{
		for (size_t i = j; i < vectorSize; ++i)
		{
			vectorData[i - 1] = vectorData[i];
		}
		--vectorSize;
	}
	checkAndShrink(vectorSize);

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
	--vectorSize;

	checkAndShrink(vectorSize);

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

	vectorData[vectorSize++] = value;
}

template<typename T>
inline void Vector<T>::push_back(T && value)
{
	checkAndExpand(vectorSize + 1);

	vectorData[vectorSize++] = std::move(value);
}

template<typename T>
inline void Vector<T>::pop_back()
{
	vectorData[--vectorSize].~T();

	checkAndShrink(vectorSize);
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

		for (int i = vectorSize; i < (vectorSize + count); ++i)
		{
			vectorData[i] = T();
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

		for (int i = vectorSize; i < (vectorSize + count); ++i)
		{
			vectorData[i] = value;
		}
	}

	vectorSize = count;
}

template<typename T>
inline void Vector<T>::swap(Vector & other) noexcept
{
	Vector<T> temp = std::move(other);
	other = std::move(*this);
	*this = std::move(temp);
}

template<typename T>
inline bool Vector<T>::operator==(Vector<T>& rhs)
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
inline bool Vector<T>::operator!=(Vector<T>& rhs)
{
	return (!(this->operator==(rhs)));
}

#include "UnitTester.h"

namespace VectorTestSuit
{
	static void runVectorUnitTest()
	{
		UnitTester::test([]()
		{
			Vector<int> vec;
			unitAssert(vec.capacity() == 2);
			unitAssert(vec.empty());
			unitAssert(vec.size() == 0);
			unitAssert(vec.begin() == vec.end());
			Vector<int> otherVec(4, 3);
			unitAssert(otherVec.capacity() == 8);
			unitAssert(otherVec.size() == 4);
			unitAssert((*(otherVec.begin())) == 3);
			unitAssert(otherVec.begin() != otherVec.end());
			vec = otherVec;
			for (auto it = vec.begin(); it != vec.end(); ++it)
			{
				unitAssert((*it) == 3);
			}
			unitAssert(vec.size() == 4);
			unitAssert(vec.at(2) == 3);
			unitAssert(vec[3] == 3);
			vec.push_back(4);
			vec.push_back(7);
			vec.push_back(2);
			vec.push_back(9);
			vec.push_back(5);
			unitAssert(vec.size() == 9);
			unitAssert(vec.capacity() == 16);
			unitAssert((*(--vec.end())) == 5);
			vec.pop_back();
			unitAssert(vec.back() == 9);
			unitAssert(vec.erase(0, 4).operator*() == 4);
			unitAssert(vec.capacity() == 8);
			unitAssert(vec.size() == 4);
			unitAssert(vec.erase(0).operator*() == 7);
			vec.resize(1, 4);
			vec.resize(5);
			unitAssert(vec.back() == 0);
			Vector<int> oOtherVec = std::move(vec);
			unitAssert(vec.size() == 0);
			oOtherVec.insert(3, 3, 1);
			unitAssert(oOtherVec.size() == 8);

			unitAssert(oOtherVec != vec);
			Vector<int> thisVector(1, 7);
			thisVector.insert(thisVector.size() - 1, 2, 0);
			thisVector.erase(0, 2);
			thisVector.insert(thisVector.size(), 2, 0);
			thisVector.push_back(1);
			thisVector.insert(thisVector.size(), 2, 1);
			thisVector.insert(thisVector.size(), 2, 0);
			unitAssert(oOtherVec == thisVector);
			thisVector.reserve(5);
			thisVector.reserve(20);
			thisVector.clear();
			unitAssert(thisVector.capacity() == 20);

			int sum = 0;
			for (int e : thisVector)
			{
				sum += e;
			}

			return true;
		});
	}
}
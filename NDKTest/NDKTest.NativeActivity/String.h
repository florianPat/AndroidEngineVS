#pragma once

#include "Utils.h"
#include "Vector.h"

#define CALL_STRING_ARRAY(exp) if(shortRep) return stringUnion.stringArrayShort.exp; else return stringUnion.stringArrayLong.exp
//NOTE: For now, there is no possibiliy for a wide-character string. I could potentialy shoot myself in the foot here for localization purporses

class String
{
public:
	static constexpr int SHORT_STRING_SIZE = 16;
	static constexpr size_t npos = -1;
protected:
	const bool shortRep;

	union StringUnion
	{
		Array<char, SHORT_STRING_SIZE> stringArrayShort;
		Vector<char> stringArrayLong;

		~StringUnion();
	} stringUnion;
public:
	String();
	String(size_t count, bool shouldBeALongString = false);
	String(size_t count, char c, bool shouldBeALongString = false);
	//String(const String& other, size_t pos, size_t count);
	//String(const char* s, size_t count);
	template <size_t N>
	String(const char (&s) [N], bool shouldBeALongString = false);
	String(const String& other);
	String(String&& other);
	String& operator=(const String& rhs);
	String& operator=(String&& rhs);
	template <size_t N>
	String& operator=(const char(&s)[N]);
	~String();

	char& at(size_t pos);
	char at(size_t pos) const;

	char& operator[](size_t pos);
	char operator[](size_t pos) const;
	
	char& front();
	char front() const;
	char& back();
	char back() const;
	const char* data() const;
	const char* c_str() const;

	Iterator<char> begin();
	Iterator<char> end();

	ConstIterator<char> begin() const;
	ConstIterator<char> end() const;

	bool empty() const;
	
	size_t size() const;
	size_t length() const;

	void reserve(size_t count);
	size_t capacity() const;

	void clear();
	
	//NOTE: Implement insert if you need them

	String& erase(size_t pos, size_t count);
	Iterator<char> erase(const Iterator<char>& pos);
	Iterator<char> erase(const Iterator<char>& first, const Iterator<char>& last);

	Iterator<char> push_back(char c);
	Iterator<char> pop_back();

	//NOTE: Will fail if string is "full" if it is a short string
	String& append(size_t count, char c);
	String& append(const String& str);
	String& append(const String& str, size_t pos, size_t count);
	String& append(const char* str);
	String& append(const char* str, size_t count);
	String& operator+=(const String& str);
	String& operator+=(char c);
	String& operator+=(const char* str);

	String substr(size_t pos = 0, size_t count = npos) const;

	size_t find(char c, size_t pos = 0) const;
	//TODO: Implement!
	size_t find(const char* str, size_t pos = 0) const;
	size_t find(const String& str, size_t pos = 0) const;
	//NOTE: isn't it unnecessary?
	size_t find_first_of(char c, size_t pos = 0) const;
	size_t find_last_of(char c, size_t pos = npos) const;

	//NOTE: Implement resize if you need it!

	void swap(String& other);

	String operator+(const String& rhs) const;
	String operator+(const char* rhs) const;
	String operator+(char c) const;
	friend String operator+(const char* lhs, const String& rhs);
	friend String operator+(char lhs, const String& rhs);
	friend String operator+(String&& lhs, const String& rhs);

	//TODO: This can go wrong!
	bool operator==(const String& rhs) const;
	bool operator==(const char* rhs) const;
	friend bool operator==(const char* lhs, const String& rhs);
	bool operator!=(const String& rhs) const;
	bool operator!=(const char* rhs) const;
	friend bool operator!=(const char* lhs, const String& rhs);
};

template<size_t N>
inline String::String(const char(&s)[N], bool shouldBeALongString) : shortRep(N >= SHORT_STRING_SIZE || shouldBeALongString ? false : true), stringUnion{{ { {0} } }}
{
	if (shortRep)
	{
		for (size_t i = 0; i < N; ++i)
			stringUnion.stringArrayShort.push_back(s[i]);
	}
	else
	{
		new (&stringUnion.stringArrayLong) Vector<char>(N);

		for (size_t i = 0; i < N; ++i)
			stringUnion.stringArrayLong.push_back(s[i]);
	}
}

template<size_t N>
inline String & String::operator=(const char(&s)[N])
{
	this->~String();

	new (this) String(s);

	return *this;
}

#include <string>

namespace std {
	template<>
	struct hash<String> {
	public:
		size_t operator()(const String &s) const
		{
			//TODO: Build own hasher!
			return std::hash<std::string>()(s.c_str());
		}
	};
}

namespace utils
{
	bool isWordInLine(const String & word, const String & lineContent);
	String getWordBetweenChars(const String& lineContent, char first, char last);
}

namespace StringUnitTest
{
	void runStringUnitTests();
	void runStdStringUnitTests();
}

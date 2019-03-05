#pragma once

#include "android_native_app_glue.h"
#include <string>

class Ifstream final
{
public:
	enum class SeekDir {beg, cur, end};
private:
	AAsset* asset = nullptr;
	AAssetManager* aassetManager = nullptr;
	bool good = true;
public:
	Ifstream(const std::string& filename);
	Ifstream();
	Ifstream(const Ifstream& other) = delete;
	Ifstream(Ifstream&& other);
	Ifstream& operator=(const Ifstream& rhs) = delete;
	Ifstream& operator=(Ifstream&& rhs);
	~Ifstream();
	explicit operator bool() const;
	bool operator!() const;
	bool eof();
	void getline(std::string& line);
	void read(void* s, uint n);
	char get();
	std::streampos tellg();
	void seekg(uint pos);
	void seekg(uint off, SeekDir way);
	long long getSize();
	void open(const std::string& filename);
	void getFullData(void* s);
	void close();
};
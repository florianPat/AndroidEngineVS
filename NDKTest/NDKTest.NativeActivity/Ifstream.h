#pragma once

#include "android_native_app_glue.h"
#include <string>
#include <ios>

class Ifstream
{
	AAsset* asset = nullptr;
	AAssetManager* assetManager = nullptr;
	bool fail = false;
public:
	Ifstream(const std::string& filename, AAssetManager* assetManager);
	Ifstream(AAssetManager* assetManager);
	~Ifstream();
	explicit operator bool() const;
	bool operator!() const;
	bool eof();
	void getline(std::string& line);
	Ifstream& operator=(const Ifstream&) = delete;
	void read(void* s, std::streamsize n);
	char get();
	std::streampos tellg();
	void seekg(std::streampos pos);
	void seekg(std::streamoff off, std::ios_base::seekdir way);
	//TODO: Think about making this an int!
	long long getSize();
	void open(const std::string& filename);
	void getFullData(void* s);
	void close();
};
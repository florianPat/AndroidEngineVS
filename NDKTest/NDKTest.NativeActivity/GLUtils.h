#pragma once

#include <string>

void clearErrors();
void checkErrors(const std::string& func, int line, const char* inFunc);

#define CallGL(x) do { clearErrors(); x; checkErrors(#x, __LINE__, __FILE__); } while(false)
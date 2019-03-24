#pragma once

#include "String.h"

void clearErrors();
void checkErrors(const String& func, int line, const char* inFunc);

#define CallGL(x) do { clearErrors(); x; checkErrors(#x, __LINE__, __FILE__); } while(false)
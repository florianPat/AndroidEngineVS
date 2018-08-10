#pragma once

#include <string>

void clearErrors();
void checkErrors(const std::string& func);

#define CallGL(x) do { clearErrors(); x; checkErrors(#x); } while(false)
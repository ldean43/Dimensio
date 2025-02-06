#pragma once
#include <string>
#include <stdexcept>

bool isDouble(std::string str);

bool isVar(std::string str);

void mismatchError(std::string op, std::string delim);

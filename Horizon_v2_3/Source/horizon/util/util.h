#ifndef UTIL_H
#define UTIL_H

#include <cctype>
#include <string>
#include <algorithm>

using namespace std;

namespace horizon {
	namespace util {

enum varType {
	INT_,
	DOUBLE_,
	FLOAT_,
	BOOL_,
	MATRIX_,
	QUAT_,
	ERROR_
};

varType strToVarType(string type);

bool atob(string var);

}}
#endif
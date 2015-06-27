#include "util.h"

namespace horizon {
	namespace util {

varType strToVarType(string type) {
	transform(type.begin(), type.end(), type.begin(), toupper);
	if(_strcmpi(type.c_str(), "INT") == 0)
		return INT_;
	else if(_strcmpi(type.c_str(), "DOUBLE") == 0)
		return DOUBLE_;
	else if(_strcmpi(type.c_str(), "FLOAT") == 0)
		return FLOAT_;
	else if(_strcmpi(type.c_str(), "BOOL") == 0)
		return BOOL_;
	else if(_strcmpi(type.c_str(), "MATRIX") == 0)
		return MATRIX_;
	else if(_strcmpi(type.c_str(), "QUAT") == 0)
		return QUAT_;
	else
		return ERROR_;
}

bool atob(string var) {
	transform(var.begin(), var.end(), var.begin(), toupper);
	if(_strcmpi(var.c_str(), "TRUE") == 0 || _strcmpi(var.c_str(), "1") == 0)
		return true;
	return false;
}

}}
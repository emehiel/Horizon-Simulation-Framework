#ifndef LUA_DEFS_H
#define LUA_DEFS_H

#include <lua.hpp>


#include "register.h"
#include <vector>

using namespace std;

namespace horizon {
	namespace script {

lua_State* getNewLuaState();

void addLuaFile(string name);

}} // horizon::script
#endif
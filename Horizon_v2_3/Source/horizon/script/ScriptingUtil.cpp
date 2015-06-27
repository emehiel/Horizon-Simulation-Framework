#include "ScriptingUtil.h"

namespace horizon {
	namespace script {

bool functionExists(lua_State* L, std::string fcnName) {
	// Push the function name onto the stack
	lua_pushstring(L, fcnName.c_str());
	// Get the of the string in the global LUA table
	lua_rawget(L, LUA_GLOBALSINDEX);
	// Check if the value is a function
	bool ret = lua_isfunction(L, -1);
	// Pop the function off the stack
	lua_pop(L, 1);
	return ret;
}

}} // horizon::script
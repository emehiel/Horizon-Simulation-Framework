/**
* \file ScriptingUtil.h
* This file contains utility functions for working with LUA and Luabind.
* @author Brian Butler
*/

#ifndef SCRIPTINGUTIL_H
#define SCRIPTINGUTIL_H

// LUA Includes
#include <lua.hpp>

// C++ Includes
#include <string>

/**
 * \namespace horizon::script
 * This namespace contains files related to scripting support.
 */
namespace horizon {
	namespace script {

/**
 * Check whether the specified LUA function exists.
 * @param L A pointer to the LUA state.
 * @param fcnName The name of the LUA function
 * @return true if the function exists in the LUA state, false otherwise.
 * @author Brian Butler
 */
bool functionExists(lua_State* L, std::string fcnName);

}}

#endif
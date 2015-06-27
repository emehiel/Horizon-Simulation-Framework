#ifndef SCRIPTEDSUBSYSTEMTESTS_H
#define SCRIPTEDSUBSYSTEMTESTS_H

#include "horizon/sub/script/ScriptedSubsystem.h"
#include "horizon/sub/script/SubsystemParameters.h"

using horizon::sub::script::ScriptedSubsystem;
using horizon::sub::script::SubsystemParameters;


// LUA / Luabind Includes
#include <lua.hpp>
#include <luabind/luabind.hpp>

// LUA / Luabind Usings
using namespace luabind;

namespace horizon {
	namespace test {

void testSubsystemParameters(lua_State* L);

void testScriptedSubsystem(lua_State* L);

}} // horizon::test 

#endif
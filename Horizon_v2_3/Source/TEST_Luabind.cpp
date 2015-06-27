#define _USE_MATH_DEFINES

// LUA / Luabind Includes
#include <lua.hpp>
//#include "horizon/script/register.h"
#include "horizon/script/luaDefs.h"
// LUA / Luabind Usings
//using namespace lua;
using namespace luabind;
//using namespace horizon::script;

#include "horizon/util/eoms/ScriptedEOMS.h"
using namespace horizon::util::eoms;
//using namespace lua;
//using namespace luabind;

class TestClass {
private:
	double var1, var2;
public:
	double publicVar;

	TestClass();

	SetVar1(double var);
	GetVar1();
};



int main( int argc, char *argv[ ], char *envp[ ] ) {
	// Open the LUA state and register Lua/C++ bindings
	lua_State* L;
	lua::L = lua_open();
	luaL_openlibs(lua::L);
	open(lua::L);
	set_pcall_callback(&luabind::luaErrFcn);
	register_LUAState(lua::L);


	//getchar();
	return 0;
}
#include "luaDefs.h"

namespace horizon {
	namespace script {
			
			vector<string> luaFiles;

			lua_State* getNewLuaState() { 
				lua_State* L = lua_open();
				luaL_openlibs(L);
				open(L);
				register_LUAState(L);
				//register_SystemScheduleWrapper(L); //
				set_pcall_callback(&luabind::luaErrFcn);
				for(vector<string>::iterator sIt = luaFiles.begin(); sIt != luaFiles.end(); sIt++)
					loadLuaFunctionsFromFile(L, *sIt);
				return L;
			}

			void addLuaFile(string name) { luaFiles.push_back(name); }

}} // horizon::script
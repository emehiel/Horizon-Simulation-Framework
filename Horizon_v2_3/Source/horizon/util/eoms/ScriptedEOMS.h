#ifndef SCRIPTEDEOMS_H
#define SCRIPTEDEOMS_H

// STL Includes
#include <string>

// HSF Includes
#include "EOMS.h"
#include "horizon/script/ScriptingUtil.h"

// LUA / Luabind Includes
#include <lua.hpp>
#include <luabind/luabind.hpp>

// Standard Library Usings
using namespace std;

// HSF Usings
using namespace horizon::script;

// LUA / Luabind Usings
using namespace luabind;

namespace horizon {
namespace util {
namespace eoms {

	/**
	* @author Brian Butler
	*/

	class ScriptedEOMS : public EOMS
	{

	public:
		/**
		* 
		*/
		ScriptedEOMS(lua_State* L, string evalEOMSfcn, string initEOMSfcn = "");

		/**
		* 
		*/
		virtual ~ScriptedEOMS();

		/**
		*
		*/
		virtual Matrix operator() ( const double &t, const Matrix &y );
	private:
		string initEOMSfcn;
		string evalEOMSfcn;
		lua_State* L;
	};

} // end namespace eoms
} // end namespace util
} // end namespace horizon



#endif
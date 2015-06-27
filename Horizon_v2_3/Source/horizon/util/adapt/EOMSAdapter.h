#ifndef EOMSADAPTER_H
#define EOMSADAPTER_H

// LUA Includes
#include <lua.hpp>

// HSF Includes
#include "horizon/util/eoms/EOMS.h"
#include "horizon/util/eoms/ScriptedEOMS.h"
#include "horizon/util/eoms/orbital_EOMS.h"
#include "horizon/util/eoms/orbital_EOMS_J2.h"
#include "horizon/util/eoms/orbital_EOMS_TEST.h"
#include "horizon/io/xmlParser.h"

// HSF Usings
using namespace horizon::util::eoms;
using namespace horizon::io;

namespace lua {
	extern lua_State* L;
}

namespace horizon {
	namespace util {
		namespace adapt {

EOMS* createEOMSObject(XMLNode& EOMSNode);

}}} // horizon::util::adapt
#endif
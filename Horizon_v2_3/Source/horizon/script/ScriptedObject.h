#ifndef SCRIPTED_OBJECT_H
#define SCRIPTED_OBJECT_H

// "register.h" includes everything needed
#include "register.h"
#include "horizon/script/luaDefs.h"

using namespace horizon::script;

namespace horizon {
	namespace script {

class ScriptedObject {
protected:
	lua_State* L_;

public:
	ScriptedObject();
	ScriptedObject(ScriptedObject &other);
	~ScriptedObject();
};

}} // horizon::script

#endif
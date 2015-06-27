#include "ScriptedObject.h"

namespace horizon {
	namespace script {

ScriptedObject::ScriptedObject() {
	L_ = horizon::script::getNewLuaState();
}

ScriptedObject::ScriptedObject(ScriptedObject &other) {
	L_ = horizon::script::getNewLuaState();
}

ScriptedObject::~ScriptedObject() {}




}} // horizon::script
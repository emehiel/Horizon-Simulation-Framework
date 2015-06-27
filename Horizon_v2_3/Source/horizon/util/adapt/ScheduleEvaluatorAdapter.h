#ifndef SCHEDULEEVALUATORADAPTER_H
#define SCHEDULEEVALUATORADAPTER_H

// LUA Includes
#include <lua.hpp>

// C++ Includes
#include <string>
#include <iostream>

// HSF Includes
#include "horizon/eval/ScheduleEvaluator.h"
#include "horizon/eval/TargetValueScheduleEvaluator.h"
#include "horizon/eval/QuantitativeScheduleEvaluator.h"
#include "horizon/eval/ScriptedScheduleEvaluator.h"
#include "horizon/eval/FullScriptedScheduleEvaluator.h"
#include "horizon/io/xmlParser.h"

// C++ Usings
using namespace std;

// HSF Usings
using namespace horizon::eval;
using namespace horizon::io;

namespace horizon {
	namespace util {
		namespace adapt {

ScheduleEvaluator* createScheduleEvaluator(lua_State* L, XMLNode& SchedEvalNode);

}}} // horizon::util::adapt
#endif
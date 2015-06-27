/**
* \file FullScriptedScheduleEvaluator.h
* This file contains a class used to evaluate schedules using a 
* LUA function.
* @author Brian Butler
*/

#ifndef FULLSCRIPTEDSCHEDULEEVALUATOR_H
#define FULLSCRIPTEDSCHEDULEEVALUATOR_H

// HSF Includes
#include "ScheduleEvaluator.h"
#include "SystemScheduleWrapper.h"
#include "horizon/script/ScriptingUtil.h"

// LUA / Luabind Includes
#include <lua.hpp>
#include <luabind/luabind.hpp>

// C++ Includes
#include <string>

// C++ Usings
using namespace std;

// LUA / Luabind Usings
using luabind::call_function;

// HSF Usings
using horizon::script::functionExists;

/**
* \namespace horizon::eval
* This namespace contains files related to schedule evaluation.
*/
namespace horizon {
namespace eval {

/**
 * A class that assigns numeric values to schedules.
 * A LUA script is used to assign a numeric value to each Event in the schedule.
 * The final schedule value is the sum of all Event values.
 * @author Brian Butler
 */
class FullScriptedScheduleEvaluator : public virtual ScheduleEvaluator {
private:
	/**
	* The name of the evaluation function.
	* The name must be a valid LUA function that assigns
	*/
	string evaluateLUAFcn;
	/**
	* Pointer to the LUA state.
	*/
	lua_State* L;
public:

	/** 
	* A constructor for the schedule evaluator.
	* Creates a quantitative schedule evaluator.
	* @param L A pointer to the LUA state.
	* @param evaluateLUAFcn The name of the LUA function that is used to evaluate each
	* event in the schedule. This function must have a single Event argument and return
	* a number representing the value of the Event.
	* <BR><BR>
	* A C++ function with the same signature as the required LUA function would be:<BR>
	* <CODE>double evalFunction(Event& event);</CODE><BR><BR>
	* This means that the LUA function must have the following signature and return a single number:<BR>
	* <CODE>function evalFunction(event)</CODE>
	*/
	FullScriptedScheduleEvaluator(lua_State* L, string evaluateLUAFcn);

	/**
	* A destructor.
	*/
	~FullScriptedScheduleEvaluator();

	/**
	 * Assigns a numeric value to the schedule.
	 * Analyzes each Event within the given schedule and assigns a value to it based on the user script.
	 * The final schedule value is the sum of all Event values.
	 * @param schedule The systemSchedule to be evaluated
	 * @return The value of the schedule
	 */
	double evaluate(systemSchedule& schedule) const;
};

}} // end namespace horizon::eval

#endif
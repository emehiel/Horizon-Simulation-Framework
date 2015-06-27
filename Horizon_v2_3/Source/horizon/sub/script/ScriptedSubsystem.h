/**
 * \file ScriptedSubsystem.h
 * This file contains the ScriptedSubsystem class.
 * @author Brian Butler
 */

#ifndef SCRIPTEDSUBSYSTEM_H
#define SCRIPTEDSUBSYSTEM_H

// STL Includes
#include <string>

// HSF Includes
#include "SubsystemParameters.h"
#include "horizon/Subsystem.h"
#include "horizon/script/ScriptingUtil.h"

// ScriptedObject
#include "horizon/script/ScriptedObject.h"

// LUA / Luabind Includes (CHECK IF NEEDED)
#include <lua.hpp>
#include <luabind/luabind.hpp>

// Standard Library Usings
using namespace std;

// HSF Usings
using namespace horizon::io;
using namespace horizon::script;

// LUA / Luabind Usings
using namespace luabind;

namespace horizon {
	namespace sub {
		namespace script {

/**
 * A class that represents a ScriptedSubsystem.
 * The logic code for the ScriptedSubsystem is specified
 * by 3 LUA functions.
 *
 * @author Brian Butler
 */
class ScriptedSubsystem : public Subsystem, public ScriptedObject {

private:
	string initLUAFcn;
	string canPerformLUAFcn;
	string canExtendLUAFcn;

	// Holder for the LUA state
	//lua_State* L;

	// Pointer to the SubsystemParameter object for this class
	SubsystemParameters* subsystemParameters;

public:
	/**
	 * The constructor for the ScriptedSubsystem class.
	 *
	 * @param L A pointer to the LUA state.
	 * @param subsystemName The name of the Subsystem.
	 * @param initLUAFcn The name of the LUA function that initializes the ScriptedSubsystem.
	 * The initialization LUA function is responsible for adding any StateVarKey's to the Subsystem
	 * and adding any parameters to the SubsystemParameters object.<BR><BR>
	 * The initialization function would have the following C++ signature:<BR>
	 * &nbsp;&nbsp;&nbsp;<CODE>void init(Subsystem*, SubsystemParameters*);</CODE><BR><BR>
	 * This means that the LUA function must have the following signature:<BR>
	 * &nbsp;&nbsp;&nbsp;<CODE>function init(sub, subParams)</CODE><BR><BR>
	 * The <CODE>this</CODE> pointer is passed to the LUA function so that StateVarKeys that
	 * need to be written out and the end of the simulation can be added.
	 * @param canPerformLUAFcn The name of the LUA function that is called when ScriptedSubsystem::canPerform() is called.
	 * In addition to the arguments of ScriptedSubsystem::canPerform(), the pointer to the SubsystemParameters object is
	 * passed to the LUA function. This is so that parameters can be retrieved during the simulation.
	 * The final canPerform function would have the following C++ signature:<BR>
	 * &nbsp;&nbsp;&nbsp;<CODE>bool canPerform(const State* oldState, State* newState, const Task* task, * Position* position, Environment* environment, NodeDependencies* deps, SubsystemParameters* subsystemParameters);</CODE><BR><BR>
	 * This means that the LUA function must have the following signature and return a true or false value:<BR>
	 * &nbsp;&nbsp;&nbsp;<CODE>function canPerform(oldState, newState, task, position, environment, deps, subParams)</CODE>
	 * @param canExtendLUAFcn The name of the LUA function that is called when ScriptedSubsystem::canExtend() is called.<BR>
	 * In addition to the arguments of ScriptedSubsystem::canExtend(), the pointer to the SubsystemParameters object is
	 * passed to the LUA function. This is so that parameters can be retrieved during the simulation.
	 * The final canExtend function would have the following C++ signature:<BR>
	 * &nbsp;&nbsp;&nbsp;<CODE>bool canExtend(State* state, Position* position, Environment* environment, const double evalToTime, NodeDependencies* deps, SubsystemParameters* subsystemParameters);</CODE><BR><BR>
	 * This means that the LUA function must have the following signature and return a true or false value:<BR>
	 * &nbsp;&nbsp;&nbsp;<CODE>function canExtend(state, position, environment, evalToTime, deps, subParams)</CODE>
	 */
	ScriptedSubsystem(/*lua_State* L, */string subsystemName, string initLUAFcn, string canPerformLUAFcn, string canExtendLUAFcn);
	
	virtual ScriptedSubsystem* clone() const;

	/**
	 * The destructor for the ScriptedSubsystem class.
	 * The destructure will delete the SubsystemParameters
	 * object that ScriptedSubsystem holds.
	 */
	virtual ~ScriptedSubsystem();


	/**
	 * Returns whether the Subsystem can perform the operations necessary for
	 * the completion of the given task.
	 *
	 * @param oldState The State at the end of the previous Event.
	 * @param newState The new State to be generated, if possible.
	 * @param task The Task to be performed.
	 * @param position The Position of the Asset that this Subsystem belongs to.
	 * @param environment The Environment where the system resides.
	 * @param deps The NodeDependencies object for this Subsystem. This object is used
	 * to call dependency functions.
	 * @return true if the Task can be performed, false otherwise. If an error occurs
	 * evaluating the LUA function called by this method, false is returned.
	 */
	virtual bool canPerform(const State* oldState, State* newState, const Task* task, Position* position, Environment* environment, NodeDependencies* deps) const;

	/**
	 * Returns whether the Subsystem can continue operations in a nominal state until the desired time.
	 * The last Event is extended
	 *
	 * @param state The State at the end of the previous Event that is to be extended.
	 * @param position The Position of the Asset that this Subsystem belongs to.
	 * @param environment The Environment where the system resides.
	 * @param evalToTime The time that the last Event needs to be extended to.
	 * @param deps The NodeDependencies object for this Subsystem. This object is used
	 * to call dependency functions.
	 * @return true if the Event can be extended, false otherwise. If an error occurs
	 * evaluating the LUA function called by this method, false is returned.
	 */
	virtual bool canExtend(State* state, Position* position, Environment* environment, const double evalToTime, NodeDependencies* deps) const;

	/**
	 * Sets the LUA state.
	 * @param L A pointer to the LUA state.
	 */
	void setLuaState(lua_State *L);
	
	/**
	 * Gets the LUA state.
	 * @return A pointer to the LUA state.
	 */
	lua_State* getLuaState();
};

// Inline function definitions
inline void ScriptedSubsystem::setLuaState(lua_State *L) {
	this->L_ = L;
}

inline lua_State* ScriptedSubsystem::getLuaState() {
	return L_;
}

}}} // horizon::sub::Script

#endif
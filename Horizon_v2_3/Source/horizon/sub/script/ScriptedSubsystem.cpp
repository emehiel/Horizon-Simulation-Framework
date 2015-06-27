#include "ScriptedSubsystem.h"

namespace horizon {
	namespace sub {
		namespace script {

ScriptedSubsystem::ScriptedSubsystem(/*lua_State* L, */string subsystemName, string initLUAFcn, string canPerformLUAFcn, string canExtendLUAFcn) :
		/*L(L),*/
		Subsystem(subsystemName),
		initLUAFcn(initLUAFcn),
		canPerformLUAFcn(canPerformLUAFcn),
		canExtendLUAFcn(canExtendLUAFcn)
{
	// Initialize the SubsystemParameters object on the heap
	subsystemParameters = new SubsystemParameters();
	// Perform initialization for the ScriptedSubsytem
	// The initializaation function must have the following signature:
	// void init(Subsystem*, SubsystemParameters*);
	
	// Call the function, and catch any error message
	try {
		if(functionExists(L_, initLUAFcn))
			call_function<void>(L_, initLUAFcn.c_str(), (Subsystem*)this, subsystemParameters);
		else {
			cout << "Initilization function '" << initLUAFcn;
			cout << "' for Scripted Subsystem '" << subsystemName;
			cout << "' does not exist!" << endl;
		}
	}
	catch(error& e) {
		object error_msg(from_stack(e.state(), -1));
		cout << "Error Initializing Scripted Subsystem '" << subsystemName << "'" << endl;
		cout << error_msg << endl;
	}
}

ScriptedSubsystem* ScriptedSubsystem::clone() const {
	ScriptedSubsystem* newSS = new ScriptedSubsystem(this->name, this->initLUAFcn, this->canPerformLUAFcn, this->canExtendLUAFcn);
	return newSS;
}

ScriptedSubsystem::~ScriptedSubsystem() {
	// Delete the SubsystemParameters object from the heap
	delete subsystemParameters;
}

bool ScriptedSubsystem::canPerform(const State* oldState, State* newState, const Task* task, Position* position, Environment* environment, NodeDependencies* deps) const {
	// Call the canPerform for the ScriptedSubsystem
	// The canPerform function must have the following signature:
	// bool canPerform(const State* oldState, State* newState, const Task* task, Position* position, Environment* environment, NodeDependencies* deps, SubsystemParameters* subsystemParameters);

	static bool canPerformFcnExists = false;
	// Call the function, and catch any error message
	try {
		// Check if the LUA function exists
		if(canPerformFcnExists || functionExists(L_, initLUAFcn)) {
			canPerformFcnExists = true;
			return call_function<bool>(L_, canPerformLUAFcn.c_str(), oldState, newState, task, position, environment, deps, subsystemParameters);
		}
		else {
			cout << "canPerform function '" << initLUAFcn;
			cout << "' for Scripted Subsystem '" << getName();
			cout << "' does not exist!" << endl;
		}
	}
	catch(error& e) {
		object error_msg(from_stack(e.state(), -1));
		cout << "Error calling canPerform on Scripted Subsystem '" << getName() << "'" << endl;
		cout << error_msg << endl;
	}
	return false;
}

bool ScriptedSubsystem::canExtend(State* state, Position* position, Environment* environment, const double evalToTime, NodeDependencies* deps) const {
	// Call the canExtend for the ScriptedSubsystem
	// The canExtend function must have the following signature:
	// bool canExtend(State* state, Position* position, Environment* environment, const double evalToTime, NodeDependencies* deps, SubsystemParameters* subsystemParameters);
	
	static bool canExtendFcnExists = false;
	// Call the function, and catch any error message
	try {
		// Check if the LUA function exists
		if(canExtendFcnExists || functionExists(L_, initLUAFcn)) {
			canExtendFcnExists = true;
			return call_function<bool>(L_, canExtendLUAFcn.c_str(), state, position, environment, evalToTime, deps, subsystemParameters);
		}
		else {
			cout << "canExtend function '" << initLUAFcn;
			cout << "' for Scripted Subsystem '" << getName();
			cout << "' does not exist!" << endl;
		}
	}
	catch(error& e) {
		object error_msg(from_stack(e.state(), -1));
		cout << "Error calling canExtend on Scripted Subsystem '" << getName() << "'" << endl;
		cout << error_msg << endl;
	}
	return false;
}

}}} // horizon::sub::Script
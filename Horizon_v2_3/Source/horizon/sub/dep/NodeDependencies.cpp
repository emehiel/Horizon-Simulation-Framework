#include "NodeDependencies.h"

namespace horizon {
	namespace sub {
		namespace dep {

CRITICAL_SECTION NodeDependencies::cs;

NodeDependencies::NodeDependencies() { depInstance = Dependencies::Instance(); };

NodeDependencies::NodeDependencies(const NodeDependencies& other) {
	// First copy all the maps
	this->intDependencies = other.intDependencies;
	this->doubleDependencies = other.doubleDependencies;
	this->floatDependencies = other.floatDependencies;
	this->boolDependencies = other.boolDependencies;
	this->matrixDependencies = other.matrixDependencies;
	this->quatDependencies = other.quatDependencies;
	this->intDependencies_scripted = other.intDependencies_scripted;
	this->doubleDependencies_scripted = other.doubleDependencies_scripted;
	this->floatDependencies_scripted = other.floatDependencies_scripted;
	this->boolDependencies_scripted = other.boolDependencies_scripted;
	this->matrixDependencies_scripted = other.matrixDependencies_scripted;
	this->quatDependencies_scripted = other.quatDependencies_scripted;

	// Copy dependencies pointer
	this->depInstance = other.depInstance;
	
	// Copy scripting flag
	this->scriptingEnabled = other.scriptingEnabled;

	// New lua state
	this->L = getNewLuaState();
}

NodeDependencies::~NodeDependencies() {};

void NodeDependencies::setThreadNum(int threadNum) {
	this->threadNum = threadNum;
}

void NodeDependencies::setDependencies(Dependencies* deps) {
	this->depInstance = deps;
}

Profile<int>	NodeDependencies::callIntDependency(string callKey) {
	// Initialize return profile, it will be returned if no dependencies are found
	Profile<int> prof;
	// Check if scripting support is enabled
	if(scriptingEnabled) {
		// First check if there is a static dependency registered for callKey
		if(intDependencies.find(callKey) == intDependencies.end()) {
			// No static dependency, search for a scripted dependency
			if(intDependencies_scripted.find(callKey) != intDependencies_scripted.end()) {
				// Scripted dependency exists, get the name of the LUA function to call
				string key = intDependencies_scripted[callKey];
				// Call the function, and catch any error message
				try {
					// Check if the LUA function exists
					if(functionExists(L, key)) {
						// LUA function exists, call it
						return call_function< Profile<int> >(L, key.c_str());
					}
					else {
						// LUA function doesn't exist
						cout << "Dependency function '" << key;
						cout << "' does not exist!" << endl;
					}
				} // end try block
				catch(error& e) {
					// Error occured in call_function
					object error_msg(from_stack(e.state(), -1));
					cout << "Error calling dependency function '" << key << "'" << endl;
					cout << error_msg << endl;
				} // end catch block
			} // end check for scripted dependency
			else {
				cout << "Cannot find static dependency function for key '" << callKey << "'!" << endl;
			}
		} // end check for static dependnecy
		else {
			// static dependency exists for key, call it
			/*
			EnterCriticalSection(&cs);
			depInstance->setThreadState(threadNum);
			Profile<int> ret = (depInstance->*(intDependencies[callKey]))();
			LeaveCriticalSection(&cs);
			return ret;
			*/
		}
	} // end check for scripting enabled
	else {
		// scripting is not enabled, try to find a static dependency for key
		if(intDependencies.find(callKey) != intDependencies.end()) {
			// static dependency exists, call it
			return (depInstance->*(intDependencies[callKey]))();
		}
		else {
			// no static dependency
			cout << "Cannot find static dependency function for key '" << callKey << "'!" << endl;
		}
	}
	// no dependencies match key, return the empty Profile
	return prof;
}

Profile<double> NodeDependencies::callDoubleDependency(string callKey) {
	// Initialize return profile, it will be returned if no dependencies are found
	Profile<double> prof;
	// Check if scripting support is enabled
	if(scriptingEnabled) {
		// First check if there is a static dependency registered for callKey
		if(doubleDependencies.find(callKey) == doubleDependencies.end()) {
			// No static dependency, search for a scripted dependency
			if(doubleDependencies_scripted.find(callKey) != doubleDependencies_scripted.end()) {
				// Scripted dependency exists, get the name of the LUA function to call
				string key = doubleDependencies_scripted[callKey];
				// Call the function, and catch any error message
				try {
					// Check if the LUA function exists
					if(functionExists(L, key)) {
						// LUA function exists, call it
						return call_function< Profile<double> >(L, key.c_str(), depInstance);
					}
					else {
						// LUA function doesn't exist
						cout << "Dependency function '" << key;
						cout << "' does not exist!" << endl;
					}
				} // end try block
				catch(error& e) {
					// Error occured in call_function
					object error_msg(from_stack(e.state(), -1));
					cout << "Error calling dependency function '" << key << "'" << endl;
					cout << error_msg << endl;
				} // end catch block
			} // end check for scripted dependency
			else {
				cout << "Cannot find static dependency function for key '" << callKey << "'!" << endl;
			}
		} // end check for static dependnecy
		else {
			// static dependency exists for key, call it
			Profile<double> ret = (depInstance->*(doubleDependencies[callKey]))();
			return ret;
		}
	} // end check for scripting enabled
	else {
		// scripting is not enabled, try to find a static dependency for key
		if(doubleDependencies.find(callKey) != doubleDependencies.end()) {
			// static dependency exists, call it
			return (depInstance->*(doubleDependencies[callKey]))();
		}
		else {
			// no static dependency
			cout << "Cannot find static dependency function for key '" << callKey << "'!" << endl;
		}
	}
	// no dependencies match key, return the empty Profile
	return prof;
}

Profile<float>	NodeDependencies::callFloatDependency(string callKey) {
	// Initialize return profile, it will be returned if no dependencies are found
	Profile<float> prof;
	// Check if scripting support is enabled
	if(scriptingEnabled) {
		// First check if there is a static dependency registered for callKey
		if(floatDependencies.find(callKey) == floatDependencies.end()) {
			// No static dependency, search for a scripted dependency
			if(floatDependencies_scripted.find(callKey) != floatDependencies_scripted.end()) {
				// Scripted dependency exists, get the name of the LUA function to call
				string key = floatDependencies_scripted[callKey];
				// Call the function, and catch any error message
				try {
					// Check if the LUA function exists
					if(functionExists(L, key)) {
						// LUA function exists, call it
						return call_function< Profile<float> >(L, key.c_str());
					}
					else {
						// LUA function doesn't exist
						cout << "Dependency function '" << key;
						cout << "' does not exist!" << endl;
					}
				} // end try block
				catch(error& e) {
					// Error occured in call_function
					object error_msg(from_stack(e.state(), -1));
					cout << "Error calling dependency function '" << key << "'" << endl;
					cout << error_msg << endl;
				} // end catch block
			} // end check for scripted dependency
			else {
				cout << "Cannot find static dependency function for key '" << callKey << "'!" << endl;
			}
		} // end check for static dependnecy
		else {
			// static dependency exists for key, call it
			return (depInstance->*(floatDependencies[callKey]))();

		}
	} // end check for scripting enabled
	else {
		// scripting is not enabled, try to find a static dependency for key
		if(floatDependencies.find(callKey) != floatDependencies.end()) {
			// static dependency exists, call it
			/*
			EnterCriticalSection(&cs);
			depInstance->setThreadState(threadNum);
			Profile<float> ret = (depInstance->*(floatDependencies[callKey]))();
			LeaveCriticalSection(&cs);
			return ret;
			*/
		}
		else {
			// no static dependency
			cout << "Cannot find static dependency function for key '" << callKey << "'!" << endl;
		}
	}
	// no dependencies match key, return the empty Profile
	return prof;
}

Profile<bool>	NodeDependencies::callBoolDependency(string callKey) {
	// Initialize return profile, it will be returned if no dependencies are found
	Profile<bool> prof;
	// Check if scripting support is enabled
	if(scriptingEnabled) {
		// First check if there is a static dependency registered for callKey
		if(boolDependencies.find(callKey) == boolDependencies.end()) {
			// No static dependency, search for a scripted dependency
			if(boolDependencies_scripted.find(callKey) != boolDependencies_scripted.end()) {
				// Scripted dependency exists, get the name of the LUA function to call
				string key = boolDependencies_scripted[callKey];
				// Call the function, and catch any error message
				try {
					// Check if the LUA function exists
					if(functionExists(L, key)) {
						// LUA function exists, call it
						return call_function< Profile<bool> >(L, key.c_str());
					}
					else {
						// LUA function doesn't exist
						cout << "Dependency function '" << key;
						cout << "' does not exist!" << endl;
					}
				} // end try block
				catch(error& e) {
					// Error occured in call_function
					object error_msg(from_stack(e.state(), -1));
					cout << "Error calling dependency function '" << key << "'" << endl;
					cout << error_msg << endl;
				} // end catch block
			} // end check for scripted dependency
			else {
				cout << "Cannot find static dependency function for key '" << callKey << "'!" << endl;
			}
		} // end check for static dependnecy
		else {
			// static dependency exists for key, call it
			/*
			EnterCriticalSection(&cs);
			depInstance->setThreadState(threadNum);
			Profile<bool> ret = (depInstance->*(boolDependencies[callKey]))();
			LeaveCriticalSection(&cs);
			return ret;
			*/
		}
	} // end check for scripting enabled
	else {
		// scripting is not enabled, try to find a static dependency for key
		if(boolDependencies.find(callKey) != boolDependencies.end()) {
			// static dependency exists, call it
			return (depInstance->*(boolDependencies[callKey]))();
		}
		else {
			// no static dependency
			cout << "Cannot find static dependency function for key '" << callKey << "'!" << endl;
		}
	}
	// no dependencies match key, return the empty Profile
	return prof;
}

Profile<Matrix> NodeDependencies::callMatrixDependency(string callKey) {
	// Initialize return profile, it will be returned if no dependencies are found
	Profile<Matrix> prof;
	// Check if scripting support is enabled
	if(scriptingEnabled) {
		// First check if there is a static dependency registered for callKey
		if(matrixDependencies.find(callKey) == matrixDependencies.end()) {
			// No static dependency, search for a scripted dependency
			if(matrixDependencies_scripted.find(callKey) != matrixDependencies_scripted.end()) {
				// Scripted dependency exists, get the name of the LUA function to call
				string key = matrixDependencies_scripted[callKey];
				// Call the function, and catch any error message
				try {
					// Check if the LUA function exists
					if(functionExists(L, key)) {
						// LUA function exists, call it
						return call_function< Profile<Matrix> >(L, key.c_str());
					}
					else {
						// LUA function doesn't exist
						cout << "Dependency function '" << key;
						cout << "' does not exist!" << endl;
					}
				} // end try block
				catch(error& e) {
					// Error occured in call_function
					object error_msg(from_stack(e.state(), -1));
					cout << "Error calling dependency function '" << key << "'" << endl;
					cout << error_msg << endl;
				} // end catch block
			} // end check for scripted dependency
			else {
				cout << "Cannot find static dependency function for key '" << callKey << "'!" << endl;
			}
		} // end check for static dependnecy
		else {
			// static dependency exists for key, call it
			/*
			EnterCriticalSection(&cs);
			depInstance->setThreadState(threadNum);
			Profile<Matrix> ret = (depInstance->*(matrixDependencies[callKey]))();
			LeaveCriticalSection(&cs);
			return ret;
			*/
		}
	} // end check for scripting enabled
	else {
		// scripting is not enabled, try to find a static dependency for key
		if(matrixDependencies.find(callKey) != matrixDependencies.end()) {
			// static dependency exists, call it
			return (depInstance->*(matrixDependencies[callKey]))();
		}
		else {
			// no static dependency
			cout << "Cannot find static dependency function for key '" << callKey << "'!" << endl;
		}
	}
	// no dependencies match key, return the empty Profile
	return prof;
}

Profile<Quat>	NodeDependencies::callQuatDependency(string callKey) {
	// Initialize return profile, it will be returned if no dependencies are found
	Profile<Quat> prof;
	// Check if scripting support is enabled
	if(scriptingEnabled) {
		// First check if there is a static dependency registered for callKey
		if(quatDependencies.find(callKey) == quatDependencies.end()) {
			// No static dependency, search for a scripted dependency
			if(quatDependencies_scripted.find(callKey) != quatDependencies_scripted.end()) {
				// Scripted dependency exists, get the name of the LUA function to call
				string key = quatDependencies_scripted[callKey];
				// Call the function, and catch any error message
				try {
					// Check if the LUA function exists
					if(functionExists(L, key)) {
						// LUA function exists, call it
						return call_function< Profile<Quat> >(L, key.c_str());
					}
					else {
						// LUA function doesn't exist
						cout << "Dependency function '" << key;
						cout << "' does not exist!" << endl;
					}
				} // end try block
				catch(error& e) {
					// Error occured in call_function
					object error_msg(from_stack(e.state(), -1));
					cout << "Error calling dependency function '" << key << "'" << endl;
					cout << error_msg << endl;
				} // end catch block
			} // end check for scripted dependency
			else {
				cout << "Cannot find static dependency function for key '" << callKey << "'!" << endl;
			}
		} // end check for static dependnecy
		else {
			// static dependency exists for key, call it
			return (depInstance->*(quatDependencies[callKey]))();
		}
	} // end check for scripting enabled
	else {
		// scripting is not enabled, try to find a static dependency for key
		if(quatDependencies.find(callKey) != quatDependencies.end()) {
			// static dependency exists, call it
			/*
			EnterCriticalSection(&cs);
			depInstance->setThreadState(threadNum);
			Profile<Quat> ret = (depInstance->*(quatDependencies[callKey]))();
			LeaveCriticalSection(&cs);
			
			return ret;
			*/
		}
		else {
			// no static dependency
			cout << "Cannot find static dependency function for key '" << callKey << "'!" << endl;
		}
	}
	// no dependencies match key, return the empty Profile
	return prof;
}

void NodeDependencies::addDependency(string callKey, IntDependency dep) {
	intDependencies[callKey] = dep;
}

void NodeDependencies::addDependency(string callKey, DoubleDependency dep) {
	doubleDependencies[callKey] = dep;
}

void NodeDependencies::addDependency(string callKey, FloatDependency dep) {
	floatDependencies[callKey] = dep;
}

void NodeDependencies::addDependency(string callKey, BoolDependency dep) {
	boolDependencies[callKey] = dep;
}

void NodeDependencies::addDependency(string callKey, MatrixDependency dep) {
	matrixDependencies[callKey] = dep;
}

void NodeDependencies::addDependency(string callKey, QuatDependency dep) {
	quatDependencies[callKey] = dep;
}

void NodeDependencies::addIntScriptedDependency(string callKey, string key) {
	intDependencies_scripted[callKey] = key;
}

void NodeDependencies::addDoubleScriptedDependency(string callKey, string key) {
	doubleDependencies_scripted[callKey] = key;
}

void NodeDependencies::addFloatScriptedDependency(string callKey, string key) {
	floatDependencies_scripted[callKey] = key;
}

void NodeDependencies::addBoolScriptedDependency(string callKey, string key) {
	boolDependencies_scripted[callKey] = key;
}

void NodeDependencies::addMatrixScriptedDependency(string callKey, string key) {
	matrixDependencies_scripted[callKey] = key;
}

void NodeDependencies::addQuatScriptedDependency(string callKey, string key) {
	quatDependencies_scripted[callKey] = key;
}

}}} // end namespace horizon::sub::dep
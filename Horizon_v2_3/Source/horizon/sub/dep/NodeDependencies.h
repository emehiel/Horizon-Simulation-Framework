/**
* \file NodeDependencies.h
* This file contains the NodeDependencies class.
* @author Brian Butler
*/

#ifndef NODEDEPENDENCIES_H
#define NODEDEPENDENCIES_H

// Windows Include
#include <windows.h>

// HSF Includes
#include "Dependencies.h"
#include "horizon/script/ScriptingUtil.h"
#include "horizon/script/luaDefs.h"


// STL Includes
#include <string>
#include <map>

// LUA / Luabind Includees
#include <lua.hpp>
#include <luabind/luabind.hpp>

// HSF Usings
using horizon::script::functionExists;
using namespace horizon::script;

// LUA / Luabind Usings
using namespace luabind;

namespace horizon {
	namespace sub {
		namespace dep {

	class Dependencies;
	/**
	* typedef for an IntDependency; an IntDependnecy is a a pointer to a member function of the
	* Dependencies class that returns a Profile<int>.
	*/
	typedef Profile<int>	(Dependencies::*IntDependency) ();
	/**
	* typedef for an FloatDependency; an FloatDependency is a a pointer to a member function of the
	* Dependencies class that returns a Profile<float>.
	*/
	typedef Profile<float>	(Dependencies::*FloatDependency) ();
	/**
	* typedef for an DoubleDependency; an DoubleDependency is a a pointer to a member function of the
	* Dependencies class that returns a Profile<double>.
	*/
	typedef Profile<double> (Dependencies::*DoubleDependency) ();
	/**
	* typedef for an BoolDependency; an BoolDependency is a a pointer to a member function of the
	* Dependencies class that returns a Profile<bool>.
	*/
	typedef Profile<bool>	(Dependencies::*BoolDependency) ();
	/**
	* typedef for an MatrixDependency; an MatrixDependency is a a pointer to a member function of the
	* Dependencies class that returns a Profile<Matrix>.
	*/
	typedef Profile<Matrix>	(Dependencies::*MatrixDependency) ();
	/**
	* typedef for an QuatDependency; an QuatDependency is a a pointer to a member function of the
	* Dependencies class that returns a Profile<Quat>.
	*/
	typedef Profile<Quat>	(Dependencies::*QuatDependency) ();

/**
* A NodeDependencies class.
* The NodeDependencies class provides a consistend interface for Subsystems
* to call dependencies, both scripted and non-scripted.
* @author Brian Butler
*/
class NodeDependencies {

	Dependencies* depInstance;
	bool scriptingEnabled;
	lua_State* L;
	int threadNum;

	map<string, IntDependency>		intDependencies;
	map<string, DoubleDependency>	doubleDependencies;
	map<string, FloatDependency>	floatDependencies;
	map<string, BoolDependency>		boolDependencies;
	map<string, MatrixDependency>	matrixDependencies;
	map<string, QuatDependency>		quatDependencies;
	map<string, string>		intDependencies_scripted;
	map<string, string>		doubleDependencies_scripted;
	map<string, string>		floatDependencies_scripted;
	map<string, string>		boolDependencies_scripted;
	map<string, string>		matrixDependencies_scripted;
	map<string, string>		quatDependencies_scripted;

public:
	/** The constructor for the NodeDependencies class. */
	NodeDependencies();

	/** The copy constructor for the NodeDependencies class. */
	NodeDependencies(const NodeDependencies& other);

	/** The destructor for the NodeDependencies class. */
	~NodeDependencies();

	void setThreadNum(int threadNum);

	void setDependencies(Dependencies* deps);

	static CRITICAL_SECTION cs;

	/**
	* Calls a dependency function that returns a Profile<int>.
	* This function calls both scripted and static dependencies.
	* If scripting support is not enabled, then a static dependency
	* is attempted to be found that matches callKey.<BR>
	*
	* If scripting support is enabled, then first static dependencies
	* are searched for one with a match to callKey. If a static dependency
	* is found then it is called. If no static dependencies
	* are found, then scripted dependencies are searched. If one is found 
	* the name of the corresponding LUA function is found and the
	* dependency is called.<BR>
	*
	* Note that if both static and scripted dependencies have been added
	* with the same callKey, then the static dependency will always be
	* called regardless of whether scripting support is enabled.
	* 
	* @param callKey The key that the dependency was added with.
	* @return A Profile<int> that is returned from the dependency function.
	* If no dependency was found using the callKey, then an empty
	* Profile<int> is returned.
	*/
	Profile<int>	callIntDependency(string callKey);

	/**
	* Calls a dependency function that returns a Profile<double>.
	* This function calls both scripted and static dependencies.
	* If scripting support is not enabled, then a static dependency
	* is attempted to be found that matches callKey.<BR>
	*
	* If scripting support is enabled, then first static dependencies
	* are searched for one with a match to callKey. If a static dependency
	* is found then it is called. If no static dependencies
	* are found, then scripted dependencies are searched. If one is found 
	* the name of the corresponding LUA function is found and the
	* dependency is called.<BR>
	*
	* Note that if both static and scripted dependencies have been added
	* with the same callKey, then the static dependency will always be
	* called regardless of whether scripting support is enabled.
	* 
	* @param callKey The key that the dependency was added with.
	* @return A Profile<double> that is returned from the dependency function.
	* If no dependency was found using the callKey, then an empty
	* Profile<double> is returned.
	*/
	Profile<double> callDoubleDependency(string callKey);

	/**
	* Calls a dependency function that returns a Profile<float>.
	* This function calls both scripted and static dependencies.
	* If scripting support is not enabled, then a static dependency
	* is attempted to be found that matches callKey.<BR>
	*
	* If scripting support is enabled, then first static dependencies
	* are searched for one with a match to callKey. If a static dependency
	* is found then it is called. If no static dependencies
	* are found, then scripted dependencies are searched. If one is found 
	* the name of the corresponding LUA function is found and the
	* dependency is called.<BR>
	*
	* Note that if both static and scripted dependencies have been added
	* with the same callKey, then the static dependency will always be
	* called regardless of whether scripting support is enabled.
	* 
	* @param callKey The key that the dependency was added with.
	* @return A Profile<float> that is returned from the dependency function.
	* If no dependency was found using the callKey, then an empty
	* Profile<float> is returned.
	*/
	Profile<float>	callFloatDependency(string callKey);

	/**
	* Calls a dependency function that returns a Profile<bool>.
	* This function calls both scripted and static dependencies.
	* If scripting support is not enabled, then a static dependency
	* is attempted to be found that matches callKey.<BR>
	*
	* If scripting support is enabled, then first static dependencies
	* are searched for one with a match to callKey. If a static dependency
	* is found then it is called. If no static dependencies
	* are found, then scripted dependencies are searched. If one is found 
	* the name of the corresponding LUA function is found and the
	* dependency is called.<BR>
	*
	* Note that if both static and scripted dependencies have been added
	* with the same callKey, then the static dependency will always be
	* called regardless of whether scripting support is enabled.
	* 
	* @param callKey The key that the dependency was added with.
	* @return A Profile<bool> that is returned from the dependency function.
	* If no dependency was found using the callKey, then an empty
	* Profile<bool> is returned.
	*/
	Profile<bool>	callBoolDependency(string callKey);

	/**
	* Calls a dependency function that returns a Profile<Matrix>.
	* This function calls both scripted and static dependencies.
	* If scripting support is not enabled, then a static dependency
	* is attempted to be found that matches callKey.<BR>
	*
	* If scripting support is enabled, then first static dependencies
	* are searched for one with a match to callKey. If a static dependency
	* is found then it is called. If no static dependencies
	* are found, then scripted dependencies are searched. If one is found 
	* the name of the corresponding LUA function is found and the
	* dependency is called.<BR>
	*
	* Note that if both static and scripted dependencies have been added
	* with the same callKey, then the static dependency will always be
	* called regardless of whether scripting support is enabled.
	* 
	* @param callKey The key that the dependency was added with.
	* @return A Profile<Matrix> that is returned from the dependency function.
	* If no dependency was found using the callKey, then an empty
	* Profile<Matrix> is returned.
	*/
	Profile<Matrix> callMatrixDependency(string callKey);

	/**
	* Calls a dependency function that returns a Profile<Quat>.
	* This function calls both scripted and static dependencies.
	* If scripting support is not enabled, then a static dependency
	* is attempted to be found that matches callKey.<BR>
	*
	* If scripting support is enabled, then first static dependencies
	* are searched for one with a match to callKey. If a static dependency
	* is found then it is called. If no static dependencies
	* are found, then scripted dependencies are searched. If one is found 
	* the name of the corresponding LUA function is found and the
	* dependency is called.<BR>
	*
	* Note that if both static and scripted dependencies have been added
	* with the same callKey, then the static dependency will always be
	* called regardless of whether scripting support is enabled.
	* 
	* @param callKey The key that the dependency was added with.
	* @return A Profile<Quat> that is returned from the dependency function.
	* If no dependency was found using the callKey, then an empty
	* Profile<Quat> is returned.
	*/
	Profile<Quat>	callQuatDependency(string callKey);

	/**
	* Adds an IntDependency that can be called with the specified key.
	*
	* @param callKey The key that must be passed to callIntDependency
	* to call the dependency being added.
	* @param dep The IntDependnecy to be added to the NodeDependnecies class.
	*/
	void addDependency(string callKey, IntDependency dep);
	/**
	* Adds a DoubleDependency that can be called with the specified key.
	*
	* @param callKey The key that must be passed to callDoubleDependency
	* to call the dependency being added.
	* @param dep The DoubleDependency to be added to the NodeDependnecies class.
	*/
	void addDependency(string callKey, DoubleDependency dep);
	/**
	* Adds a FloatDependency that can be called with the specified key.
	*
	* @param callKey The key that must be passed to callFloatDependency
	* to call the dependency being added.
	* @param dep The FloatDependency to be added to the NodeDependnecies class.
	*/
	void addDependency(string callKey, FloatDependency dep);
	/**
	* Adds a BoolDependency that can be called with the specified key.
	*
	* @param callKey The key that must be passed to callBoolDependency
	* to call the dependency being added.
	* @param dep The BoolDependency to be added to the NodeDependnecies class.
	*/
	void addDependency(string callKey, BoolDependency dep);
	/**
	* Adds a MatrixDependency that can be called with the specified key.
	*
	* @param callKey The key that must be passed to callMatrixDependency
	* to call the dependency being added.
	* @param dep The MatrixDependency to be added to the NodeDependnecies class.
	*/
	void addDependency(string callKey, MatrixDependency dep);
	/**
	* Adds a QuatDependency that can be called with the specified key.
	*
	* @param callKey The key that must be passed to callQuatDependency
	* to call the dependency being added.
	* @param dep The QuatDependency to be added to the NodeDependnecies class.
	*/
	void addDependency(string callKey, QuatDependency dep);

	/**
	* Adds a scripted dependency that returns a Profile<int>.
	*
	* @param callKey The key that must be passed to callIntDependency
	* to call the dependency being added.
	* @param key The name of the LUA function that will be called
	* as the dependency. This function must take no arguments, get
	* data from the Dependencies class using the getState(assetNum) 
	* function exposed to LUA and must return a Profile<int>
	* (the class name in LUA is iProfile).
	* <BR><BR>
	* An example LUA function:<BR>
	* <CODE>function depFunction()<BR>
	* &nbsp;&nbsp;&nbsp;prof = iProfile()<BR>
	* &nbsp;&nbsp;&nbsp;state = getState(1) --Get the state for Asset 1<BR>
	* &nbsp;&nbsp;&nbsp;... --Process state data, fill in prof<BR>
	* &nbsp;&nbsp;&nbsp;return prof --Return the profile<BR>
	* end</CODE>
	*/
	void addIntScriptedDependency(string callKey, string key);
	
	/**
	* Adds a scripted dependency that returns a Profile<double>.
	*
	* @param callKey The key that must be passed to callDoubleDependency
	* to call the dependency being added.
	* @param key The name of the LUA function that will be called
	* as the dependency. This function must take no arguments, get
	* data from the Dependencies class using the getState(assetNum) 
	* function exposed to LUA and must return a Profile<double>
	* (the class name in LUA is dProfile).
	* <BR><BR>
	* An example LUA function:<BR>
	* <CODE>function depFunction()<BR>
	* &nbsp;&nbsp;&nbsp;prof = dProfile()<BR>
	* &nbsp;&nbsp;&nbsp;state = getState(1) --Get the state for Asset 1<BR>
	* &nbsp;&nbsp;&nbsp;... --Process state data, fill in prof<BR>
	* &nbsp;&nbsp;&nbsp;return prof --Return the profile<BR>
	* end</CODE>
	*/
	void addDoubleScriptedDependency(string callKey, string key);

	/**
	* Adds a scripted dependency that returns a Profile<float>.
	*
	* @param callKey The key that must be passed to callFloatDependency
	* to call the dependency being added.
	* @param key The name of the LUA function that will be called
	* as the dependency. This function must take no arguments, get
	* data from the Dependencies class using the getState(assetNum) 
	* function exposed to LUA and must return a Profile<float>
	* (the class name in LUA is fProfile).
	* <BR><BR>
	* An example LUA function:<BR>
	* <CODE>function depFunction()<BR>
	* &nbsp;&nbsp;&nbsp;prof = fProfile()<BR>
	* &nbsp;&nbsp;&nbsp;state = getState(1) --Get the state for Asset 1<BR>
	* &nbsp;&nbsp;&nbsp;... --Process state data, fill in prof<BR>
	* &nbsp;&nbsp;&nbsp;return prof --Return the profile<BR>
	* end</CODE>
	*/
	void addFloatScriptedDependency(string callKey, string key);

	/**
	* Adds a scripted dependency that returns a Profile<bool>.
	*
	* @param callKey The key that must be passed to callBoolDependency
	* to call the dependency being added.
	* @param key The name of the LUA function that will be called
	* as the dependency. This function must take no arguments, get
	* data from the Dependencies class using the getState(assetNum) 
	* function exposed to LUA and must return a Profile<bool>
	* (the class name in LUA is bProfile).
	* <BR><BR>
	* An example LUA function:<BR>
	* <CODE>function depFunction()<BR>
	* &nbsp;&nbsp;&nbsp;prof = bProfile()<BR>
	* &nbsp;&nbsp;&nbsp;state = getState(1) --Get the state for Asset 1<BR>
	* &nbsp;&nbsp;&nbsp;... --Process state data, fill in prof<BR>
	* &nbsp;&nbsp;&nbsp;return prof --Return the profile<BR>
	* end</CODE>
	*/
	void addBoolScriptedDependency(string callKey, string key);

	/**
	* Adds a scripted dependency that returns a Profile<Matrix>.
	*
	* @param callKey The key that must be passed to callMatrixDependency
	* to call the dependency being added.
	* @param key The name of the LUA function that will be called
	* as the dependency. This function must take no arguments, get
	* data from the Dependencies class using the getState(assetNum) 
	* function exposed to LUA and must return a Profile<Matrix>
	* (the class name in LUA is mProfile).
	* <BR><BR>
	* An example LUA function:<BR>
	* <CODE>function depFunction()<BR>
	* &nbsp;&nbsp;&nbsp;prof = mProfile()<BR>
	* &nbsp;&nbsp;&nbsp;state = getState(1) --Get the state for Asset 1<BR>
	* &nbsp;&nbsp;&nbsp;... --Process state data, fill in prof<BR>
	* &nbsp;&nbsp;&nbsp;return prof --Return the profile<BR>
	* end</CODE>
	*/
	void addMatrixScriptedDependency(string callKey, string key);

	/**
	* Adds a scripted dependency that returns a Profile<Quat>.
	*
	* @param callKey The key that must be passed to callQuatDependency
	* to call the dependency being added.
	* @param key The name of the LUA function that will be called
	* as the dependency. This function must take no arguments, get
	* data from the Dependencies class using the getState(assetNum) 
	* function exposed to LUA and must return a Profile<Quat>
	* (the class name in LUA is qProfile).
	* <BR><BR>
	* An example LUA function:<BR>
	* <CODE>function depFunction()<BR>
	* &nbsp;&nbsp;&nbsp;prof = qProfile()<BR>
	* &nbsp;&nbsp;&nbsp;state = getState(1) --Get the state for Asset 1<BR>
	* &nbsp;&nbsp;&nbsp;... --Process state data, fill in prof<BR>
	* &nbsp;&nbsp;&nbsp;return prof --Return the profile<BR>
	* end</CODE>
	*/
	void addQuatScriptedDependency(string callKey, string key);

	/**
	* Enables scripting support for the NodeDependencies class.
	*/
	void enableScriptingSupport();
	/**
	* Disables scripting support for the NodeDependencies class.
	*/
	void disableScriptingSupport();

	/**
	* Sets the pointer to the LUA state that is used.
	* @param L A pointer to a LUA state.
	*/
	void setLuaState(lua_State *L);
	/**
	* Gets the pointer to the LUA state that is used.
	* @return A pointer to the LUA state.
	*/
	lua_State* getLuaState();
};

inline void NodeDependencies::enableScriptingSupport() {
	scriptingEnabled = true;
}
inline void NodeDependencies::disableScriptingSupport() {
	scriptingEnabled = false;
}
inline void NodeDependencies::setLuaState(lua_State *L) {
	this->L = L;
}
inline lua_State* NodeDependencies::getLuaState() {
	return L;
}

}}} // end namespace horizon::sub::dep
#endif // NODEDEPENDENCIES_H
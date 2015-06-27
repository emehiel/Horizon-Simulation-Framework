#ifndef NODEDEPENDENCIES_H
#define NODEDEPENDENCIES_H

#include "Dependencies.h"
#include <string>
#include <map>

#include <lua.hpp>
#include <luabind/luabind.hpp>
using namespace luabind;

namespace horizon {
	namespace sub {
		namespace dep {

	class Dependencies;
	typedef Profile<int>	(Dependencies::*IntDependency) ();
	typedef Profile<float>	(Dependencies::*FloatDependency) ();
	typedef Profile<double> (Dependencies::*DoubleDependency) ();
	typedef Profile<bool>	(Dependencies::*BoolDependency) ();
	typedef Profile<Matrix>	(Dependencies::*MatrixDependency) ();
	typedef Profile<Quat>	(Dependencies::*QuatDependency) ();

class NodeDependencies {
	Dependencies* depInstance;
	bool scriptingEnabled;
	lua_State* L;

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
	NodeDependencies();
	
	~NodeDependencies();

	Profile<int>	callIntDependency(string callKey);
	Profile<double> callDoubleDependency(string callKey);
	Profile<float>	callFloatDependency(string callKey);
	Profile<bool>	callBoolDependency(string callKey);
	Profile<Matrix> callMatrixDependency(string callKey);
	Profile<Quat>	callQuatDependency(string callKey);

	void addDependency(string callKey, IntDependency dep);
	void addDependency(string callKey, DoubleDependency dep);
	void addDependency(string callKey, FloatDependency dep);
	void addDependency(string callKey, BoolDependency dep);
	void addDependency(string callKey, MatrixDependency dep);
	void addDependency(string callKey, QuatDependency dep);

	void addIntScriptedDependency(string callKey, string key);
	void addDoubleScriptedDependency(string callKey, string key);
	void addFloatScriptedDependency(string callKey, string key);
	void addBoolScriptedDependency(string callKey, string key);
	void addMatrixScriptedDependency(string callKey, string key);
	void addQuatScriptedDependency(string callKey, string key);

	void enableScriptingSupport();
	void disableScriptingSupport();

	void setLuaState(lua_State *L);
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
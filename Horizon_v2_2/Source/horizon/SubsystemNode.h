#ifndef SUBSYSTEMNODE_H
#define SUBSYSTEMNODE_H

#include <vector>
#include "Subsystem.h"
#include "util/Matrix.h"
#include "util/Quat.h"
#include "sub/dep/NodeDependencies.h"

namespace horizon {

// Forward declaration to avoid circular include problems
class Subsystem;
class Asset;
using namespace horizon::sub::dep;

class SubsystemNode
{
	bool checked;
	bool scriptingEnabled;
	const Subsystem* subsystem;
	Asset* asset;
	size_t nAsset;
	vector<SubsystemNode*> preceedingNodes;
	NodeDependencies dependencies;
	
public:

	SubsystemNode(const Subsystem*, Asset*);

	~SubsystemNode();

	void reset();

	void addPreceedingNode(SubsystemNode*);

	vector<SubsystemNode*> getPreceedingNodes();

	const Asset* getAsset() const;

	const Subsystem* getSubsystem() const;

	const size_t getAssetNum() const;

	void setAssetNum(const size_t assetNum);

	bool canPerform(State* newState, const Task* task, Environment* environment, const double evalToTime, const bool mustEvaluate);

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

inline const Asset* SubsystemNode::getAsset() const {
	return asset; }

inline const Subsystem* SubsystemNode::getSubsystem() const {
	return subsystem; }

inline const size_t SubsystemNode::getAssetNum() const {
	return nAsset; }

inline void SubsystemNode::setAssetNum(const size_t assetNum) {
	nAsset = assetNum; }

inline vector<SubsystemNode*> SubsystemNode::getPreceedingNodes() {
	return preceedingNodes; }

inline void SubsystemNode::addDependency(string key, IntDependency dep) {
	dependencies.addDependency(key, dep);
}
inline void SubsystemNode::addDependency(string key, DoubleDependency dep) {
	dependencies.addDependency(key, dep);
}
inline void SubsystemNode::addDependency(string key, FloatDependency dep) {
	dependencies.addDependency(key, dep);
}
inline void SubsystemNode::addDependency(string key, BoolDependency dep) {
	dependencies.addDependency(key, dep);
}
inline void SubsystemNode::addDependency(string key, MatrixDependency dep) {
	dependencies.addDependency(key, dep);
}
inline void SubsystemNode::addDependency(string key, QuatDependency dep) {
	dependencies.addDependency(key, dep);
}

inline void SubsystemNode::addIntScriptedDependency(string callKey, string key) {
	dependencies.addIntScriptedDependency(callKey, key);
}
inline void SubsystemNode::addDoubleScriptedDependency(string callKey, string key) {
	dependencies.addDoubleScriptedDependency(callKey, key);
}
inline void SubsystemNode::addFloatScriptedDependency(string callKey, string key) {
	dependencies.addFloatScriptedDependency(callKey, key);
}
inline void SubsystemNode::addBoolScriptedDependency(string callKey, string key) {
	dependencies.addBoolScriptedDependency(callKey, key);
}
inline void SubsystemNode::addMatrixScriptedDependency(string callKey, string key) {
	dependencies.addMatrixScriptedDependency(callKey, key);
}
inline void SubsystemNode::addQuatScriptedDependency(string callKey, string key) {
	dependencies.addQuatScriptedDependency(callKey, key);
}

inline void SubsystemNode::enableScriptingSupport() {
	scriptingEnabled = true;
	dependencies.enableScriptingSupport();
}
inline void SubsystemNode::disableScriptingSupport() {
	scriptingEnabled = false;
	dependencies.disableScriptingSupport();
}
inline void SubsystemNode::setLuaState(lua_State *L) {
	dependencies.setLuaState(L);
}
inline lua_State* SubsystemNode::getLuaState() {
	return dependencies.getLuaState();
}

} // end namespace horizon

#endif /*SUBSYSTEM_H_*/
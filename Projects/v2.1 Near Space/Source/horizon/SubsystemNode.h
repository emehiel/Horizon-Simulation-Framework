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

	void addDependency(string key, IntDependency dep);
	void addDependency(string key, DoubleDependency dep);
	void addDependency(string key, FloatDependency dep);
	void addDependency(string key, BoolDependency dep);
	void addDependency(string key, MatrixDependency dep);
	void addDependency(string key, QuatDependency dep);

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
	dependencies.addDependcy(key, dep);
}
inline void SubsystemNode::addDependency(string key, DoubleDependency dep) {
	dependencies.addDependcy(key, dep);
}
inline void SubsystemNode::addDependency(string key, FloatDependency dep) {
	dependencies.addDependcy(key, dep);
}
inline void SubsystemNode::addDependency(string key, BoolDependency dep) {
	dependencies.addDependcy(key, dep);
}
inline void SubsystemNode::addDependency(string key, MatrixDependency dep) {
	dependencies.addDependcy(key, dep);
}
inline void SubsystemNode::addDependency(string key, QuatDependency dep) {
	dependencies.addDependcy(key, dep);
}

} // end namespace horizon

#endif /*SUBSYSTEM_H_*/

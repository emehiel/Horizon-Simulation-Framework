#ifndef SUBSYSTEM_H
#define SUBSYSTEM_H

#include <string>

#include "Task.h"
#include "State.h"
#include "geom/Position.h"
#include "geom/Environment.h"
#include "../simParams.h"
#include "sub/dep/NodeDependencies.h"
#include "sub/SubsystemKeys.h"

namespace horizon {
	
	using std::string;
	using horizon::geom::Environment;
	using horizon::geom::Position;
	using horizon::sub::dep::NodeDependencies;

/**
 * A component representing an aspect of a system, possibly dependent on
 * other Subsystems.
 * @author Einar Pehrson 
 * @author Cory O'Connor
 */
class Subsystem {

	/** The unique name of the subsystem */
	const string name;

protected:

	vector<StateVarKey<int>>		ikeys;
	vector<StateVarKey<double>>		dkeys;
	vector<StateVarKey<float>>		fkeys;
	vector<StateVarKey<bool>>		bkeys;
	vector<StateVarKey<Matrix>>		mkeys;
	vector<StateVarKey<Quat>>		qkeys;

public:

	/**
	 * Creates a subsystem with the given name.
	 * @param name the unique name of the subsystem
	 */
	Subsystem(const string name);

	/** Destroys the subsystem */
	virtual ~Subsystem() = 0;

	/** Returns the name of the subsystem */
	const string& getName() const;

	/**
	 * Returns whether the subsystem can perform the operations necessary for
	 * the completion of the given task.
	 * @param oldState the state at the end of the previous event
	 * @param newState the new state to be generated, if possible
	 * @param task the task to be performed
	 * @param position
	 * @param environment the environment where the system resides
	 * @return true if the task can be performed
	 */
	virtual bool canPerform(const State* oldState, State* newState, const Task* task, Position* position, Environment* environment, NodeDependencies* dependencies) const = 0;

	virtual bool canExtend(State* newState, Position* position, Environment* environment, const double evalToTime, NodeDependencies* dependencies) const = 0;

	void addKey(const StateVarKey<int> keyIn);
	void addKey(const StateVarKey<double> keyIn);
	void addKey(const StateVarKey<float> keyIn);
	void addKey(const StateVarKey<bool> keyIn);
	void addKey(const StateVarKey<Matrix> keyIn);
	void addKey(const StateVarKey<Quat> keyIn);

	const vector< StateVarKey<int> > getIkeys() const;
	const vector< StateVarKey<double> > getDkeys() const;
	const vector< StateVarKey<float> > getFkeys() const;
	const vector< StateVarKey<bool> > getBkeys() const;
	const vector< StateVarKey<Matrix> > getMkeys() const;
	const vector< StateVarKey<Quat> > getQkeys() const;

	//  Any other member function you add must be declared const.  ie:
	//  returnType myMemberFunction(argList) const;
};

inline const string& Subsystem::getName() const {
	return name; }

inline void Subsystem::addKey(const StateVarKey<int> keyIn) {
	ikeys.push_back(keyIn); }
inline void Subsystem::addKey(const StateVarKey<double> keyIn) {
	dkeys.push_back(keyIn); }
inline void Subsystem::addKey(const StateVarKey<float> keyIn) {
	fkeys.push_back(keyIn); }
inline void Subsystem::addKey(const StateVarKey<bool> keyIn) {
	bkeys.push_back(keyIn); }
inline void Subsystem::addKey(const StateVarKey<Matrix> keyIn) {
	mkeys.push_back(keyIn); }
inline void Subsystem::addKey(const StateVarKey<Quat> keyIn) {
	qkeys.push_back(keyIn); }

inline const vector< StateVarKey<int> > Subsystem::getIkeys() const {
	return ikeys; }
inline const vector< StateVarKey<double> > Subsystem::getDkeys() const {
	return dkeys; }
inline const vector< StateVarKey<float> > Subsystem::getFkeys() const {
	return fkeys; }
inline const vector< StateVarKey<bool> > Subsystem::getBkeys() const {
	return bkeys; }
inline const vector< StateVarKey<Matrix> > Subsystem::getMkeys() const {
	return mkeys; }
inline const vector< StateVarKey<Quat> > Subsystem::getQkeys() const {
	return qkeys; }

} // end namespace horizon

#endif /*SUBSYSTEM_H_*/

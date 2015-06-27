#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>
#include <string>
#include <set>
#include <map>

#include "Asset.h"
#include "Task.h"
#include "State.h"
#include "SubsystemNode.h"
#include "Constraint.h"
#include "geom/Environment.h"
#include "../simParams.h"
#include "systemSchedule.h"
#include "sub/dep/Dependencies.h"


namespace horizon {

using std::set;
using std::map;
using std::list;
using std::string;
using horizon::geom::Environment;
using horizon::systemSchedule;

/**
 * A stateful entity that performs tasks.
 * @author Cory O'Connor
 * @author Einar Pehrson
 */
class System {

	/** The Assets of which the system consists */
	const vector<const Asset*> assets;

	/** The Subsystems of which the system consists */
	vector<SubsystemNode*> subsystemNodes;

	/** The constraints that further limit the legal states of the system */
	vector<const Constraint*> constraints;

	/** The Environment that the system exists in */
	Environment* environment;

	int threadNum;

public:
	
	System();

	/**
	 * Creates a system.
	 */
	System(const vector<const Asset*> assets, const vector<SubsystemNode*> subsystems, const vector<const Constraint*> constraints, 
		Environment* environment);

	/**
	 * Copy constructor.
	 */
	System(const System& other);

	/** Destroys the system */
	~System();

	/**
	 * Returns whether the system can perform the operations necessary for
	 * the completion of the given task.
	 * @param state the state at the end of the previous event
	 * @param task the task to be performed
	 * @param newState the new state to be generated, if possible.
	 * @return true if the task can be performed
	 */
	bool canPerform(systemSchedule* sysSched);

	const vector<const Asset*>& getAssets();

	const vector<SubsystemNode*>& getSubsystemNodes();

	const vector<const Constraint*>& getConstraints();

	Environment* getEnvironment();
	
	/**
	 * Checks whether the system model contains any ciruclar dependencies in its subsystemNodes
	 * @return true if there are any circular dependecies
	 */
	bool checkForCircularDependencies() const;

	void setThreadNum(int threadNum);

	void setDependencies(Dependencies* deps);

private:

	//bool checkConstrainedSubs(vector<SubsystemNode*> subNodeVec, systemSchedule* sysSched);

	bool checkSubs(vector<SubsystemNode*> subNodeVec, systemSchedule* sysSched, const bool mustEvaluate);

	bool checkSubForCircularDependncies(SubsystemNode* currNode, SubsystemNode* beginNode) const;
};

inline const vector<const Asset*>& System::getAssets()  {
	return assets; }

inline const vector<SubsystemNode*>& System::getSubsystemNodes()  {
	return subsystemNodes; }

inline const vector<const Constraint*>& System::getConstraints()  {
	return constraints; }

inline Environment* System::getEnvironment()  {
	return environment; }

} // end namespace horizon

#endif /*SYSTEM_H_*/

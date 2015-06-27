#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "systemSchedule.h"
#include "horizon/SubsystemNode.h"

namespace horizon {

using std::vector;
using horizon::systemSchedule;

/**
 * A system-external restriction on one or more state variables.
 * @author Einar Pehrson
 */
class Constraint {
	
	/** The names of the Subsystems on which the constraint operates */
	vector<SubsystemNode*> subsystemNodes;

public:

	/** Creates a constraint operating on the given Subsystems */
	Constraint();

	/** The virtual destructor */
	virtual ~Constraint() = 0;

	void addConstrianedSubNode(SubsystemNode* node);

	/**
	 * Applies the constraint to the appropriate variables in the given state,
	 * that contains updated state data for all the requested Subsystems.
	 * @param state a partially updated state
	 * @return true if the state passes the constraint check
	 */
	virtual bool accepts(const systemSchedule* schedule) const = 0;

	/** Returns the Subsystems on which the constraint operates */
	const vector<SubsystemNode*>& getSubsystemNodes() const;
};

} // end namespace horizon

#endif /*CONSTRAINT_H_*/

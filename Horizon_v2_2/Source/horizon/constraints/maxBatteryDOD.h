#ifndef MAXBATTERYDOD_H_
#define MAXBATTERYDOD_H_

#include "../Constraint.h"
#include "../sub/Power.h"

namespace horizon {
namespace constraints {

#define MAX_DOD 0.25

/**
 * A very simple power constraint. Basically says that the power constraint
 * @author Cory O'Connor
 */
class maxBatteryDOD : public Constraint
{
	/** The maximum depth of discharge allowed. */
	double maxDOD;

public:
	maxBatteryDOD();

	/** Destroys the DOD Constraint */
	~maxBatteryDOD(void);

	/**
	 * Returns whether the depth of discharge state in the power subsystem has gone above the
	 * specified value during the state.
	 * @param state the State that the Constraint is being applied to
	 * @return true if the DOD hasn't gone above the specified maximum depth of discharge
	 */
	virtual bool accepts(const systemSchedule* schedule) const;
};

}} // end namespace horizon::constraints

#endif // MAXBATTERYDOD_H_
#ifndef MAXDATABUFFER_H_
#define MAXDATABUFFER_H_

#include "../Constraint.h"
#include "../sub/Ssdr.h"

namespace horizon {
namespace constraints {

#define MAX_BUFF .700

	using horizon::util::Profile;

/**
 * A very simple power constraint. Basically says that the power constraint
 * @author Cory O'Connor
 */
class maxDataBuffer : public Constraint
{
	/** The maximum depth of discharge allowed. */
	double maxBUFF;

public:
	maxDataBuffer();

	/** Destroys the data buffer constraint */
	~maxDataBuffer(void);

	/**
	 * Returns whether the data buffer state in the ssdr subsystem has gone above the
	 * specified value during the state.
	 * @param state The State that the Constraint is being applied to.
	 * @return True if the data buffer hasn't gone above the specified maximum.
	 */
	virtual bool accepts(const systemSchedule* schedule) const;

	virtual Constraint* clone() const;
};

}} // end namespace horizon::constraints

#endif // MAXDATABUFFER_H_
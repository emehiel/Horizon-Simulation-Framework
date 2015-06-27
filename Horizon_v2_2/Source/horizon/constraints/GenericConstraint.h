#ifndef GENERICCONSTRAINT
#define GENERICCONSTRAINT

#include <string>
#include "../Constraint.h"

#define FAIL_IF_HIGHER "FAIL_IF_HIGHER"
#define FAIL_IF_LOWER "FAIL_IF_LOWER"

namespace horizon {
namespace constraints {
	using horizon::util::Profile;
	using horizon::util::StateVarKey;

/**
 * A generic constraint for intrinsic data types. Can be set to fail for values above or below
 * the specified limit
 * @author Brian Butler
 */
template <typename T> 
class GenericConstraint : public Constraint
{
	StateVarKey<T> key;
	T val;
	string type;

public:
	GenericConstraint(StateVarKey<T> key, T val, string type) : key(key), val(val), type(type) {}

	GenericConstraint(StateVarKey<T> key, T val) : key(key), val(val), type(FAIL_IF_HIGHER) {}

	/** Destroys the constraint */
	~GenericConstraint(void) {}

	/**
	 * Returns whether state satisfies this constraint
	 * @param schedule The schedule that the Constraint is being applied to.
	 * @return True if the state hasn't gone above/below the specified maximum/minimum.
	 */
	virtual bool accepts(const systemSchedule* schedule) const {
		vector<assetSchedule*>::const_iterator assetSchedIt = schedule->getAssetScheds().begin();
		for(; assetSchedIt != schedule->getAssetScheds().end(); assetSchedIt++) {
			if(!(*assetSchedIt)->empty()) {
				Profile<T> prof = (*assetSchedIt)->getLastState()->getProfile(key);
				if(type == FAIL_IF_HIGHER) {
					if(prof.pmax() > val)
						return false;
				}
				else if(type == FAIL_IF_LOWER) {
					if(prof.pmin() < val)
						return false;
				}
				else
					return false;
			}
		}
		return true;
	}
};

/**
 * A generic constraint for intrinsic data types. Can be set to fail for values above or below
 * the specified limit
 * @author Brian Butler
 */
template <typename T> 
class SingleAssetGenericConstraint : public Constraint
{
	StateVarKey<T> key;
	T val;
	string type;
	int assetNum;

public:
	SingleAssetGenericConstraint(StateVarKey<T> key, T val, string type, int assetNum) : key(key), val(val), type(type), assetNum(assetNum) {}

	SingleAssetGenericConstraint(StateVarKey<T> key, T val, int assetNum) : key(key), val(val), type(FAIL_IF_HIGHER), assetNum(assetNum) {}

	/** Destroys the constraint */
	~SingleAssetGenericConstraint(void) {}

	/**
	 * Returns whether state satisfies this constraint
	 * @param schedule The schedule that the Constraint is being applied to.
	 * @return True if the state hasn't gone above/below the specified maximum/minimum.
	 */
	virtual bool accepts(const systemSchedule* schedule) const {
		assetSchedule* aSched = schedule->getAssetScheds().at(assetNum - 1);
		if(!aSched->empty()) {
			Profile<T> prof = aSched->getLastState()->getProfile(key);
			if(type == FAIL_IF_HIGHER) {
				if(prof.pmax() > val)
					return false;
			}
			else if(type == FAIL_IF_LOWER) {
				if(prof.pmin() < val)
					return false;
			}
			else
				return false;
		}
		return true;
	}
};

}} // end namespace horizon::constraints

#endif // GENERICCONSTRAINT
/** \file GenericConstraint.h
* This file contains a generic Constraint.
*/

#ifndef GENERICCONSTRAINT
#define GENERICCONSTRAINT

#include <string>
#include "horizon/Constraint.h"

/**
* \def FAIL_IF_HIGHER
* This define should be used as the type parameter in the constructor if the
* Constraint should fail when the constrained variable rises above the specified value.
*/
#define FAIL_IF_HIGHER "FAIL_IF_HIGHER"

#define FAIL_IF_HIGHER_OR_EQUAL "FAIL_IF_HIGHER_OR_EQUAL"

/**
* \def FAIL_IF_LOWER
* This define should be used as the type parameter in the constructor if the
* Constraint should fail when the constrained variable falls below the specified value.
*/
#define FAIL_IF_LOWER "FAIL_IF_LOWER"

#define FAIL_IF_LOWER_OR_EQUAL "FAIL_IF_LOWER_OR_EQUAL"

/**
* \namespace horizon::constraints
* This namespace contains implementations of the abstract Constraint class.
*/
namespace horizon {
namespace constraints {
	using horizon::util::Profile;
	using horizon::util::StateVarKey;

/**
 * A generic Constraint class. This Constraint can be applied to state variables of
 * type int, double, or float and can be set to fail if the constrained variable rises
 * above or falls below a specified value. This constraint is applied to the State
 * corresponding to a single Asset.
 * @author Brian Butler
 */
template <typename T> 
class SingleAssetGenericConstraint : public Constraint
{
	/**
	* The StateVarKey that the Constraint is applied to.
	* The Constraint checks that the data stored in the State
	* using this StateVarKey is satisfied.
	*/
	StateVarKey<T> key;
	/**
	* The value of the Constraint.
	* Represents the maximum value of the constrained variable if the type of
	* the Constraint is FAIL_IF_HIGHER or the minimum value of the constrained
	* variable if the type of the Constraint is FAIL_IF_LOWER.
	*/
	T val;
	/**
	* The type of the Constraint.
	*/
	string type;
	/**
	* String containing the variable type constrained (int, float, or double).
	*/
	string varType;
	/**
	* The Asset number that this Constraint applies to.
	* The Constraint will only be applied to the State which corresponds
	* to the State specified by assetNum.
	*/
	int assetNum;

public:
	/**
	* A constructor for the Constraint.
	* This constructor is used if the type of the Constraint needs to be specified.
	* @param key The StateVarKey that corresponds to the constrained variable.
	* @param val The value of the Constraint.
	* @param type The type of the Constraint. This should be set to FAIL_IF_HIGHER if the constrained variable can't rise above val or FAIL_IF_LOWER if the constrained variable can't fall below val.
	* @param assetNum The Asset number that this Constraint applies to.
	*/
	SingleAssetGenericConstraint(StateVarKey<T> key, T val, string type, string varType, int assetNum) : key(key), val(val), type(type), varType(varType), assetNum(assetNum) {}

	/**
	* A constructor for the Constraint.
	* This constructor is used if the type doesn't need to be specified. The type defaults to FAIL_IF_HIGHER.
	* @param key The StateVarKey that corresponds to the constrained variable.
	* @param val The value of the Constraint.
	* @param assetNum The Asset number that this Constraint applies to.
	*/
	SingleAssetGenericConstraint(StateVarKey<T> key, T val, string varType, int assetNum) : key(key), val(val), type(FAIL_IF_HIGHER), varType(varType), assetNum(assetNum) {}

	SingleAssetGenericConstraint(const SingleAssetGenericConstraint& other) : key(other.key), val(other.val), type(other.type), varType(other.varType), assetNum(other.assetNum) {}	

	/** A destructor. Destroys the Constraint. */
	~SingleAssetGenericConstraint(void) {}

	/**
	 * Returns whether systemSchedule satisfies this Constraint
	 * @param schedule The systemSchedule that the Constraint is being applied to.
	 * @return true if the constrained variable hasn't gone above/below the specified maximum/minimum and false otherwise.
	 */
	virtual bool accepts(const systemSchedule* schedule) const {
		assetSchedule* aSched = schedule->getAssetScheds().at(assetNum - 1);
		if(!aSched->empty()) {
			Profile<T> prof = aSched->getLastState()->getProfile(key);
			if(type == FAIL_IF_HIGHER) {
				if(prof.pmax() > val) {
					Logger::ReportGenericConstraint(varType, type, val);
					return false;
				}
			}
			else if(type == FAIL_IF_HIGHER_OR_EQUAL) {
				if(prof.pmax() >= val) {
					Logger::ReportGenericConstraint(varType, type, val);
					return false;
				}
			}
			else if(type == FAIL_IF_LOWER) {
				if(prof.pmin() < val) {
					Logger::ReportGenericConstraint(varType, type, val);
					return false;
				}
			}
			else if(type == FAIL_IF_LOWER_OR_EQUAL) {
				if(prof.pmin() <= val) {
					Logger::ReportGenericConstraint(varType, type, val);
					return false;
				}
			}
			else
				return false;
		}
		return true;
	}

	virtual Constraint* clone() const {
		SingleAssetGenericConstraint* newConstraint = new SingleAssetGenericConstraint(*this);
		return (Constraint*) newConstraint;
	}
};

}} // end namespace horizon::constraints

#endif // GENERICCONSTRAINT
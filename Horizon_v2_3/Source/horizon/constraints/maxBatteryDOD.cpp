#include "maxBatteryDOD.h"

namespace horizon {
namespace constraints {

maxBatteryDOD::maxBatteryDOD() : maxDOD(MAX_DOD) {}

maxBatteryDOD::~maxBatteryDOD(void) {}

bool maxBatteryDOD::accepts(const systemSchedule* schedule) const
{
	vector<assetSchedule*>::const_iterator assetSchedIt = schedule->getAssetScheds().begin();
	for(; assetSchedIt != schedule->getAssetScheds().end(); assetSchedIt++) {
		if(!(*assetSchedIt)->empty()) {
			Profile<double> powerProfile = (*assetSchedIt)->getLastState()->getProfile(StateVarKey<double>(STATEVARNAME_DOD));
			if(!powerProfile.empty() && powerProfile.pmax() > maxDOD)
				return false;
		}
	}
	return true;
}

Constraint* maxBatteryDOD::clone() const
{
	maxBatteryDOD* newConstraint = new maxBatteryDOD(*this);
	return (Constraint*) newConstraint;
}

}}
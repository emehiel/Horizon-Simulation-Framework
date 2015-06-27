#include "maxDataBuffer.h"

namespace horizon {
namespace constraints {

maxDataBuffer::maxDataBuffer() : maxBUFF(MAX_BUFF) {}

maxDataBuffer::~maxDataBuffer(void) {}

bool maxDataBuffer::accepts(const systemSchedule* schedule) const
{
	vector<assetSchedule*>::const_iterator assetSchedIt = schedule->getAssetScheds().begin();
	for(; assetSchedIt != schedule->getAssetScheds().end(); assetSchedIt++) {
		if(!(*assetSchedIt)->empty()) {
			Profile<double> bufferProfile = (*assetSchedIt)->getLastState()->getProfile(StateVarKey<double>(STATEVARNAME_DATABUFFERRATIO));
			if(bufferProfile.pmax() > maxBUFF)
				return false;
		}
	}
	return true;
}

Constraint* maxDataBuffer::clone() const
{
	maxDataBuffer* newConstraint = new maxDataBuffer(*this);
	return (Constraint*) newConstraint;
}

}}
#include "TargetValueScheduleEvaluator.h"

namespace horizon {
namespace eval {

TargetValueScheduleEvaluator::TargetValueScheduleEvaluator() {}

TargetValueScheduleEvaluator::~TargetValueScheduleEvaluator() {}

double TargetValueScheduleEvaluator::evaluate(systemSchedule& schedule) const {
	double sum = 0;
	vector<assetSchedule*>::const_iterator assSchedIt = schedule.getAssetScheds().begin();
	for(; assSchedIt != schedule.getAssetScheds().end(); assSchedIt++) {
		for(vector<shared_ptr<Event>>::const_iterator eIt = (*assSchedIt)->getEvents().begin(); eIt != (*assSchedIt)->getEvents().end(); eIt++) {
			sum += 5 - (*eIt)->getTask()->getTarget()->getValue();
			if((*eIt)->getTask()->getType() == TASK_TYPE_COMM) {
				StateVarKey<double> s1(STATEVARNAME_DATABUFFERRATIO);
				double ts = (*eIt)->getState()->getTaskStart();
				double te = (*eIt)->getState()->getTaskEnd();
				double begindataratio = (*eIt)->getState()->getValueAtTime(s1, ts).second;
				double enddataratio = (*eIt)->getState()->getValueAtTime(s1, te).second;
				double deltadataratio = begindataratio - enddataratio;
				sum += deltadataratio*50;
			}

/*			vector<shared_ptr<Event>>::const_iterator eItStart = (*assSchedIt)->getEvents().begin();
			vector<shared_ptr<Event>>::const_iterator eItCurrent = eIt;
			double t = timeBetweenEventsWithSameTask(eItStart, eIt);
			if (t > (*eIt)->getTask()->getTarget()->getFreq_days()*86400.0){
				sum = 0;
				break;
			}
			if (t < 550){
				sum = 0;
				break;
			}*/
		}
	}
	return sum;
}

double TargetValueScheduleEvaluator::timeBetweenEventsWithSameTask(vector<shared_ptr<Event>>::const_iterator eItStart, vector<shared_ptr<Event>>::const_iterator eItCurrent) const {

	string targName = (*eItCurrent)->getTask()->getTarget()->getName();
	double targCollectTime = (*eItCurrent)->getState()->getTaskStart();

	while (eItCurrent != eItStart){
		eItCurrent--;

		if ((*eItCurrent)->getTask()->getTarget()->getName() == targName)
			return (targCollectTime - (*eItCurrent)->getState()->getTaskStart());
	}

	return targCollectTime;
}

}} // end namespace horizon::eval

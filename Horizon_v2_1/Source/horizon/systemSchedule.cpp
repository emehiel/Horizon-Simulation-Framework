#include "systemSchedule.h"

namespace horizon {

systemSchedule::systemSchedule(const vector<State*>& initialstates) : scheduleValue(0) {
	for(vector<State*>::const_iterator stIt = initialstates.begin(); stIt != initialstates.end(); stIt++) {
		assetscheds.push_back(new assetSchedule(*stIt));
	}
}

systemSchedule::systemSchedule(const systemSchedule* oldSchedule, const vector<const Task*>& newTaskList, double newTaskStartTime) : scheduleValue(0) {
	vector<const Task*>::const_iterator tIt = newTaskList.begin();
	for(vector<assetSchedule*>::const_iterator assSchedIt = oldSchedule->assetscheds.begin(); assSchedIt != oldSchedule->assetscheds.end(); assSchedIt++, tIt++) {
		if(*tIt != NULL) {
			shared_ptr<Event> eventToAdd(new Event(*tIt, new State((*assSchedIt)->getLastState(), newTaskStartTime)));
			assetscheds.push_back(new assetSchedule(*assSchedIt, eventToAdd));
		}
		else
			assetscheds.push_back(new assetSchedule(*assSchedIt));
	}
}

systemSchedule::~systemSchedule() {
	for(vector<assetSchedule*>::reverse_iterator asIt = assetscheds.rbegin(); asIt != assetscheds.rend(); asIt++)
		delete (*asIt);
	assetscheds.clear();
}

const bool systemSchedule::canAddTasks(const vector<const Task*>& newTaskList, const double newTaskStartTime) {
	size_t count = 0;
	vector<assetSchedule*>::iterator asIt2 = assetscheds.begin();
	for(vector<const Task*>::const_iterator tIt = newTaskList.begin(); tIt != newTaskList.end(); tIt++, asIt2++) {
		if(*tIt != NULL) {
			for(vector<assetSchedule*>::iterator asIt = assetscheds.begin(); asIt != assetscheds.end(); asIt++) {
				count += (*asIt)->timesCompletedTask(*tIt);
			}
			if(count >= (*tIt)->getMaxTimesPerformable())
				return false;
			if(!(*asIt2)->empty()) {
				if((*asIt2)->getLastState()->getEventEnd() > newTaskStartTime)
					return false;
			}
		}
	}
	return true;
}

const size_t systemSchedule::getTotalNumEvents() {
	size_t count = 0;
	for(vector<assetSchedule*>::iterator asIt = assetscheds.begin(); asIt != assetscheds.end(); asIt++)
		count += (*asIt)->size();
	return count;
}

State* systemSchedule::getSubNewState(size_t assetNum) {
	if(assetscheds[assetNum]->empty())
		return assetscheds[assetNum]->getInitialState();
	else
		return assetscheds[assetNum]->getLastState();
}

const Task* systemSchedule::getSubNewTask(size_t assetNum) {
	if(assetscheds[assetNum]->empty())
		return NULL;
	else
		return assetscheds[assetNum]->getLastTask();
}

const double systemSchedule::getLastTaskStart() const {
	double lasttime = 0;
	for(vector<assetSchedule*>::const_iterator aIt = assetscheds.begin(); aIt != assetscheds.end(); aIt++)
		if(!(*aIt)->empty())
			lasttime = lasttime > (*aIt)->getLastState()->getTaskStart() ? lasttime : (*aIt)->getLastState()->getTaskStart();
	return lasttime;
}

const vector<State*> systemSchedule::getEndStates() const {
	vector<State*> endStates;
	for(vector<assetSchedule*>::const_iterator assSchedIt = assetscheds.begin(); assSchedIt != assetscheds.end(); assSchedIt++)
		endStates.push_back((*assSchedIt)->getLastState());
	return endStates;
}

void systemSchedule::setScheduleValue(double value) {
	scheduleValue = value; }

bool schedGreater(systemSchedule* elem1, systemSchedule* elem2) {
   return elem1->getScheduleValue() > elem2->getScheduleValue(); }

} // end namespace horizon
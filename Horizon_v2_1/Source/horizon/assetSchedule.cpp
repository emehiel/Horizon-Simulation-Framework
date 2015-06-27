#include "assetSchedule.h"

namespace horizon {

assetSchedule::assetSchedule(State* initialState) : initialState(initialState) {}

assetSchedule::assetSchedule(assetSchedule* schedToMakeSafeCopyFrom) {
	if(!schedToMakeSafeCopyFrom->empty()) {
		// copy the initial state
		initialState = schedToMakeSafeCopyFrom->getInitialStateSharedPtr();
		size_t numE = schedToMakeSafeCopyFrom->size();
		vector<shared_ptr<Event>>::const_iterator eIt = schedToMakeSafeCopyFrom->getEvents().begin();
		//Cory was here!
		// for all except the last event push back shallow copies of the events
		for(size_t countChocula = 1; countChocula < numE; countChocula++, eIt++)
			events.push_back(*eIt);
		// make a deep copy of the last event in the schedule
		shared_ptr<Event> lastEventCopy(new Event(*eIt));
		events.push_back(shared_ptr<Event> (lastEventCopy));
	}
	else
		initialState = shared_ptr<State> (new State(schedToMakeSafeCopyFrom->getInitialState()));
}

assetSchedule::assetSchedule(const assetSchedule* oldSchedule, shared_ptr<Event> newEvent) : initialState(oldSchedule->initialState),
events(oldSchedule->events) {
	events.push_back(newEvent);
}

assetSchedule::~assetSchedule() {
	events.clear();
}

State* assetSchedule::getLastState() const {
	if(!empty()) return events.back()->getState();
	else return initialState.get();
}

const Task* assetSchedule::getLastTask() const {
	if(!empty()) return events.back()->getTask();
	else return 0;
}

const bool assetSchedule::empty() const {
	return size() == 0; }

size_t assetSchedule::size() const {
	return events.size(); }

const size_t assetSchedule::timesCompletedTask(const Task* newTask) {
	size_t count = 0;
	for(vector<shared_ptr<Event>>::iterator eIt = events.begin(); eIt != events.end(); eIt++) {
		if(((*eIt) != NULL) && (newTask != NULL)) {
			if((*eIt)->getTask() == newTask) count++;
		}
	}
	return count;
}
} // end namespace horizon

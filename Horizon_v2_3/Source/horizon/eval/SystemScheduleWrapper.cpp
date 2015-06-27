#include "SystemScheduleWrapper.h"

namespace horizon {
	namespace eval {

SystemScheduleWrapper::SystemScheduleWrapper(systemSchedule* schedule) : schedule(schedule) {}

SystemScheduleWrapper::~SystemScheduleWrapper() {}

int SystemScheduleWrapper::GetNumberAssets() {
	return schedule->getAssetScheds().size();
}

int SystemScheduleWrapper::GetNumberEvents(int assetNum) {
	const vector<assetSchedule*>& assetScheds = schedule->getAssetScheds();
	vector<assetSchedule*>::const_iterator asIt = assetScheds.begin();
	int i = assetNum;
	while(i > 1) asIt++;
	const vector<shared_ptr<Event>>& events = (*asIt)->getEvents();
	return events.size();
}

Event* SystemScheduleWrapper::GetEvent(int assetNum, int eventNum) {
	const vector<assetSchedule*>& assetScheds = schedule->getAssetScheds();
	vector<assetSchedule*>::const_iterator asIt = assetScheds.begin();
	int i = assetNum;
	while(i > 1) asIt++;
	const vector<shared_ptr<Event>>& events = (*asIt)->getEvents();
	return events.at(eventNum-1).get();
}



}} // horizon::eval
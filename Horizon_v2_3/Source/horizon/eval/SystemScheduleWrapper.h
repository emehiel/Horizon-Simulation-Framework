#ifndef SYSTEM_SCHEDULE_WRAPPER
#define SYSTEM_SCHEDULE_WRAPPER

#include "horizon/systemSchedule.h"

using namespace horizon;


namespace horizon {
class systemSchedule; // forward
namespace eval {


class SystemScheduleWrapper {
private:
	systemSchedule* schedule;

public:
	SystemScheduleWrapper(systemSchedule* schedule);
	~SystemScheduleWrapper();

	int GetNumberAssets();

	int GetNumberEvents(int assetNum);

	Event* GetEvent(int assetNum, int eventNum);
};

}} // horizon::eval

#endif
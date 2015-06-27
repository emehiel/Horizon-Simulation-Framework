#include "register2.h"

void register_SystemScheduleWrapper(lua_State* L) {
	module(L) [
		class_<SystemScheduleWrapper> ("SystemScheduleWrapper")
			.def(constructor<systemSchedule*> ())
			.def("GetNumberAssets", &SystemScheduleWrapper::GetNumberAssets)
			.def("GetNumberEvents", &SystemScheduleWrapper::GetNumberEvents)
			.def("GetEvent", &SystemScheduleWrapper::GetEvent)
	];
}
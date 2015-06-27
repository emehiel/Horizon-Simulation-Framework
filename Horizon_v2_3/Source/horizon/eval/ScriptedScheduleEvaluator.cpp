#include "ScriptedScheduleEvaluator.h"

namespace horizon {
namespace eval {

ScriptedScheduleEvaluator::ScriptedScheduleEvaluator(lua_State* L, string evaluateLUAFcn) : L(L), evaluateLUAFcn(evaluateLUAFcn){}

ScriptedScheduleEvaluator::~ScriptedScheduleEvaluator() {}

double ScriptedScheduleEvaluator::evaluate(systemSchedule& schedule) const {
	double sum = 0;
	// Loop through each Event within each assetSchedule in the systemSchedule
	vector<assetSchedule*>::const_iterator assSchedIt = schedule.getAssetScheds().begin();
	for(; assSchedIt != schedule.getAssetScheds().end(); assSchedIt++) {
		for(vector<shared_ptr<Event>>::const_iterator eIt = (*assSchedIt)->getEvents().begin(); eIt != (*assSchedIt)->getEvents().end(); eIt++) {
			Event* evnt = (*eIt).get();	
			// Evaluate the schedule
			// The evaluation function must have the following signature:
			// double evaluate(Event& event);
			double eventValue = 0;
			// Call the function, and catch any error message
			try {
				if(functionExists(L, evaluateLUAFcn))
					eventValue = call_function<double>(L, evaluateLUAFcn.c_str(), evnt);
				else {
					cout << "Schedule evaluation function '" << evaluateLUAFcn;
					cout << "' does not exist!" << endl;
				}
			}
			catch(error& e) {
				object error_msg(from_stack(e.state(), -1));
				cout << "Error calling schedule evaluation function '" << evaluateLUAFcn << "'!" << endl;
				cout << error_msg << endl;
			}
			// Add the event value to the running total for the entire schedule
			sum += eventValue;
		}
	}
	return sum;
}

}} // end namespace horizon::eval
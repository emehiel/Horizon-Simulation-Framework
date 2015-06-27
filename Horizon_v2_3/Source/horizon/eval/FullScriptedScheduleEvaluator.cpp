#include "FullScriptedScheduleEvaluator.h"

namespace horizon {
namespace eval {

FullScriptedScheduleEvaluator::FullScriptedScheduleEvaluator(lua_State* L, string evaluateLUAFcn) : L(L), evaluateLUAFcn(evaluateLUAFcn){}

FullScriptedScheduleEvaluator::~FullScriptedScheduleEvaluator() {}

double FullScriptedScheduleEvaluator::evaluate(systemSchedule& schedule) const {
	SystemScheduleWrapper wrapper(&schedule);
	double val = 0.0;
	// Call the function, and catch any error message
	try {
		if(functionExists(L, evaluateLUAFcn))
			val = call_function<double>(L, evaluateLUAFcn.c_str(), wrapper);
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
	return val;
}

}} // end namespace horizon::eval
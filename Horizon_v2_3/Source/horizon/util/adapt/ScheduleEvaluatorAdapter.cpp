#include "ScheduleEvaluatorAdapter.h"

namespace horizon {
	namespace util {
		namespace adapt {

ScheduleEvaluator* createScheduleEvaluator(lua_State* L, XMLNode& SchedEvalNode) {
//	lua_State* L = horizon::script::lua::getNewLuaState();
	cout << "Setting up ScheduleEvaluator..." << endl;
	// First check if the type attribute is set or an empty XMLNode
	// If not set or empty then default to TargetValueScheduleEvaluator
	if(SchedEvalNode.isEmpty() || !SchedEvalNode.isAttributeSet("type")) {
		cout << "<SCHEDULE_EVALUATOR> node missing or 'type' attribute is not set;";
		cout << " defaulting to TargetValueScheduleEvaluator." << endl;
		cout << "Schedule Evaluator set to 'TargetValueScheduleEvaluator'" << endl;
		return new TargetValueScheduleEvaluator();
	}
	// Get the type of the ScheduleEvaluator
	string type = SchedEvalNode.getAttribute("type");
	if(_strcmpi(type.c_str(), "scripted") == 0) {
		// For a scripted ScheduleEvaluator, the attribute 'evaluateLUAFcn'
		// needs to be set. If it isn't set print error message and resort
		// to a default of TargetValueScheduleEvaluator
		if(SchedEvalNode.isAttributeSet("evaluateLUAFcn")) {
			string fcn = SchedEvalNode.getAttribute("evaluateLUAFcn");
			cout << "Schedule Evaluator set to 'ScriptedScheduleEvaluator'";
			cout << " using LUA function '" << fcn << "'" << endl;
			return new ScriptedScheduleEvaluator(L, fcn);
		}
		else {
			// Type attribute is missing, default to TargetValueScheduleEvaluator
			cout << "'evaluateLUAFcn' attribute missing from <SCHEDULE_EVALUATOR> node;";
			cout << " defaulting to TargetValueScheduleEvaluator." << endl;
			cout << "Schedule Evaluator set to 'TargetValueScheduleEvaluator'" << endl;
			return new TargetValueScheduleEvaluator();
		}		
	}
	/*else if(_strcmpi(type.c_str(), "scripted2") == 0) {
		// For a scripted ScheduleEvaluator, the attribute 'evaluateLUAFcn'
		// needs to be set. If it isn't set print error message and resort
		// to a default of TargetValueScheduleEvaluator
		if(SchedEvalNode.isAttributeSet("evaluateLUAFcn")) {
			string fcn = SchedEvalNode.getAttribute("evaluateLUAFcn");
			cout << "Schedule Evaluator set to 'FullScriptedScheduleEvaluator'";
			cout << " using LUA function '" << fcn << "'" << endl;
			return new FullScriptedScheduleEvaluator(L, fcn);
		}
		else {
			// Type attribute is missing, default to TargetValueScheduleEvaluator
			cout << "'evaluateLUAFcn' attribute missing from <SCHEDULE_EVALUATOR> node;";
			cout << " defaulting to TargetValueScheduleEvaluator." << endl;
			cout << "Schedule Evaluator set to 'TargetValueScheduleEvaluator'" << endl;
			return new TargetValueScheduleEvaluator();
		}		
	}*/
	else if(_strcmpi(type.c_str(), "TargetValueScheduleEvaluator") == 0) {
			cout << "Schedule Evaluator set to 'TargetValueScheduleEvaluator'" << endl;
			return new TargetValueScheduleEvaluator();
	}
	else if(_strcmpi(type.c_str(), "QuantitativeScheduleEvaluator") == 0) {
			cout << "Schedule Evaluator set to 'QuantitativeScheduleEvaluator'" << endl;
			return new QuantitativeScheduleEvaluator();
	}
	else {
		// No match for type attribute, default to TargetValueScheduleEvaluator
		cout << "No matches for 'type' attribute in <SCHEDULE_EVALUATOR> node;";
		cout << " defaulting to TargetValueScheduleEvaluator." << endl;
		cout << "Schedule Evaluator set to 'TargetValueScheduleEvaluator'" << endl;
		return new TargetValueScheduleEvaluator();
	}
}

}}} // horizon::util::adapt
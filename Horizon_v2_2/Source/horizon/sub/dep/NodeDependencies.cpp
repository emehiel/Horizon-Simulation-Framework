#include "NodeDependencies.h"

namespace horizon {
	namespace sub {
		namespace dep {

NodeDependencies::NodeDependencies() { depInstance = Dependencies::Instance(); };

NodeDependencies::~NodeDependencies() {};

Profile<int>	NodeDependencies::callIntDependency(string callKey) {
	if(scriptingEnabled) {
		if( intDependencies.find(callKey) == intDependencies.end()) {
			string key = intDependencies_scripted[callKey];
			Profile<int> prof = call_function< Profile<int> >(L, key.c_str());
			return prof;
		}
		else
			return (depInstance->*(intDependencies[callKey]))();
	}
	else
		return (depInstance->*(intDependencies[callKey]))();
}
Profile<double> NodeDependencies::callDoubleDependency(string callKey) {
	if(scriptingEnabled) {
		if( doubleDependencies.find(callKey) == doubleDependencies.end()) {
			string key = doubleDependencies_scripted[callKey];
			Profile<double> prof = call_function< Profile<double> >(L, key.c_str());
			return prof;
		}
		else
			return (depInstance->*(doubleDependencies[callKey]))();
	}
	else
		return (depInstance->*(doubleDependencies[callKey]))();
}
Profile<float>	NodeDependencies::callFloatDependency(string callKey) {
	if(scriptingEnabled) {
		if( floatDependencies.find(callKey) == floatDependencies.end()) {
			string key = floatDependencies_scripted[callKey];
			Profile<float> prof = call_function< Profile<float> >(L, key.c_str());
			return prof;
		}
		else
			return (depInstance->*(floatDependencies[callKey]))();
	}
	else
		return (depInstance->*(floatDependencies[callKey]))();
}
Profile<bool>	NodeDependencies::callBoolDependency(string callKey) {
	if(scriptingEnabled) {
		if( boolDependencies.find(callKey) == boolDependencies.end()) {
			string key = boolDependencies_scripted[callKey];
			Profile<bool> prof = call_function< Profile<bool> >(L, key.c_str());
			return prof;
		}
		else
			return (depInstance->*(boolDependencies[callKey]))();
	}
	else
		return (depInstance->*(boolDependencies[callKey]))();
}
Profile<Matrix> NodeDependencies::callMatrixDependency(string callKey) {
	if(scriptingEnabled) {
		if( matrixDependencies.find(callKey) == matrixDependencies.end()) {
			string key = matrixDependencies_scripted[callKey];
			Profile<Matrix> prof = call_function< Profile<Matrix> >(L, key.c_str());
			return prof;
		}
		else
			return (depInstance->*(matrixDependencies[callKey]))();
	}
	else
		return (depInstance->*(matrixDependencies[callKey]))();
}
Profile<Quat>	NodeDependencies::callQuatDependency(string callKey) {
	if(scriptingEnabled) {
		if( quatDependencies.find(callKey) == quatDependencies.end()) {
			string key = quatDependencies_scripted[callKey];
			Profile<Quat> prof = call_function< Profile<Quat> >(L, key.c_str());
			return prof;
		}
		else
			return (depInstance->*(quatDependencies[callKey]))();
	}
	else
		return (depInstance->*(quatDependencies[callKey]))();
}

void NodeDependencies::addDependency(string callKey, IntDependency dep) {
	intDependencies[callKey] = dep;
}
void NodeDependencies::addDependency(string callKey, DoubleDependency dep) {
	doubleDependencies[callKey] = dep;
}
void NodeDependencies::addDependency(string callKey, FloatDependency dep) {
	floatDependencies[callKey] = dep;
}
void NodeDependencies::addDependency(string callKey, BoolDependency dep) {
	boolDependencies[callKey] = dep;
}
void NodeDependencies::addDependency(string callKey, MatrixDependency dep) {
	matrixDependencies[callKey] = dep;
}
void NodeDependencies::addDependency(string callKey, QuatDependency dep) {
	quatDependencies[callKey] = dep;
}

void NodeDependencies::addIntScriptedDependency(string callKey, string key) {
	intDependencies_scripted[callKey] = key;
}
void NodeDependencies::addDoubleScriptedDependency(string callKey, string key) {
	doubleDependencies_scripted[callKey] = key;
}
void NodeDependencies::addFloatScriptedDependency(string callKey, string key) {
	floatDependencies_scripted[callKey] = key;
}
void NodeDependencies::addBoolScriptedDependency(string callKey, string key) {
	boolDependencies_scripted[callKey] = key;
}
void NodeDependencies::addMatrixScriptedDependency(string callKey, string key) {
	matrixDependencies_scripted[callKey] = key;
}
void NodeDependencies::addQuatScriptedDependency(string callKey, string key) {
	quatDependencies_scripted[callKey] = key;
}


}}} // end namespace horizon::sub::dep
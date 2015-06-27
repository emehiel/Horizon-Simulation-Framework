#include "NodeDependencies.h"

namespace horizon {
	namespace sub {
		namespace dep {

NodeDependencies::NodeDependencies() { depInstance = Dependencies::Instance(); };

NodeDependencies::~NodeDependencies() {};

Profile<int>	NodeDependencies::callIntDependency(string key) {
	return (depInstance->*(intDependencies[key]))();
}
Profile<double> NodeDependencies::callDoubleDependency(string key) {
	return (depInstance->*(doubleDependencies[key]))();
}
Profile<float>	NodeDependencies::callFloatDependency(string key) {
	return (depInstance->*(floatDependencies[key]))();
}
Profile<bool>	NodeDependencies::callBoolDependency(string key) {
	return (depInstance->*(boolDependencies[key]))();
}
Profile<Matrix> NodeDependencies::callMatrixDependency(string key) {
	return (depInstance->*(matrixDependencies[key]))();
}
Profile<Quat>	NodeDependencies::callQuatDependency(string key) {
	return (depInstance->*(quatDependencies[key]))();
}

void NodeDependencies::addDependcy(string key, IntDependency dep) {
	intDependencies[key] = dep;
}
void NodeDependencies::addDependcy(string key, DoubleDependency dep) {
	doubleDependencies[key] = dep;
}
void NodeDependencies::addDependcy(string key, FloatDependency dep) {
	floatDependencies[key] = dep;
}
void NodeDependencies::addDependcy(string key, BoolDependency dep) {
	boolDependencies[key] = dep;
}
void NodeDependencies::addDependcy(string key, MatrixDependency dep) {
	matrixDependencies[key] = dep;
}
void NodeDependencies::addDependcy(string key, QuatDependency dep) {
	quatDependencies[key] = dep;
}

}}} // end namespace horizon::sub::dep
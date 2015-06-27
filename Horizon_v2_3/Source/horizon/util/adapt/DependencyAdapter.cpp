#include "DependencyAdapter.h"

	using namespace horizon::sub;
	using namespace horizon::sub::dep;

namespace horizon {
namespace util {
namespace adapt {

DependencyAdapter::DependencyAdapter() {}

void DependencyAdapter::addIntDependency(string key, IntDependency dep) {
	intDependencies.insert(make_pair(key, dep));
}
void DependencyAdapter::addDoubleDependency(string key, DoubleDependency dep) {
	doubleDependencies.insert(make_pair(key, dep));
}
void DependencyAdapter::addFloatDependency(string key, FloatDependency dep) {
	floatDependencies.insert(make_pair(key, dep));
}
void DependencyAdapter::addBoolDependency(string key, BoolDependency dep) {
	boolDependencies.insert(make_pair(key, dep));
}
void DependencyAdapter::addMatrixDependency(string key, MatrixDependency dep) {
	matrixDependencies.insert(make_pair(key, dep));
}
void DependencyAdapter::addQuatDependency(string key, QuatDependency dep) {
	quatDependencies.insert(make_pair(key, dep));
}

IntDependency DependencyAdapter::getIntDependency(string key) {
	return intDependencies.find(key)->second;
}
DoubleDependency DependencyAdapter::getDoubleDependency(string key) {
	return doubleDependencies.find(key)->second;
}
FloatDependency DependencyAdapter::getFloatDependency(string key) {
	return floatDependencies.find(key)->second;
}
BoolDependency DependencyAdapter::getBoolDependency(string key) {
	return boolDependencies.find(key)->second;
}
MatrixDependency DependencyAdapter::getMatrixDependency(string key) {
	return matrixDependencies.find(key)->second;
}
QuatDependency DependencyAdapter::getQuatDependency(string key) {
	return quatDependencies.find(key)->second;
}

}}}
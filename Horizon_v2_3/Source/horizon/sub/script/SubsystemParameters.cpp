#include "SubsystemParameters.h"

namespace horizon {
	namespace sub {
		namespace script {

SubsystemParameters::SubsystemParameters() {}

SubsystemParameters::~SubsystemParameters() {}

double SubsystemParameters::getDoubleParameter(string key) {
	// Find the value in the map corresponding to the specified key
	DoubleParamMap::const_iterator valueIt = dParams.find(key);
	// If the key exists in the map, return the corresponding value, otherwise
	// return a default constructed parameter type
	if(valueIt != dParams.end())
		return valueIt->second;
	else
		return double();
}

float SubsystemParameters::getFloatParameter(string key) {
	// Find the value in the map corresponding to the specified key
	FloatParamMap::const_iterator valueIt = fParams.find(key);
	// If the key exists in the map, return the corresponding value, otherwise
	// return a default constructed parameter type
	if(valueIt != fParams.end())
		return valueIt->second;
	else
		return float();
}

int SubsystemParameters::getIntParameter(string key) {
	// Find the value in the map corresponding to the specified key
	IntParamMap::const_iterator valueIt = iParams.find(key);
	// If the key exists in the map, return the corresponding value, otherwise
	// return a default constructed parameter type
	if(valueIt != iParams.end())
		return valueIt->second;
	else
		return int();
}

bool SubsystemParameters::getBoolParameter(string key) {
	// Find the value in the map corresponding to the specified key
	BoolParamMap::const_iterator valueIt = bParams.find(key);
	// If the key exists in the map, return the corresponding value, otherwise
	// return a default constructed parameter type
	if(valueIt != bParams.end())
		return valueIt->second;
	else
		return bool();
}

Matrix SubsystemParameters::getMatrixParameter(string key) {
	// Find the value in the map corresponding to the specified key
	MatrixParamMap::const_iterator valueIt = mParams.find(key);
	// If the key exists in the map, return the corresponding value, otherwise
	// return a default constructed parameter type
	if(valueIt != mParams.end())
		return valueIt->second;
	else
		return Matrix();
}

Quat SubsystemParameters::getQuatParameter(string key) {
	// Find the value in the map corresponding to the specified key
	QuatParamMap::const_iterator valueIt = qParams.find(key);
	// If the key exists in the map, return the corresponding value, otherwise
	// return a default constructed parameter type
	if(valueIt != qParams.end())
		return valueIt->second;
	else
		return Quat();
}

}}} // horizon::sub::Script
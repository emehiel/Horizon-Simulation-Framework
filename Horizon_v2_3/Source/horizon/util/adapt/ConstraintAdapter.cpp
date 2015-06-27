#include "ConstraintAdapter.h"

namespace horizon {
namespace util {
namespace adapt {

typedef Constraint* (ConstraintAdapter::*fcnPtr)();


ConstraintAdapter::ConstraintAdapter() {}

bool ConstraintAdapter::initialize() {
	bool initialized = true;
	// Get the function pointer
	fcnPtr ptr = &ConstraintAdapter::create_maxBatteryDOD;
	// Insert function pointers into maps
	fcnMap.insert(make_pair(string("maxBatteryDOD"), ptr));
	ptr = &ConstraintAdapter::create_maxDataBuffer;
	fcnMap.insert(make_pair(string("maxDataBuffer"), ptr));
	return initialized;
}

bool ConstraintAdapter::finalize() {
	fcnMap.clear();
	return true;
}

Constraint* ConstraintAdapter::create(string key) {
	Constraint* sub = NULL;
	map<string, fcnPtr>::const_iterator pos = fcnMap.find(key);
	if(pos != fcnMap.end()) {
		fcnPtr ptr = fcnMap.find(key)->second;
		if(ptr != NULL)
			sub = (this->*ptr)();
	}
	return sub;
}

Constraint* ConstraintAdapter::create_maxBatteryDOD() { return new maxBatteryDOD(); }
Constraint* ConstraintAdapter::create_maxDataBuffer() { return new maxDataBuffer(); }

}}}

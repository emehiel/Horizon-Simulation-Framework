#include "SubsystemAdapter.h"

namespace horizon {
namespace util {
namespace adapt {

	typedef Subsystem* (SubsystemAdapter::*fcnPtr)();
	typedef Subsystem* (SubsystemAdapter::*xmlFcnPtr)(XMLNode&);
	
using namespace horizon::sub;

SubsystemAdapter::SubsystemAdapter() {}

bool SubsystemAdapter::initialize() {
	bool initialized = true;

	fcnPtr ptr = &SubsystemAdapter::create_Access;
	xmlFcnPtr ptr2 = &SubsystemAdapter::create_Access;
	fcnMap.insert(make_pair(string("Access"), ptr));
	xmlFcnMap.insert(make_pair(string("Access"), ptr2));
	ptr = &SubsystemAdapter::create_Adcs;
	ptr2 = &SubsystemAdapter::create_Adcs;
	fcnMap.insert(make_pair(string("Adcs"), ptr));
	xmlFcnMap.insert(make_pair(string("Adcs"), ptr2));
	ptr = &SubsystemAdapter::create_Comm;
	ptr2 = &SubsystemAdapter::create_Comm;
	fcnMap.insert(make_pair(string("Comm"), ptr));
	xmlFcnMap.insert(make_pair(string("Comm"), ptr2));
	ptr = &SubsystemAdapter::create_EOSensor;
	ptr2 = &SubsystemAdapter::create_EOSensor;
	fcnMap.insert(make_pair(string("EOSensor"), ptr));
	xmlFcnMap.insert(make_pair(string("EOSensor"), ptr2));
	ptr = &SubsystemAdapter::create_Power;
	ptr2 = &SubsystemAdapter::create_Power;
	fcnMap.insert(make_pair(string("Power"), ptr));
	xmlFcnMap.insert(make_pair(string("Power"), ptr2));
	ptr = &SubsystemAdapter::create_Ssdr;
	ptr2 = &SubsystemAdapter::create_Ssdr;
	fcnMap.insert(make_pair(string("Ssdr"), ptr));
	xmlFcnMap.insert(make_pair(string("Ssdr"), ptr2));

	return initialized;
}

bool SubsystemAdapter::finalize() {
	fcnMap.clear();
	xmlFcnMap.clear();
	return true;
}

Subsystem* SubsystemAdapter::create(string key) {
	Subsystem* sub = NULL;
	fcnPtr ptr = fcnMap.find(key)->second;
	if(ptr != NULL)
		sub = (this->*ptr)();
	return sub;
}

Subsystem* SubsystemAdapter::create(string key, XMLNode& subNode) {
	Subsystem* sub = NULL;
	xmlFcnPtr ptr = xmlFcnMap.find(key)->second;
	if(ptr != NULL)
		sub = (this->*ptr)(subNode);
	return sub;
}

Subsystem* SubsystemAdapter::create_Access() { return new Access(); }
Subsystem* SubsystemAdapter::create_Access(XMLNode& subNode) { return new Access(subNode); }
Subsystem* SubsystemAdapter::create_Adcs() { return new Adcs(); }
Subsystem* SubsystemAdapter::create_Adcs(XMLNode& subNode) { return new Adcs(subNode); }
Subsystem* SubsystemAdapter::create_Comm() { return new Comm(); }
Subsystem* SubsystemAdapter::create_Comm(XMLNode& subNode) { return new Comm(subNode); }
Subsystem* SubsystemAdapter::create_EOSensor() { return new EOSensor(); }
Subsystem* SubsystemAdapter::create_EOSensor(XMLNode& subNode) { return new EOSensor(subNode); }
Subsystem* SubsystemAdapter::create_Power() { return new Power(); }
Subsystem* SubsystemAdapter::create_Power(XMLNode& subNode) { return new Power(subNode); }
Subsystem* SubsystemAdapter::create_Ssdr() { return new Ssdr(); }
Subsystem* SubsystemAdapter::create_Ssdr(XMLNode& subNode) { return new Ssdr(subNode); }


}}}
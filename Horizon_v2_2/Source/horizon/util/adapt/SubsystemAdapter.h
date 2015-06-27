#ifndef SUBSYSTEMADAPTER_H
#define SUBSYSTEMADAPTER_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "horizon/Subsystem.h"
#include "horizon/io/xmlParser.h"

#include "horizon/sub/Access.h"
#include "horizon/sub/Adcs.h"
#include "horizon/sub/Comm.h"
#include "horizon/sub/EOSensor.h"
#include "horizon/sub/Power.h"
#include "horizon/sub/Ssdr.h"

using namespace std;

namespace horizon {
namespace util {
namespace adapt {

class SubsystemAdapter {
	typedef Subsystem* (SubsystemAdapter::*fcnPtr)();
	typedef Subsystem* (SubsystemAdapter::*xmlFcnPtr)(XMLNode&);

private:
	map<string, fcnPtr> fcnMap;
	map<string, xmlFcnPtr> xmlFcnMap;

public:
	SubsystemAdapter();

	bool initialize();

	bool finalize();

	Subsystem* create(string key);
	Subsystem* create(string key, XMLNode& subNode);

private:
	Subsystem* create_Access();
	Subsystem* create_Access(XMLNode& subNode);
	Subsystem* create_Adcs();
	Subsystem* create_Adcs(XMLNode& subNode);
	Subsystem* create_Comm();
	Subsystem* create_Comm(XMLNode& subNode);
	Subsystem* create_EOSensor();
	Subsystem* create_EOSensor(XMLNode& subNode);
	Subsystem* create_Power();
	Subsystem* create_Power(XMLNode& subNode);
	Subsystem* create_Ssdr();
	Subsystem* create_Ssdr(XMLNode& subNode);
	
};

}}}

#endif
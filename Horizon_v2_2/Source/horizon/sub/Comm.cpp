#include "Comm.h"
#include "dep/Dependencies.h"

namespace horizon {
namespace sub {

	using namespace horizon::sub::dep;

Comm::Comm() : Subsystem(SUBNAME_COMM){
	addKey(DATARATE_KEY);
}

Comm::Comm(XMLNode& commNode) : Subsystem(SUBNAME_COMM){
	addKey(DATARATE_KEY);
}

Comm::~Comm() {}

bool Comm::canPerform(const State* oldState, State* newState, const Task* task, Position* position, Environment* environment, NodeDependencies* deps) const
{	
	if(task->getType() == TASK_TYPE_COMM) {
		Profile<double> newProf = deps->callDoubleDependency("COMMSUB_getDataRateProfile");
		if(!newProf.empty())
			newState->setProfile(DATARATE_KEY, newProf);
	}
	return true;
}

bool Comm::canExtend(State* newState, Position* position, Environment* environment, const double evalToTime, NodeDependencies* deps) const
{
	if (newState->getEventEnd() < evalToTime)
		newState->setEventEnd(evalToTime);
	return true;
}

}} // end namespace horizon::sub

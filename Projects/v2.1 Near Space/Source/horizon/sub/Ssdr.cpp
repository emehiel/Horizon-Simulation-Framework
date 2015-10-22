#include "Ssdr.h"
#include "dep/Dependencies.h"

namespace horizon {
namespace sub {

	using namespace horizon::sub::dep;

Ssdr::Ssdr() : Subsystem(SUBNAME_SSDR)
{
	buffersize = 5*1000;
	addKey(DATABUFFERRATIO_KEY);
}

Ssdr::Ssdr(XMLNode ssdrNode) : Subsystem(ssdrNode.getAttribute("subsystemName")), 
	buffersize(atoi(ssdrNode.getAttribute("bufferSize"))),
	subNode(ssdrNode)
{
	addKey(DATABUFFERRATIO_KEY);
}

Ssdr::~Ssdr() {}

bool Ssdr::canPerform(const State* oldState, State* newState, const Task* task, Position* position, Environment* environment, NodeDependencies* deps) const
{
	if(task->getType() == TASK_TYPE_IMAGE)
	{
		double ts = newState->getTaskStart();
		double te = newState->getTaskEnd();
		double oldbufferratio = oldState->getLastValue(dkeys.front()).second;
		Profile<double> newdataratein = (deps->callDoubleDependency("SSDRSUB_getNewDataProfile")/ buffersize);

		bool exceeded;
		Profile<double> newdataratio = newdataratein.upperLimitIntegrateToProf(ts, te, 5, 1, exceeded, 0, oldbufferratio);
		if(!exceeded) {
			newState->addValue(DATABUFFERRATIO_KEY, newdataratio);
			return true;
		}
		return false;
	}
	else if(task->getType() == TASK_TYPE_COMM)
	{
		double ts = newState->getTaskStart();
		newState->setTaskEnd(ts + 60.0);
		double te = newState->getTaskEnd();

		double data = buffersize*oldState->getLastValue(dkeys.front()).second;
		double dataqueout = data/2 > 50 ? data/2 : data;

		if(data-dataqueout < 0)
			dataqueout = data;

		if(dataqueout > 0)
			newState->addValue(DATABUFFERRATIO_KEY, make_pair(te, (data-dataqueout)/buffersize));
		return true;
	}
	return true;
}

bool Ssdr::canExtend(State* newState, Position* position, Environment* environment, const double evalToTime, NodeDependencies* deps) const
{
	if (newState->getEventEnd() < evalToTime)
		newState->setEventEnd(evalToTime);
	return true;
}

}} // end namespace horizon::sub
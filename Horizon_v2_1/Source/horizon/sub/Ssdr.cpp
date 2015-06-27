#include "Ssdr.h"
#include "dep/Dependencies.h"

namespace horizon {
namespace sub {

	using namespace horizon::sub::dep;

Ssdr::Ssdr() : Subsystem(SUBNAME_SSDR)
{
	boardSize = 2000000000;
	numBoards = 8;
	dnlkRate = 1000000.0;
	addKey(BITSONBUFFER_KEY);
}

Ssdr::Ssdr(XMLNode ssdrNode) : Subsystem(SUBNAME_SSDR), 
	
	subNode(ssdrNode)
{
	addKey(BITSONBUFFER_KEY);
	boardSize = atoi(ssdrNode.getAttribute("boardSize"));
	numBoards = atoi(ssdrNode.getAttribute("boards"));
	bufferSize = (double)(boardSize*numBoards);
	dnlkRate = atof(ssdrNode.getAttribute("downLinkRate"));

}

Ssdr::~Ssdr() {}

bool Ssdr::canPerform(const State* oldState, State* newState, const Task* task, Position* position, Environment* environment, NodeDependencies* deps) const
{
	if(task->getType() == TASK_TYPE_IMAGE)
	{
		double ts = newState->getTaskStart();
		double te = newState->getTaskEnd();
		double oldBufferBits = oldState->getLastValue(dkeys.front()).second;
		Profile<double> bitsToAddProf = (deps->callDoubleDependency("SSDRSUB_getNewDataProfile"));
		double bitsToAdd = bitsToAddProf.getLastValue();
		//if (bitsToAdd+oldBufferBits > bufferSize)
		//	return false;

		newState->addValue(BITSONBUFFER_KEY, make_pair(te, bitsToAdd+oldBufferBits));

		return true;
	}
	else if(task->getType() == TASK_TYPE_COMM)
	{
		double ts = newState->getTaskStart();
		newState->setTaskEnd(ts + 60.0);
		double te = newState->getTaskEnd();

		double bitsOnBuffer = oldState->getLastValue(dkeys.front()).second;
		if (bitsOnBuffer < 0.0001)
			return false;

		double transTime = bitsOnBuffer/dnlkRate;
		double dataOut = 0.0;

		if(transTime > 60.0)
			dataOut = 60.0*dnlkRate;
		else
			dataOut = bitsOnBuffer;

		newState->addValue(BITSONBUFFER_KEY, make_pair(te, bitsOnBuffer - dataOut));
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
#include "Access.h"
#include "../util/Matrix.h"
#include "../io/xmlParser.h" 

namespace horizon {
namespace sub {

Access::Access() : Subsystem(SUBNAME_ACCESS) {

}

Access::Access(XMLNode accessNode) : Subsystem(SUBNAME_ACCESS) {

}

Access::~Access() {}

bool Access::canPerform(const State* oldState, State* newState, const Task* task, Position* position, Environment* environment, NodeDependencies* deps) const
{

// Check illumination at Target
	//shadow_state shadow = environment->getSun()->castShadowOnPos(task->getTarget()->getPos(), newState->getTaskStart());
	
	//return (shadow == NO_SHADOW);
	return true;
}

bool Access::canExtend(State* state, Position* position, Environment* environment, const double evalToTime, NodeDependencies* deps) const
{
	if (state->getEventEnd() < evalToTime)
		state->setEventEnd(evalToTime);
	return true;
}

}} // end namespace horizon::sub
#include "Adcs.h"
#include "dep/Dependencies.h"


namespace horizon {
namespace sub {

	using horizon::sub::dep::Dependencies;

Adcs::Adcs() : Subsystem(SUBNAME_ADCS){
	addKey(POINTVEC_KEY);
}

Adcs::Adcs(XMLNode& adcsNode) : Subsystem(SUBNAME_ADCS){
	addKey(POINTVEC_KEY);
}

Adcs::~Adcs() {}

bool Adcs::canPerform(const State* oldState, State* newState, const Task* task, Position* position, Environment* environment, NodeDependencies* deps) const
{
	double timetoslew = (rand()%5)+8;

	double es = newState->getEventStart();
	double ts = newState->getTaskStart();

	if(es + timetoslew > ts) {
		return false;
	}	
	
	// from Brown, Pp. 99
	Matrix m_SC_pos_at_ts_ECI =position->getPosECI(ts);
	Matrix m_target_pos_at_ts_ECI = task->getTarget()->getPos()->getPosECI(ts);
	Matrix m_pv = m_target_pos_at_ts_ECI - m_SC_pos_at_ts_ECI;

	// set state data
	newState->addValue(POINTVEC_KEY, make_pair(ts, m_pv));

	return true;
}

bool Adcs::canExtend(State* state, Position* position, Environment* environment, const double evalToTime, NodeDependencies* deps) const {
	if (state->getEventEnd() < evalToTime)
		state->setEventEnd(evalToTime);
	return true;
}

}} // end namespace horizon::sub

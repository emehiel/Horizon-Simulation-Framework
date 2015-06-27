#include "Power.h"
#include <list>
#include "dep/Dependencies.h"

namespace horizon {
namespace sub {
		
	using namespace horizon::sub::dep;

Power::Power() : Subsystem(SUBNAME_POWER)
{
	batterysize = 1000000;
	full_solarpanelpower = 150;
	penumbra_solarpanelpower = 75;
	addKey(DOD_KEY);
	addKey(POWIN_KEY);
}

Power::Power(XMLNode& powerNode) : Subsystem(powerNode.getAttribute("subsystemName")), 
	batterysize(atoi(powerNode.getAttribute("batterySize"))),
	full_solarpanelpower(atoi(powerNode.getAttribute("fullSolarPower"))),
	penumbra_solarpanelpower(atoi(powerNode.getAttribute("penumbraSolarPower"))),
	subNode(powerNode)
{
	addKey(DOD_KEY);
	addKey(POWIN_KEY);
}

Power::~Power() {}

bool Power::canPerform(const State* oldState, State* newState, const Task* task, Position* position, Environment* environment, NodeDependencies* deps) const
{
	double es = newState->getEventStart();
	double te = newState->getTaskEnd();
	double ee = newState->getEventEnd();
	double powerSubPowerOut = 10;

	if(ee>simParams::SIMEND_SECONDS())
		return false;

	// get the old DOD
	double olddod = oldState->getLastValue(dkeys.front()).second;

	// collect power profile out
	Profile<double> powerOut = deps->callDoubleDependency("POWERSUB_getPowerProfile");
	powerOut = powerOut + powerSubPowerOut;
	// collect power profile in
	Profile<double> powerIn = calcSolarPanelPowerProfile(es, te, newState, position, environment);
	// calculate dod rate
	Profile<double> dodrateofchange = ((powerOut-powerIn)/batterysize);

	bool exceeded;
	double freq = 5.0;
	Profile<double> dodProf = dodrateofchange.lowerLimitIntegrateToProf(es, te, freq, 0.0, exceeded, 0, olddod);

	newState->addValue(DOD_KEY, dodProf);
	return true;
}

bool Power::canExtend(State* newState, Position* position, Environment* environment, const double evalToTime, NodeDependencies* deps) const
{
	double ee = newState->getEventEnd();
	if(ee>simParams::SIMEND_SECONDS())
		return false;

	Sun* sun = environment->getSun();
	double te = newState->getLastValue(DOD_KEY).first;
	if (newState->getEventEnd() < evalToTime)
		newState->setEventEnd(evalToTime);

	// get the dod initial conditions
	double olddod = newState->getValueAtTime(DOD_KEY, te).second;

	// collect power profile out
	Profile<double> powerOut;
	powerOut[te] = 65;
	// collect power profile in
	Profile<double> powerIn = calcSolarPanelPowerProfile(te, ee, newState, position, environment);
	// calculate dod rate
	Profile<double> dodrateofchange = ((powerOut-powerIn)/batterysize);

	bool exceeded_lower, exceeded_upper;
	double freq = 5.0;
	Profile<double> dodProf = dodrateofchange.limitIntegrateToProf(te, ee, freq, 0.0, 1.0, exceeded_lower, exceeded_upper, 0, olddod);
	if(exceeded_upper)
		return false;
	newState->addValue(DOD_KEY, dodProf);
	return true;
}

double Power::getSolarPanelPower(shadow_state shadow) const
{
	switch(shadow)
	{
		case UMBRA:
			return 0;
		case PENUMBRA:
			return penumbra_solarpanelpower;
		default:
			return full_solarpanelpower;
	}
}

Profile<double> Power::calcSolarPanelPowerProfile(double start, double end, State* state, Position* position, Environment* environment) const {
	// create solar panel profile for this event
	Profile<double> solarpanelpowerprofile;
	double freq = 5;

	shadow_state last_shadow = environment->getSun()->castShadowOnPos(position, start);
	solarpanelpowerprofile[start] = getSolarPanelPower(last_shadow);

	for( double time = start+freq; time <= end; time += freq) {
		shadow_state shadow = environment->getSun()->castShadowOnPos(position, time);
		// if the shadow state changes during this step, save the power data
		if(shadow != last_shadow) {
			solarpanelpowerprofile[time] = getSolarPanelPower(shadow);
			last_shadow = shadow;
		}		
	}
	state->addValue(POWIN_KEY, solarpanelpowerprofile);
	return solarpanelpowerprofile;
}

}} // end namespace horizon::sub

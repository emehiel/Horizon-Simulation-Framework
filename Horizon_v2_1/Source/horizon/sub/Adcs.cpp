#include "Adcs.h"
#include "dep/Dependencies.h"


namespace horizon {
namespace sub {

	using horizon::sub::dep::Dependencies;

Adcs::Adcs() : Subsystem(SUBNAME_ADCS){
	addKey(POINTVEC_KEY);
	addKey(LOOKANGLE_KEY);
	addKey(SUNEL_KEY);
	max_vel = 1.0;
}

Adcs::Adcs(XMLNode adcsNode) : Subsystem(SUBNAME_ADCS){
	addKey(POINTVEC_KEY);
	addKey(LOOKANGLE_KEY);
	addKey(SUNEL_KEY);
	max_vel = atof(adcsNode.getAttribute("maxVelocity"));
	minSunElevationAngle = atof(adcsNode.getAttribute("minSunElevationAngle"));
}

Adcs::~Adcs() {}

bool Adcs::canPerform(const State* oldState, State* newState, const Task* task, Position* position, Environment* environment, NodeDependencies* deps) const
{
	double csTime = newState->getTaskStart();

	// Check illumination at Target
	Matrix sunVec = environment->getSun()->getEarSunVec(csTime);	// Sun Vector in ECI
	Matrix r = position->getPosECI(csTime);							// Vehicle location ECI
	Matrix p = task->getTarget()->getPos()->getPosECI(csTime);		// Target Location ECI
	Matrix v = position->getVelECI(csTime);  // vehicle velocity ECI
	Matrix h_h = cross(r,v)/norm(cross(r,v));  // orbital unit angular velocity

	double el = M_PI_2 - acos(dot(p/norm(p), sunVec/norm(sunVec)));
	el *= (180.0/M_PI);
	//if (el < minSunElevationAngle)
	//	return false;

	newState->addValue(SUNEL_KEY, make_pair(csTime, el));

	// Determin Look angle to target
	Matrix rho = p - r;
	double rho_n = norm(rho);
	double z = norm(r) - EARTH_RADIUS;
	double a = 0.0;
	if (z/rho_n > 0.999)
		a = 0.0;
	else
		a = acos(z/rho_n);

	newState->addValue(LOOKANGLE_KEY, make_pair(csTime,a*180/M_PI));

	// Determin Slew Time
	// Find the direction off Nadir we're looking
	double dir;
	if (dot(h_h, rho) > 0)
		dir = 1;
	else
		dir = -1;

	a *= dir*(180.0/M_PI);

	double timeToSlewSec = a/max_vel;

	double es = newState->getEventStart();
	double ts = newState->getTaskStart();

	//if(es + timeToSlewSec > ts) {
	//	return false;
	//}	
	
	// Determin Pointing Vector in ECI
	newState->addValue(POINTVEC_KEY, make_pair(ts, rho));

	return true;
}

bool Adcs::canExtend(State* state, Position* position, Environment* environment, const double evalToTime, NodeDependencies* deps) const {
	if (state->getEventEnd() < evalToTime)
		state->setEventEnd(evalToTime);
	return true;
}

}} // end namespace horizon::sub

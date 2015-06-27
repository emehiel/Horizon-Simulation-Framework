#ifndef POWER_H
#define POWER_H

#include <utility>
#include <list>
#include <string>
#include <iostream>

#include "../util/Matrix.h"
#include "../Subsystem.h"
#include "horizon/io/xmlParser.h"

namespace horizon {
namespace sub {

//#define SUBNAME_POWER "Power"
//#define STATEVARNAME_DOD "DepthofDischarge"
//#define STATEVARNAME_POWIN "SolarPanelPowerIn"

//#define DOD_KEY StateVarKey<double>(STATEVARNAME_DOD)
//#define POWIN_KEY StateVarKey<double>(STATEVARNAME_POWIN)

	using std::pair;
	using std::make_pair;
	using std::list;
	using std::string;
	using horizon::io::XMLNode;
	using horizon::util::Matrix;
	using horizon::geom::shadow_state;
	using horizon::geom::Sun;
	using namespace geom;

class Power : public Subsystem {

	XMLNode subNode;

public:
	
	/** Creates a new power subsystem with default parameter values. */
	Power();

	/** Creates a new Power subsystem using the values in the XMLNode. */
	Power(XMLNode& powerNode);

	virtual Power* Power::clone() const;

	/** Destroys the power subsystem. */
	virtual ~Power();

	/**
	 * Returns whether the Power subsystem can perform the operations necessary for
	 * the completion of the given task.
	*/
	virtual bool canPerform(const State* oldState, State* newState, const Task* task, Position* position, Environment* environment, NodeDependencies* dependencies) const;

	virtual bool canExtend(State* newState, Position* position, Environment* environment, const double evalToTime, NodeDependencies* dependencies) const;


private:
	// private methods

	double batterysize;
	double full_solarpanelpower;
	double penumbra_solarpanelpower;

	double getSolarPanelPower(shadow_state shadow) const;
	Profile<double> calcSolarPanelPowerProfile(double start, double end, State* state, Position* position, Environment* environment) const;

};

inline Power* Power::clone() const {
	return new Power(*this);
}

}} // end namespace horizon::sub

#endif  // POWER_H
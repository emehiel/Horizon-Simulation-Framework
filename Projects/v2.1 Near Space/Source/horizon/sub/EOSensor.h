#ifndef EOSENSOR_H
#define EOSENSOR_H

#include "../Subsystem.h"
#include "../util/umath.h"
#include "../io/xmlParser.h" 
#include "../util/Matrix.h"
#include "../../simParams.h"
#include <cstdlib>
#include <cmath>
#include <utility>
#include "math.h"


namespace horizon {
namespace sub {

#define _USE_MATH_DEFINES
//#define SUBNAME_EOSENSOR "EOSensor"
//#define STATEVARNAME_PIXELS "numPixels"
//#define STATEVARNAME_INCIDENCE "IncidenceAngle"
//#define STATEVARNAME_EOON "EOSensorOn"


//#define PIXELS_KEY StateVarKey<double> (STATEVARNAME_PIXELS)
//#define INCIDENCE_KEY StateVarKey<double> (STATEVARNAME_INCIDENCE)
//#define EOON_KEY StateVarKey<bool> (STATEVARNAME_EOON)

	using namespace horizon::io;
	using horizon::util::umath::dot;
	using horizon::util::norm;

/**
 * A model of a default EO sensor.
 * @author Travis Lockyer
 */
class EOSensor : public Subsystem {

	XMLNode subNode;

public:

	/** Creates a new EO sensor with default parameter values. */
	EOSensor();

	/** Creates a new EOSensor subsystem using the values in the XMLNode. */
	EOSensor(XMLNode ssdrNode);

	/** Destroys the EO sensor. */
	~EOSensor();

	/**
	 * Returns whether the EOSensor subsystem can perform the operations necessary for
	 * the completion of the given task.
	*/
	virtual bool canPerform(const State* oldState, State* newState, const Task* task, Position* position, Environment* environment, NodeDependencies* deps) const;

	virtual bool canExtend(State* newState, Position* position, Environment* environment, const double evalToTime, NodeDependencies* deps) const;

private:

	double lowQualityPixels;
	double lowQualityTime;
	double midQualityPixels;
	double midQualityTime;
	double highQualityPixels;
	double highQualityTime;
	// Parametric definitions

};

}} // end namespace horizon::sub

#endif  // EOSENSOR_H
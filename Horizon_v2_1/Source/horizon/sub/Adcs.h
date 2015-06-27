#ifndef ADCS_H
#define ADCS_H

#include "../Subsystem.h"
#include "../io/xmlParser.h"
#include <cstdlib>
#include "../../simParams.h"
#include "../util/umath.h"

namespace horizon {
namespace sub {

//#define SUBNAME_ADCS "Adcs"
//#define STATEVARNAME_POINTVEC "ECI_Pointing_Vector(XYZ)"

//#define POINTVEC_KEY StateVarKey<Matrix> (STATEVARNAME_POINTVEC)

	using namespace horizon::io;
	using horizon::util::umath::cross;
	using horizon::util::umath::dot;

/**
 * A model of a default ADCS System.
 * @author Cory O'Connor
 */

class Adcs : public Subsystem
{
public:

	/** Creates a new ADCS System with default parameter values. */
	Adcs();

	/** Creats a new ADCS subsystem using the values in the XMLNode. */
	Adcs(XMLNode adcsNode);

	/** Destroys the ADCS Subsystem. */
	virtual ~Adcs();

	/**
	 * Returns whether the ADCS subsystem can perform the operations necessary for
	 * the completion of the given task.
	*/
	virtual bool canPerform(const State* oldState, State* newState, const Task* task, Position* position, Environment* environment, NodeDependencies* deps) const;

	virtual bool canExtend(State* state, Position* position, Environment* environment, const double evalToTime, NodeDependencies* deps) const;

private:

	// Parametric definitions

	double max_vel; // (deg/s) from WV1 Datasheet - Eric Mehiel (9/26/07)
	double minSunElevationAngle;  // Degrees
	// Private Functions

};

}} // end namespace horizon::sub

#endif
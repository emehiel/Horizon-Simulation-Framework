#ifndef COMM_H
#define COMM_H

#include "../Subsystem.h"
#include "../util/Matrix.h"
#include "../util/umath.h"
#include "../geom/geomUtil.h"
#include "../geom/Position.h"

namespace horizon {
namespace sub {

//#define SUBNAME_COMM "Comm"
//#define STATEVARNAME_DATARATE "DataRate(MB/s)"

//#define DATARATE_KEY StateVarKey<double> (STATEVARNAME_DATARATE)

	using namespace horizon::io;

class Comm : public Subsystem {

public:

	/** Creates a new EO sensor with default parameter values. */
	Comm();

	Comm(XMLNode& commNode);

	/** Destroys the Comm subsystem. */
	~Comm();

	/**
	 * Returns whether the Comm subsystem can perform the operations necessary for
	 * the completion of the given task.
	*/
	virtual bool canPerform(const State* oldState, State* newState, const Task* task, Position* position, Environment* environment, NodeDependencies* deps) const;
	
	virtual bool canExtend(State* newState, Position* position, Environment* environment, const double evalToTime, NodeDependencies* deps) const;

private:

};

}} // end namespace horizon::sub

#endif  // COMM_H
#ifndef SSDR_H
#define SSDR_H

#include <list>
#include <map>
#include <string>
#include <cmath>

#include "../Subsystem.h"
#include "horizon/io/xmlParser.h"

namespace horizon {
namespace sub {

//#define SUBNAME_SSDR "SSDR"
//#define STATEVARNAME_DATABUFFERRATIO "DataBufferFillRatio"

//#define DATABUFFERRATIO_KEY StateVarKey<double> (STATEVARNAME_DATABUFFERRATIO)

	using namespace horizon::io;

class Ssdr : public Subsystem {

	XMLNode subNode;

public:

	/** Creates a new SSDR subsystem with default parameter values. */
	Ssdr();

	/** Creates a new SSDR subsystem using the values in the XMLNode. */
	Ssdr(XMLNode& ssdrNode);

	/** Destroys the SSDR subsystem. */
	virtual ~Ssdr();

	/**
	 * Returns whether the SSDR subsystem can perform the operations necessary for
	 * the completion of the given task.
	*/
	virtual bool canPerform(const State* oldState, State* newState, const Task* task, Position* position, Environment* environment, NodeDependencies* deps) const;

	virtual bool canExtend(State* newState, Position* position, Environment* environment, const double evalToTime, NodeDependencies* deps) const;

private:
	// parametric definitions
	double buffersize;
};

}} // end namespace horizon::sub

#endif  // SSDR_H
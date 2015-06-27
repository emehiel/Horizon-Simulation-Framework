#ifndef ACCESS_H
#define ACCESS_H

#include "../Subsystem.h"
#include "dep/Dependencies.h"
#include "horizon/io/xmlParser.h"
#include "../../simParams.h"


namespace horizon {
namespace sub {

	using namespace horizon::io;
	using namespace horizon::geom;

/**
 * A model of the geometric access from system to task subsystem.
 * @author Eric Mehiel
 */
class Access : public Subsystem {

public:
	
	/** Creates a new Access subsystem with default parameter values. */
	Access();

	/** Creats a new Access subsystem using the values in the XMLNode. */
	Access(XMLNode accessNode);
	
	/** Destroys the Access subsystem. */
	virtual ~Access();

	/**
	 * Returns whether the Access subsystem can perform the operations necessary for
	 * the completion of the given task.
	*/
	virtual bool canPerform(const State* oldState, State* newState, const Task* task, Position* position, Environment* environment, NodeDependencies* deps) const;

	virtual bool canExtend(State* state, Position* position, Environment* environment, const double evalToTime, NodeDependencies* deps) const;

private:
	// private methods

};

}} // end namespace horizon::sub

#endif  // ACCESS_H
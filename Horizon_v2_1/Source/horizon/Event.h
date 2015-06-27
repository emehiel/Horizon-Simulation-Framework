#ifndef EVENT_H
#define EVENT_H

#include "Task.h"
#include "State.h"
#include "../simParams.h"
#include <boost/shared_ptr.hpp>

namespace horizon {

	using horizon::Task;
	using horizon::State;
	using boost::shared_ptr;

/**
 * An Asset performing a Task during a time interval, with a time history of its current State.
 * @author Cory O'Connor
 * @author Einar Pehrson
 */
class Event {

	/** The task that is to be performed. */
	const Task* task;

	/** The time history of the Asset State during the current Event. */
	shared_ptr<State> state;

public:

	/**
	 * Creates an Event, in which the Task was performed by an Asset, and the time history 
	 * of the pertinent State information was saved.
	 * @param task The Task that was performed.
	 * @param state The time history of the Asset State during the Event.
	 */
	Event(const Task* task, State* state);

	Event(Event* eventToCopyExactly);

	Event(shared_ptr<Event> eventToCopyExactly);

	~Event();

	/** Returns the task that was performed. */
	const Task* getTask() const;

	/** Returns the Asset State during the Event. */
	State* getState() const;
};

inline const Task* Event::getTask() const {
	return task; }

inline State* Event::getState() const {
	return state.get(); }

} // end namespace horizon

#endif /*EVENT_H_*/

#ifndef ASSETSCHEDULE_H
#define ASSETSCHEDULE_H

#include <vector>
#include <boost/shared_ptr.hpp>

#include "Event.h"
#include "State.h"
#include "Asset.h"

namespace horizon {

	//class Asset;
	//using horizon::Asset;
	using std::vector;
	using boost::shared_ptr;

class assetSchedule {

	/** The initial state of the schedule */
	shared_ptr<State> initialState;

	/** The events in the schedule */
	vector<shared_ptr<Event>> events;

public:

	/**
	 * Creates a new empty schedule with the given initial state.
	 * @param initialState the initial state of the system for which the schedule applies
	 */
	assetSchedule(State* initialState);

	/**
	 * Creates a new endstate-safe schedule from the given schedule. (last state copied as deep copy, all others shallow copies)
	 * @param schedToMakeSafeCopyFrom the schedule to copy
	 */
	assetSchedule(assetSchedule* schedToMakeSafeCopyFrom);

	/**
	 * Creates a new assetSchedule from and old assetSchedule and a new Event shared pointer
	 * @param oldSchedule the old schedule to base this schedule off of
	 * @param newEvent the new event to add to the schedule
	 */
	assetSchedule(const assetSchedule* oldSchedule, shared_ptr<Event> newEvent);

	/** default destructor */
	~assetSchedule();
	
	/** Returns a list of all the events in the schedule */
	const vector<shared_ptr<Event>>& getEvents() const;

	/**
	* Returns the initial State of the Schedule
	* @return the initial State
	*/
	State* getInitialState() const;

	/**
	* Returns the shared_ptr that hold the initial State
	* @return the shared_ptr for the initial State
	*/
	shared_ptr<State> getInitialStateSharedPtr() const;

	/**
	* Returns a pointer to the last State in the schedule
	* @return the last State in the schedule
	*/
	State* getLastState() const;

	/**
	* Returns a pointer to the last Task in the schedule
	* @return the last Task in the schedule
	*/
	const Task* getLastTask() const;
	
	/**
	* Returns the number of times the specified task has been completed in this schedule
	* @param newTask the task to count the times completed
	* @return the number of times the task has been completed
	*/
	const size_t timesCompletedTask(const Task* newTask);
	
	/**
	* Returns the number of events in the schedule.
	* @return the number of events in the schedule
	*/
	size_t size() const;

	/**
	* Returns whether the schedule is empty
	* @return true if the schedule contains no events, false otherwise
	*/
	const bool empty() const;
};

inline const vector<shared_ptr<Event>>& assetSchedule::getEvents() const {
	return events; }

inline State* assetSchedule::getInitialState() const {
	return initialState.get(); }

inline shared_ptr<State> assetSchedule::getInitialStateSharedPtr() const {
	return initialState; }

} // end namespace horizon

#endif /*ASSETSCHEDULE_H_*/

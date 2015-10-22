#ifndef SYSTEMSCHEDULE_H
#define SYSTEMSCHEDULE_H

#include "assetSchedule.h"

namespace horizon {

	using horizon::assetSchedule;
	using std::vector;

class systemSchedule {

	vector<assetSchedule*> assetscheds;

	/** The value of the schedule assigned from the QuantitativeScheduleEvaluator::evaluate method*/
	double scheduleValue;

public:

	/**
	 *
	 */
	systemSchedule(const vector<State*>& initialstates);
	
	/**
	 *
	 */
	systemSchedule(const systemSchedule* oldSchedule, const vector<const Task*>& newTaskList, double newTaskStartTime);

	~systemSchedule();

	const bool canAddTasks(const vector<const Task*>& newTaskList, const double newTaskStartTime);

	/** Return the value of the schedule. */
	const double getScheduleValue(void) const;

	/** Sets the value of the schedule. */
	void setScheduleValue(double value);

	const size_t getTotalNumEvents();

	State* getSubNewState(size_t assetNum);

	const Task* getSubNewTask(size_t assetNum);

	const double getLastTaskStart() const;

	const vector<assetSchedule*>& getAssetScheds() const;

	const vector<State*> getEndStates() const;
};

inline const double systemSchedule::getScheduleValue(void) const {
	return scheduleValue; }

bool schedGreater(systemSchedule* elem1, systemSchedule* elem2);

inline const vector<assetSchedule*>& systemSchedule::getAssetScheds() const {
	return assetscheds; }

} // end namespace horizon

#endif /*SYSTEMSCHEDULE_H_*/

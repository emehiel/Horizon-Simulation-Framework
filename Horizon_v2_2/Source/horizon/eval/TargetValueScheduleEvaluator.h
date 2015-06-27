#ifndef TARGETVALUESCHEDULEEVALUATOR_H
#define TARGETVALUESCHEDULEEVALUATOR_H

#include "ScheduleEvaluator.h"
#include "../sub/Ssdr.h"
#include "../util/Profile.h"
#include <boost/shared_ptr.hpp>

namespace horizon {
namespace eval {

	using namespace horizon::util;
	using boost::shared_ptr;

/**
 * A class that assigns values to schedules based on the sum of the target values
 * @author Cory O'Connor
 */
class TargetValueScheduleEvaluator : public virtual ScheduleEvaluator {

public:

	/** Creates a quantitative schedule evaluator. */
	TargetValueScheduleEvaluator();

	/** Destroys the quantitative schedule evaluator. */
	~TargetValueScheduleEvaluator();

	/**
	 * Analyzes the given schedule and assigns a value to it, based on the number
	 * of events it contains.
	 * @param schedule the schedule to be evaluated
	 * @return the value of the schedule
	 */
	double evaluate(systemSchedule& schedule) const;

	double timeBetweenEventsWithSameTask(vector<shared_ptr<Event>>::const_iterator eItStart, vector<shared_ptr<Event>>::const_iterator eItCurrent) const;
};

}} // end namespace horizon::eval

#endif /*TARGETVALUESCHEDULEEVALUATOR_H*/

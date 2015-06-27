#ifndef SCHEDULEEVALUATOR_H
#define SCHEDULEEVALUATOR_H

#include "../systemSchedule.h"
#include "../assetSchedule.h"

namespace horizon {
namespace eval {

using horizon::assetSchedule;
using horizon::systemSchedule;

/**
 * A pure virtual class that assigns values to schedules according to some heuristic.
 * @author Einar Pehrson
 */
class ScheduleEvaluator {

public:

	/** Destroys the schedule evaluator */
	virtual ~ScheduleEvaluator() = 0;

	/**
	 * Analyzes the given schedule and assigns a value to it.
	 * @param schedule the schedule to be evaluated
	 * @return the value of the schedule
	 */
	virtual double evaluate(systemSchedule& schedule) const = 0;
};

}} // end namespace horizon::eval

#endif /*SCHEDULEEVALUATOR_H_*/

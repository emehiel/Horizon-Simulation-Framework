#ifndef QUANTITATIVESCHEDULEEVALUATOR_H
#define QUANTITATIVESCHEDULEEVALUATOR_H

#include "ScheduleEvaluator.h"

namespace horizon {
namespace eval {

/**
 * A class that assigns values to schedules based on size
 * @author Einar Pehrson
 */
class QuantitativeScheduleEvaluator : public virtual ScheduleEvaluator {

public:

	/** Creates a quantitative schedule evaluator. */
	QuantitativeScheduleEvaluator();

	/** Destroys the quantitative schedule evaluator. */
	~QuantitativeScheduleEvaluator();

	/**
	 * Analyzes the given schedule and assigns a value to it, based on the number
	 * of events it contains.
	 * @param schedule the schedule to be evaluated
	 * @return the value of the schedule
	 */
	double evaluate(systemSchedule& schedule) const;
};

}} // end namespace horizon::eval

#endif /*QUANTITATIVESCHEDULEEVALUATOR_H_*/

#include "QuantitativeScheduleEvaluator.h"

namespace horizon {
namespace eval {

QuantitativeScheduleEvaluator::QuantitativeScheduleEvaluator() {}

QuantitativeScheduleEvaluator::~QuantitativeScheduleEvaluator() {}

double QuantitativeScheduleEvaluator::evaluate(systemSchedule& schedule) const {
	return (double)schedule.getTotalNumEvents();
}

}} // end namespace horizon::eval

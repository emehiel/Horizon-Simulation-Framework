#ifndef TASK_H
#define TASK_H

#include "Target.h"
#include "horizon/io/xmlParser.h"
#include <list>

namespace horizon {

using namespace horizon::io;
using std::string;
using std::list;

/**
 * An action to be performed at a target, with limitations and suggestions for
 * scheduling.
 * @author Einar Pehrson
 */
class Task {

	/** The type of task being performed */
	const string taskType;

	/** The target at which the target is to be performed */
	Target* target;

	/** The maximum number of times the task should be performed by the ENTIRE SYSTEM (all assets count towards this)*/
	size_t maxTimesToPerform;

public:

	/**
	 * Creates a new task to be performed at the given target, with the given
	 * scheduling limitations
	 * @param taskType the type of task to perform
	 * @param target the target at which the task is to be performed
	 * @param maxTimesToPerform the maximum number of times the task should be performed
	 * @param desiredTime the desired scheduled time of the task
	 * @param maxIncidenceAngle the maximum incidence angle at which the task can be performed
	 */
	Task(const string& taskType, Target* target, size_t maxTimesToPerform);

	Task();
	
	/** Returns the target at which the task is to be performed */
	const string getType() const;

	/** Returns the target at which the task is to be performed */
	Target* getTarget() const;

	/** Returns the maximum number of times the task should be performed */
	size_t getMaxTimesPerformable() const;

};

inline const string Task::getType() const {
	return taskType;
}

inline Target* Task::getTarget() const {
	return target;
}

inline size_t Task::getMaxTimesPerformable() const {
	return maxTimesToPerform;
}

} // end namespace horizon

#endif /*TARGET_H_*/

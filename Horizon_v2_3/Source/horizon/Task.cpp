#include "Task.h"

namespace horizon {

Task::Task(const string& taskType, Target* target, size_t maxTimesToPerform) : taskType(taskType), target(target), 
maxTimesToPerform(maxTimesToPerform) {}

Task::Task() : taskType(TASK_TYPE_EMPTY), target(NULL), maxTimesToPerform(10000000) {}

} // end namespace horizon

#ifndef GENSCHEDULES_H
#define GENSCHEDULES_H

#include <vector>
#include "Task.h"

namespace horizon {

	using horizon::Task;
	using std::vector;

	vector< vector<const Task*> > genExhaustiveSystemSchedules(vector< vector<const Task*> >& assetTasks);

} // end namspace horizon
#endif /* GENSCHEDULES_H */
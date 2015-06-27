#include "genSchedules.h"

namespace horizon {

	vector< vector<const Task*> > genExhaustiveSystemSchedules(vector< vector<const Task*> >& assetTasks)
	{
		vector< vector<const Task*> >::iterator assetTaskIt = assetTasks.begin();
		Task* NULLTASK = NULL;
		for(; assetTaskIt != assetTasks.end(); assetTaskIt++) {
			assetTaskIt->push_back(NULLTASK);
		}

		size_t num_combs = 1;
		for(vector< vector<const Task*> >::const_iterator assetTaskIt = assetTasks.begin(); assetTaskIt != assetTasks.end(); assetTaskIt++)
		{
			num_combs *= assetTaskIt->size();
		}
		
		vector< vector<const Task*> > comb;
		comb.reserve(num_combs);

		for(vector<const Task*>::const_iterator lIt = assetTasks.begin()->begin(); lIt != assetTasks.begin()->end(); lIt++) {
			vector<const Task*> newvec;
			newvec.push_back(*lIt);
			comb.push_back(newvec);
		}

		assetTaskIt = assetTasks.begin();
		assetTaskIt++;
		for(; assetTaskIt != assetTasks.end(); assetTaskIt++) {
			size_t lSize = assetTaskIt->size(); // number of tasks for this assset
			size_t combSize = comb.size();

			vector< vector<const Task*> > combcopy = comb;

			for(size_t j = 2; j <= lSize; j++)
				comb.insert(comb.end(), combcopy.begin(), combcopy.end());

			size_t i = 1;
			vector<const Task*>::iterator taskToAddIt = assetTaskIt->begin();
			for(vector< vector<const Task*> >::iterator combIt = comb.begin(); combIt != comb.end(); combIt++, i++) {
				combIt->push_back(*taskToAddIt);
				if(i == combSize) {
					taskToAddIt++;
					i = 0;
				}
			}
		}
		comb.pop_back();		
		return comb;
	}

} // end namspace horizon
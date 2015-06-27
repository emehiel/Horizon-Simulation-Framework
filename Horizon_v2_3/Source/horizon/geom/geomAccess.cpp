#include "geomAccess.h"

namespace horizon {
	namespace geom {

vector< vector<const Task*> > getCurrentAccesses(vector<const Task*>& taskList,  const vector< vector< map<double, bool> > >& access_pregen, double time)
{
	size_t num_tasks = access_pregen.size();
	size_t num_assets = access_pregen.at(0).size();

	// initialize vector to hold tasks each asset has access to
	vector< vector<const Task*> > assetTasks(num_assets);
	
	vector< vector< map<double, bool> > >::const_iterator accessPregenIt = access_pregen.begin();		
	vector<const Task*>::const_iterator taskIt = taskList.begin();
	
	// iterate through each task that acces has been pregenerated for
	for(; taskIt != taskList.end(); accessPregenIt++, taskIt++)
	{
		vector< map<double, bool> >::const_iterator assetAccessPregenIt = accessPregenIt->begin();
		vector< vector<const Task*> >::iterator assetTaskIt = assetTasks.begin();

		bool hasAccess;
		for(; assetAccessPregenIt != accessPregenIt->end(); assetAccessPregenIt++, assetTaskIt++)
		{
			// check the map to find if the asset has access to the task
			map<double, bool>::const_iterator access = assetAccessPregenIt->upper_bound(time);				
			if(access != assetAccessPregenIt->end()) {
				hasAccess = !(access->second);

			}
			else {
				hasAccess = assetAccessPregenIt->rbegin()->second;
			}
			// if the asset has access, push back the task onto its vector of tasks
			if(hasAccess) {
				assetTaskIt->push_back((*taskIt));
			}
		}
	}
	return assetTasks;
}

vector< vector< map<double, bool> > > pregenerateAccesses(System& system, vector<const Task*>& tasks, double stepLength, double endTime)
{
	size_t num_tasks = tasks.size();
	size_t num_assets = system.getAssets().size();
	vector< map<double, bool> > asset_pregen(num_assets);
	vector< vector< map<double, bool> > > access_pregen(num_tasks, asset_pregen);
	size_t num_access_pregen = access_pregen.size();

	vector<const Task*>::const_iterator taskIt = tasks.begin();
	vector< vector< map<double, bool> > >::iterator taskAccessIt = access_pregen.begin();
	int numAccesses = num_access_pregen;
	int c = 0;
	for(; taskAccessIt != access_pregen.end(); taskAccessIt++, taskIt++) {
		c++;
		for(double access_time = 0; access_time < endTime; access_time += stepLength) {
			vector<const Asset*>::const_iterator assetIt = system.getAssets().begin();
			for( vector< map<double, bool> >::iterator assetAccessIt = taskAccessIt->begin(); assetAccessIt != taskAccessIt->end(); assetAccessIt++, assetIt++) {
				if(assetAccessIt->empty()) {
					Matrix assetPosECI = (*assetIt)->getPos()->getPosECI(access_time);
					Matrix targetPosECI = (*taskIt)->getTarget()->getPos()->getPosECI(access_time);
					bool canAccess = hasLOS(assetPosECI, targetPosECI);
					assetAccessIt->insert(make_pair(access_time, canAccess));
				}
				else {
					bool lastAccess = assetAccessIt->rbegin()->second;
					Matrix assetPosECI = (*assetIt)->getPos()->getPosECI(access_time);
					Matrix targetPosECI = (*taskIt)->getTarget()->getPos()->getPosECI(access_time);
					bool newAccess = hasLOS(assetPosECI, targetPosECI);
					if(newAccess != lastAccess) {
						assetAccessIt->insert(make_pair(access_time, newAccess));
						printf("Inserting Accesses:  %4.2f%% done\r", 100*(double)c/(double)numAccesses);
					}
				}
			}
		}
	}
	return access_pregen;
}

	}}
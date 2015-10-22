#include "Scheduler.h"

namespace horizon {

Scheduler::Scheduler() : startTime(0), endTime(simParams::SIMEND_SECONDS()) {}

list<systemSchedule*> Scheduler::generateSchedules(System& system, vector<const Task*>& tasks, 
		const vector<State*>& initialStateList, const ScheduleEvaluator* schedVals)
{
	// get the global dependencies object
	Dependencies* dependencies = Dependencies::Instance();

	cout << endl << "SIMULATING... " << endl;
	// Create empty systemSchedule with initial state set
	list<systemSchedule*> sysScheds;
	systemSchedule* emptySched = new systemSchedule(initialStateList);
	sysScheds.push_back(emptySched);

	// if all asset position types are not dynamic types, can pregenerate accesses for the simulation
	bool canPregenAccess = true;	
	for(vector<const Asset*>::const_iterator assetIt = system.getAssets().begin(); assetIt != system.getAssets().end(); assetIt++)
		canPregenAccess &= ((*assetIt)->getPos()->getType() != POSITION_TYPE_DYNAMIC_ECI) && ((*assetIt)->getPos()->getType() != POSITION_TYPE_DYNAMIC_LLA);
	
	vector< vector< map<double, bool> > > access_pregen;
	vector< vector<const Task*> > assetTaskList;
	// if accesses can be pregenerated, do it now
	if(canPregenAccess) {
		cout << "Pregenerating Accesses..." << endl;
		access_pregen = pregenerateAccesses(system,tasks, stepLength, endTime);
		cout << "Pregenerating Accesses... DONE!" << endl;
	}
	// otherwise generate an exhaustive list of possibilities for assetTaskList
	else {
		cout << "Generating Exhaustive Task Combinations... ";
		vector< vector<const Task*> > exhaustive(system.getAssets().size(), tasks);
		assetTaskList = genExhaustiveSystemSchedules(exhaustive);
		cout << assetTaskList.size() << " tasks generated... ";
		cout << "DONE!" << endl;
	}

	/// \todo TODO: Delete (or never create in the first place) schedules with inconsistent asset tasks (because of asset dependencies)

	// Find the next timestep for the simulation
	for (double currenttime = startTime; currenttime < endTime; currenttime += stepLength)
	{
		// if accesses are pregenerated, look up the access information and update assetTaskList
		if(canPregenAccess) {
			vector< vector<const Task*> > assetTasks = getCurrentAccesses(tasks, access_pregen, currenttime);
			assetTaskList = genExhaustiveSystemSchedules(assetTasks);
		}

		// Check if it's necessary to crop the systemSchedule list to a more managable number
		if(sysScheds.size() > maxNumSchedules) cropSchedules(sysScheds, schedVals, emptySched);

		// Create a new system schedule list by adding each of the new Task commands for the Assets onto each of the old schedules
		list<systemSchedule*> newSysScheds;
		for(list<systemSchedule*>::iterator oldSchedIt = sysScheds.begin(); oldSchedIt != sysScheds.end(); oldSchedIt++) {
			for(vector< vector<const Task*> >::const_iterator newTaskListIt = assetTaskList.begin(); newTaskListIt != assetTaskList.end(); newTaskListIt++) {
				// Checks whether the System is allowed to perform the Task again and the Tasks are scheduled after the previous Events end
				if((*oldSchedIt)->canAddTasks(*newTaskListIt, currenttime)) {
					// Creates new systemSchedule based on previous systemSchedule and new Event list
					newSysScheds.push_back(new systemSchedule(*oldSchedIt, *newTaskListIt, currenttime));
				}
			}
		}

		// Generate an exhaustive list of new tasks possible from the combinations of Assets and Tasks
		/// \todo TODO: Parallelize this.
		vector<bool> systemCanPerformList;
		for(list<systemSchedule*>::iterator newSchedIt = newSysScheds.begin(); newSchedIt != newSysScheds.end(); newSchedIt++) {
			dependencies->updateStates((*newSchedIt)->getEndStates());
			systemCanPerformList.push_back(system.canPerform(*newSchedIt));
		}

		// delete systemSchedules (and corresponding lower level classes) that are not possible
		list<systemSchedule*>::iterator eraseIt = newSysScheds.begin();
		for(vector<bool>::iterator successIt = systemCanPerformList.begin(); successIt != systemCanPerformList.end(); successIt++) {
			if(*successIt) {eraseIt++;}
			else {
				delete *eraseIt; 
				eraseIt = newSysScheds.erase(eraseIt);
			}
		}

		// Merge old and new systemSchedules
		sysScheds.insert(sysScheds.begin(), newSysScheds.begin(), newSysScheds.end());

		// Print completion percentage in command window
		printf("Scheduler Status: %4.2f%% done; %i schedules generated.\r", 100*currenttime/endTime, sysScheds.size());
	}

	cropSchedules(sysScheds, schedVals);

	// extend all schedules to the end of the simulation
	for(list<systemSchedule*>::iterator schedIt = sysScheds.begin(); schedIt != sysScheds.end(); schedIt++)
	{
		dependencies->updateStates((*schedIt)->getEndStates());
		bool canExtendUntilEnd = true;
		// Iterate through Subsystem Nodes and set that they havent run
		for(vector<SubsystemNode*>::const_iterator subNodeIt = system.getSubsystemNodes().begin(); subNodeIt !=system.getSubsystemNodes().end(); subNodeIt++)
			(*subNodeIt)->reset();

		size_t subAssetNum;
		for(vector<SubsystemNode*>::const_iterator subNodeIt = system.getSubsystemNodes().begin(); subNodeIt != system.getSubsystemNodes().end(); subNodeIt++) {
			subAssetNum = (*subNodeIt)->getAssetNum();
			canExtendUntilEnd &= (**subNodeIt).canPerform((*schedIt)->getSubNewState(subAssetNum), (*schedIt)->getSubNewTask(subAssetNum), system.getEnvironment(), endTime, true);
		}
		// Iterate through constraints
		for(vector<const Constraint*>::const_iterator constraintIt = system.getConstraints().begin(); constraintIt != system.getConstraints().end(); constraintIt++)
			canExtendUntilEnd &= (*constraintIt)->accepts(*schedIt);

		if(!canExtendUntilEnd) {
			//delete *schedIt;
			cout << "Schedule may not be valid";
		}
	}
	return sysScheds;
}

void Scheduler::cropSchedules(list<systemSchedule*>& schedsToCrop, const ScheduleEvaluator* schedVals, systemSchedule* emptySched) {
	// Evaluate the schedules and set their values
	for (list<systemSchedule*>::iterator evalIt = schedsToCrop.begin(); evalIt != schedsToCrop.end(); evalIt++) {
		(*evalIt)->setScheduleValue(schedVals->evaluate(**evalIt));
	}
	// Sort the sysScheds by their values
	schedsToCrop.sort(schedGreater);
	// Delete the sysScheds that don't fit
	list<systemSchedule*>::iterator cropIt = schedsToCrop.begin();
	for(size_t i = 1; cropIt != schedsToCrop.end(); i++) {
		if((i > schedCropTo) && (*cropIt != emptySched)) {
			delete *cropIt; 
			schedsToCrop.erase(cropIt++); 
		}
		else
			cropIt++;
	}
}

} // end namespace::horizon

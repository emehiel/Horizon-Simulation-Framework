#include "Scheduler.h"

namespace horizon {

	Scheduler::Scheduler() : startTime(simParams::SIMSTART_SECONDS()),
		stepLength(schedParams::SIMSTEP_SECONDS()),
		endTime(simParams::SIMEND_SECONDS()),
		maxNumSchedules(schedParams::MAX_SCHEDS()),
		schedCropTo(schedParams::NUM_CROP())
		{}

list<systemSchedule*> Scheduler::generateSchedules(System& system, vector<const Task*>& tasks, 
		const vector<State*>& initialStateList, const ScheduleEvaluator* schedVals)
{
	system.setThreadNum(1);
	DWORD startTickCount = GetTickCount();
	accumSchedTimeMs = 0;

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
		canPregenAccess &= ((*assetIt)->getPos()->getType() != POSITION_TYPE_DYNAMIC_ECI) && ((*assetIt)->getPos()->getType() != POSITION_TYPE_DYNAMIC_ECI);
	
	vector< vector< map<double, bool> > > access_pregen;
	vector< vector<const Task*> > assetTaskList;
	// if accesses can be pregenerated, do it now
	if(canPregenAccess) {
		cout << "Pregenerating Accesses..." << endl;
		DWORD startPregenTickCount = GetTickCount();
		access_pregen = pregenerateAccesses(system,tasks, stepLength, endTime);
		DWORD endPregenTickCount = GetTickCount();
		pregenTimeMs = endPregenTickCount - startPregenTickCount;
		writeAccessReport(access_pregen, tasks);
		cout << "Pregenerating Accesses... DONE!" << endl;
	}
	// otherwise generate an exhaustive list of possibilities for assetTaskList
	else {
		cout << "Generating Exhaustive Task Combinations... ";
		vector< vector<const Task*> > exhaustive(system.getAssets().size(), tasks);
		assetTaskList = genExhaustiveSystemSchedules(exhaustive);
		cout << "DONE!" << endl;
	}

	/// \todo TODO: Delete (or never create in the first place) schedules with inconsistent asset tasks (because of asset dependencies)

	// Find the next timestep for the simulation
	DWORD startSchedTickCount = GetTickCount();
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

		// Start timing
		DWORD startAccumCount = GetTickCount();

		// Generate an exhaustive list of new tasks possible from the combinations of Assets and Tasks
		/// \todo TODO: Parallelize this.
		vector<bool> systemCanPerformList;
		for(list<systemSchedule*>::iterator newSchedIt = newSysScheds.begin(); newSchedIt != newSysScheds.end(); newSchedIt++) {
			//dependencies->updateStates(1, (*newSchedIt)->getEndStates());
			//dependencies->setThreadState(1);
			dependencies->updateStates((*newSchedIt)->getEndStates());
			systemCanPerformList.push_back(system.canPerform(*newSchedIt));
		}

		// End timing
		DWORD endAccumCount = GetTickCount();
		accumSchedTimeMs += endAccumCount - startAccumCount;

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

	DWORD endSchedTickCount = GetTickCount();
	schedTimeMs = endSchedTickCount - startSchedTickCount;

	DWORD endTickCount = GetTickCount();
	totalTimeMs = endTickCount - startTickCount;

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

void Scheduler::writeAccessReport(vector< vector< map<double, bool> > >& access_pregen, vector<const Task*>& tasks) {
	string outputDir = simParams::getOutputDir();
	string filename = "AccessReport.txt";
	string fullFilename = outputDir + "\\" + filename;

	ofstream fout(fullFilename.c_str(), ios_base::out);
	if(fout.is_open()) {
		fout << "Access report for scenario: " << simParams::getScenarioName() << endl;
		fout << "All times in seconds referenced from starting Julian date: " << simParams::SIMSTART_JD() << endl;

		// Loop through all Tasks in access_pregen, loop through tasks in parallel so that
		// we can dereference corresponding iterator to look up target name
		vector< vector< map<double, bool> > >::const_iterator apTIt = access_pregen.begin();
		vector<const Task*>::const_iterator tIt = tasks.begin();
		for(; apTIt != access_pregen.end(); apTIt++, tIt++) {
			// Here it gets a little tricky, we don't want to write out a bunch of header information if
			// there are no accesses, because it will make the file larger and hard to read. Unfortunatly
			// we dont know whether there are any accesses until we do the next two loops. A solution is
			// to write out all this header info to a string stream, not yet to a file. Also create a flag
			// which will be set to true if we find that there is an access. At the end of the loop, if the
			// flag is set, we write the info from the string stream to the file.
			ostringstream allAssetHeader;
			bool anyAssetHasAccess = false;
			// Write out the current target name
			allAssetHeader << "\tAccesses for target: " << (*tIt)->getTarget()->getName() << endl;
			// Loop through Assets for this target			
			vector< map<double, bool> >::const_iterator assIt = apTIt->begin();
			for(int assetNum = 1; assIt != apTIt->end(); assIt++, assetNum++) {
				ostringstream currAssetHeader;
				bool thisAssetHasAccess = false;
				// Write out current access (Target -> Asset)
				currAssetHeader << "\t\tAccesses for target: " << (*tIt)->getTarget()->getName() << " to Asset: " << assetNum << endl;
				// Loop through the map that contains access information for this asset/target combo
				// First check the map isnt empty
				if(!assIt->empty()) {
					map<double, bool>::const_iterator accessIt = assIt->begin();
					for(; accessIt != assIt->end(); accessIt++) {
						// Check for entry (0.0, false) and skip
						if(accessIt == assIt->begin() && !accessIt->second)
							continue;
						string lineHdr = accessIt->second ? "Start access: " : "End access: ";
						currAssetHeader << "\t\t\t" << lineHdr << accessIt->first << endl;
						thisAssetHasAccess = true;
						anyAssetHasAccess = true;
					}	
				}
				if(thisAssetHasAccess)
					allAssetHeader << currAssetHeader.str();
			}
			if(anyAssetHasAccess)
				fout << allAssetHeader.str();
		}
		fout.close();
	}




}

} // end namespace::horizon

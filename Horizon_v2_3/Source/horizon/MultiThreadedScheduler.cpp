#include "MultiThreadedScheduler.h"

namespace horizon {

MultiThreadedScheduler::MultiThreadedScheduler(int numThreads)
{
	if(nThread < 1) {
		SYSTEM_INFO sysinfo;
		GetSystemInfo( &sysinfo );
		DWORD numCPU = sysinfo.dwNumberOfProcessors;
		nThread = (int)numCPU;
		if(nThread < 1 || nThread > 8) nThread = 8;
	} else {
		nThread = numThreads;
	}
}

list<systemSchedule*> MultiThreadedScheduler::generateSchedules(System& system, vector<const Task*>& tasks, 
		const vector<State*>& initialStateList, const ScheduleEvaluator* schedVals)
{
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

	// Create thread handles and schedules
	int nThreadsUsed = 0;
	HANDLE* tHandles = new HANDLE[nThread];
	
	vector<System*> systems;
	systems.push_back(&system);
	for(int i = 1; i < nThread; i++) {
		System* sNew = new System(system);
		systems.push_back(sNew);
	}
	ThreadData* threadDataArr = new ThreadData[nThread];
	for(int i = 0; i < nThread; i++) {
		threadDataArr[i].threadNum = i;
		threadDataArr[i].system = systems.at(i);
	}
	for(int i = 0; i < nThread; i++) {
		threadDataArr[i].system->setDependencies(Dependencies::Instance(threadDataArr[i].threadNum));
	}

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

		// Generate an exhaustive list of new tasks possible from the combinations of Assets and Tasks
		
		/*if(newSysScheds.size() > 3) {
			list<systemSchedule*>::iterator eraseIt = newSysScheds.begin();
			eraseIt++;
			eraseIt++;
			eraseIt++;
			for(; eraseIt != newSysScheds.end();) {
				delete *eraseIt; 
				eraseIt = newSysScheds.erase(eraseIt);
			}
		}*/

		// Start timing
		DWORD startAccumCount = GetTickCount();

		// Copy list of schedule results
		int nSched = newSysScheds.size();
		vector<bool> systemCanPerformList(nSched);
		list<systemSchedule*>::iterator schedItL = newSysScheds.begin();

		// Initial threads
		nThreadsUsed = 0;
		int schedI = 0;
		while((nThreadsUsed < nThread) && (schedI < nSched)) {
			// Add schedule
			threadDataArr[nThreadsUsed].schedNum = schedI;
			threadDataArr[nThreadsUsed].schedule = *schedItL;
			threadDataArr[nThreadsUsed].result = false;
			// Run thread
			HANDLE hThread;
			unsigned threadID;
			hThread = (HANDLE)_beginthreadex(NULL, 0, &ThreadFunc, (void*)&threadDataArr[nThreadsUsed], 0, &threadID);
			// Update handles
			tHandles[nThreadsUsed] = hThread;
			schedI++;
			schedItL++;
			nThreadsUsed++;
		}

		// Loop until all schedules have been evaluated
		while(schedI < nSched) {
			// Wait for a thread to finish
			DWORD wRet;
			wRet = WaitForMultipleObjects(nThreadsUsed, tHandles, false, INFINITE);
			int idx = wRet - WAIT_OBJECT_0;
			// Set true/false
			systemCanPerformList[threadDataArr[idx].schedNum] = threadDataArr[idx].result;
			// Close thread
			CloseHandle(tHandles[idx]);
			// Create new thread
			threadDataArr[idx].schedNum = schedI;
			threadDataArr[idx].schedule = *schedItL;
			threadDataArr[idx].result = false;
			// Run thread
			HANDLE hThread;
			unsigned threadID;
			hThread = (HANDLE)_beginthreadex(NULL, 0, &ThreadFunc, (void*)&threadDataArr[idx], 0, &threadID);
			// Update handles
			tHandles[idx] = hThread;
			schedI++;
			schedItL++;
		}
		// Wait for all threads to finish
		DWORD wRet;
		wRet = WaitForMultipleObjects(nThreadsUsed, tHandles, true, INFINITE);
		// Iterate through threads
		for(int i = 0; i < nThreadsUsed; i++) {
			// Set true/false
			systemCanPerformList[threadDataArr[i].schedNum] = threadDataArr[i].result;
			// Close thread
			CloseHandle(tHandles[i]);
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

unsigned __stdcall ThreadFunc( void* data) {
	ThreadData* tdata = (ThreadData*)data;
	tdata->system->setThreadNum(tdata->threadNum);
	Dependencies* dependencies = Dependencies::Instance(tdata->threadNum);
	dependencies->updateStates(tdata->schedule->getEndStates());
	
	tdata->result = tdata->system->canPerform(tdata->schedule);
	_endthreadex(0);
	return 0;
}

} // end namespace::horizon

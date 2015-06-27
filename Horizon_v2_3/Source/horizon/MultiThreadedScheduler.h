#ifndef MULTI_THREADED_SCHEDULER_H
#define MULTI_THREADED_SCHEDULER_H

//#include <windows.h>
#include <stdio.h>
#include <process.h>

#include "Scheduler.h"

#include "Task.h"
#include "systemSchedule.h"
#include "assetSchedule.h"
#include "System.h"
#include "Event.h"
#include "../simParams.h"
#include "horizon/eval/ScheduleEvaluator.h"
#include "genSchedules.h"
#include "geom/geomAccess.h"
#include "sub/dep/Dependencies.h"
#include <list>
#include <vector>
#include <map>
#include <sstream>
#include <iostream>
#include <fstream>

namespace horizon {

	using horizon::sub::dep::Dependencies;
	using std::sort;
	using std::list;
	using namespace horizon::eval;
	using namespace horizon::geom;

struct ThreadData {
	int threadNum;
	int schedNum;
	System* system;
	systemSchedule* schedule;
	bool result;
};

class MultiThreadedScheduler : public Scheduler {
	/** The simulation start time (seconds). */
	//double startTime;

	/** The simulation step size (seconds). */
	//double stepLength;

	/** The simulation end time (seconds). */
	//double endTime;

	/** The maximum number of schedules to maintain in the schedule deque */
	//size_t maxNumSchedules;

	/** The number of schedules to crop to if the max number of schedules is reached */
	//size_t schedCropTo;

public:
	/*DWORD totalTimeMs;
	DWORD pregenTimeMs;
	DWORD schedTimeMs;
	DWORD accumSchedTimeMs;*/
	int nThread;

	/**
	 * Creates a scheduler for the given system
	 */
	MultiThreadedScheduler(int numThreads = -1);

	/**
	 * Generates valid schedules for the systems
	 * @param system the system for which to generate schedules
	 * @param tasks the tasks to schedule
	 * @param initialStateList the initial State list of the system
	 * @param startTime the earliest time at which tasks can be scheduled to start
	 * @param endTime the latest time at which tasks can be scheduled to end
	 * @param stepLength the amount of time between each possible task start time
	 * @return a list of dynamically allocated schedules
	 */
	list<systemSchedule*> generateSchedules(System& system, vector<const Task*>& tasks,
		const vector<State*>& initialStateList, const ScheduleEvaluator* schedVals);

	/** 
	 * Sets the max number of schedules saved before cropping.
	 * @param numSched The number of schedules.
	 */
	//void setMaxNumSchedules(size_t numSched);

	/** 
	 * Sets the number of schedules to crop to after the max number of schedules is reached.
	 * @param cropTo The number of schedules to crop to.
	 */
	//void setSchedCropTo(size_t cropTo);

	/** 
	 * Sets the step size of the simulation
	 * @param cropTo The number of schedules to crop to.
	 */
	//void setStepLength(double time);

private:

	//void cropSchedules(list<systemSchedule*>& schedsToCrop, const ScheduleEvaluator* schedVals, systemSchedule* emptySched = 0);

	//void writeAccessReport(vector< vector< map<double, bool> > >& access_pregen, vector<const Task*>& tasks);

};

/*inline void MultiThreadedScheduler::setStepLength(double time) { stepLength = time; }

inline void MultiThreadedScheduler::setMaxNumSchedules(size_t numSched) { maxNumSchedules = numSched; }

inline void MultiThreadedScheduler::setSchedCropTo(size_t cropTo) { schedCropTo = cropTo; }*/

unsigned __stdcall ThreadFunc( void* data);


} // end namespace horizon

#endif /*SCHEDULER_H_*/

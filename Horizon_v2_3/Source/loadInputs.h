/**
 * \file loadInputs.h
 * This file contains functions used in setting up the simulation.
 * @author Brian Butler
 * @author Cory O'Connor
 */

#ifndef LOADINPUTS_H
#define LOADINPUTS_H

#include <list>
#include "horizon/Scheduler.h"
#include "horizon/State.h"
#include "horizon/Task.h"
#include "horizon/io/xmlParser.h"
#include "horizon/Target.h"
#include "horizon/sub/Ssdr.h"
#include "horizon/sub/Power.h"
#include "horizon/sub/Comm.h"
#include "horizon/sub/EOSensor.h"
#include "horizon/util/Profile.h"

using std::list;
using horizon::Scheduler;
using horizon::State;
using horizon::Task;
using horizon::Target;
using namespace horizon::io;
using namespace horizon::sub;
using horizon::util::Profile;
using horizon::util::StateVarKey;

/** 
 * Sets the initial State of the simulation given the input XML files.
 * @param subsystemXMLNode The main XMLNode from the model input XML file.
 * @param initialState A pointer to the initial state vector that will be filled.
 * @return true if the initial states were set successfully, false otherwise.
 */
bool setInitialSystemState(XMLNode& modelInputXMLNode, vector<State*>* initialState);

/**
 * Loads all the scheduler parameters into the scheduler from the XML input file.
 * @param schedParametersNodeXML The XMLNode to read parameters from.
 * This XMLNode must have the following parameters:<BR>
 * <CODE>MaxSchedules</CODE> - The number of schedules that must be generated to trigger cropping.<BR>
 * <CODE>CropTo</CODE> - The number of schedules that will remain after cropping.<BR>
 * <CODE>SimStep</CODE> - The time step that the Scheduler will attempt to schedule Tasks on.
 * @param scheduler A pointer to the scheduler to write the parameters to.
 * @return true if the parameters were successfully set, false otherwise.
 */
bool loadSchedulerParams(XMLNode& schedParametersNodeXML, Scheduler* scheduler);

/**
 * Loads Tasks specified in the XMLNode into the Task list.
 * @param targetDeckXMLNode The target deck XMLNode that contains all information about all Tasks.
 * @param tasks A pointer to the list of tasks (previously created) that will be filled up with the Tasks.
 * @return true if all the targets were loaded in correctly, false otherwise.
 */
const bool loadTargetsIntoTaskList(XMLNode& targetDeckXMLNode, vector<const Task*>* tasks);

#endif /* LOADINPUTS_H */
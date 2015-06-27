#ifndef LOADINPUTS_H
#define LOADINPUTS_H

#include <list>
#include "horizon/Scheduler.h"
#include "horizon/State.h"
#include "horizon/Task.h"
#include "horizon/SubsystemNode.h"
#include "horizon/Subsystem.h"
#include "horizon/io/xmlParser.h"
#include "horizon/Target.h"
#include "horizon/sub/Ssdr.h"
#include "horizon/sub/Power.h"
#include "horizon/sub/Comm.h"
#include "horizon/sub/EOSensor.h"
#include "horizon/util/Profile.h"
#include "horizon/util/Quat.h"

using std::list;
using horizon::Scheduler;
using horizon::State;
using horizon::Task;
using horizon::SubsystemNode;
using horizon::Subsystem;
using horizon::Target;
using namespace horizon::io;
using namespace horizon::sub;
using horizon::util::Profile;
using horizon::util::StateVarKey;
using horizon::util::Quat;
using horizon::util::atoq;
using horizon::util::atom;

/** 
 * Sets the initial State of the simulation given the input XML files.
 * @param simParametersXMLNode The simulation parameters node. Used for start time.
 * @param subsystemXMLNode The subsystem initial conditions and parameters node. Used for basic subsystems parameters
 * and to specify the initial conditions for the state values of the subsystems.
 * @param initialState The initial state to store the data in.
 */
bool setInitialSystemState(vector<XMLNode>& subsystemXMLNode, vector<SubsystemNode*>& subNodeList, vector<State*>* initialState);

/**
 * Loads all the scheduler parameters into the scheduler from the XML input file
 * @param simParametersNodeXML The node to read parameters from.
 * @param scheduler The scheduler to write the parameters to.
 * @return Whether the parameters were successfully set.
 */
//bool loadSchedulerParams(Scheduler* scheduler);

/**
 * Loads targets specified in the target deck into the task list.
 * @param tasks The list of tasks (previously created) that will be filled up with the targets.
 * @param targDeckFileName The name of the target deck file, specified in the executable arguments.
 * @return Whether all the targets were loaded in correctly.
 */
const bool loadTargetsIntoTaskList(XMLNode& targDeckFileName, vector<const Task*>* tasks);

#endif /* LOADINPUTS_H */
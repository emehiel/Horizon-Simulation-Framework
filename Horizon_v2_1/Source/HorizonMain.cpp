#define _USE_MATH_DEFINES
// Horizon Framework includes - DO NOT MODIFY
#include "simParams.h"
#include "schedParams.h"
#include "loadInputs.h"
#include "horizon/Asset.h"
#include "horizon/Scheduler.h"
#include "horizon/State.h"
#include "horizon/System.h"
#include "horizon/Task.h"
#include "horizon/Subsystem.h"
#include "horizon/SubsystemNode.h"
#include "horizon/Constraint.h"
#include "horizon/assetSchedule.h"
#include "horizon/systemSchedule.h"
#include "horizon/geom/Environment.h"
#include "horizon/geom/Sun.h"
#include "horizon/io/xmlParser.h"
#include "horizon/io/scheduleDataWriter.h"
#include <windows.h>
// Horizon Framework Usings	 - DO NOT MODIFY
using namespace horizon;
using namespace horizon::io;
using namespace horizon::eval;
using namespace horizon::geom;
using namespace horizon::util::eoms;
// STL Includes - DO NOT MODIFY
#include <fstream>
#include <list>
// STL Usings - DO NOT MODIFY
using namespace std;

// Utilities
#include "horizon/eval/TargetValueScheduleEvaluator.h"	
#include "horizon/util/eoms/EOMS.h"
#include "horizon/util/eoms/orbital_EOMS_J2.h"
#include "horizon/util/Matrix.h"
#include "horizon/geom/geomUtil.h"
// Utility Usings
using horizon::util::Matrix;
using horizon::util::atom;
// Dependencises
#include "horizon/sub/dep/Dependencies.h"
#include "horizon/sub/dep/NodeDependencies.h"

// USER - Include Subsystems Here
#include "horizon/sub/Access.h"
#include "horizon/sub/Adcs.h"
#include "horizon/sub/EOSensor.h"
#include "horizon/sub/Ssdr.h"
#include "horizon/sub/Comm.h"
#include "horizon/sub/Power.h"
// USER - Include Constraints Here
#include "horizon/constraints/maxBatteryDOD.h"
#include "horizon/constraints/maxDataBuffer.h"
// USER - Subsystem Usings
using namespace horizon::sub;
// USER - Constraint Usings
using namespace horizon::constraints;

#include "horizon/geom/CloudCover.h"

int main( int argc, char *argv[ ], char *envp[ ] )
{
	string inputFileName = argv[1];
	string targDeckFileName = argv[2];	
	string outputPath = argv[3];

	// Find the main input node from the XML input files
	XMLNode mainXMLNode = XMLNode::openFileHelper(inputFileName.c_str() , "SCENARIO");
	cout << endl << "EXECUITING SCENARIO: " << mainXMLNode.getAttribute("scenarioName") << endl;
	
	// Load the simulation parameters from the XML input file
	XMLNode simParametersXMLNode = mainXMLNode.getChildNode("SIMULATION_PARAMETERS");
	bool simParamsLoaded = simParams::LoadSimParameters(simParametersXMLNode);
	// Load the scheduler parameters defined in the XML input file
	bool paramsLoaded = schedParams::loadSchedulerParams(simParametersXMLNode);
	Scheduler* systemScheduler = new Scheduler;

	// Load the target deck into the targets list from the XML input file
	XMLNode targetDeckNode = XMLNode::openFileHelper(targDeckFileName.c_str(),"TARGETDECK");
	vector<const Task*>* systemTasks = new vector<const Task*>;
	bool targetsLoaded = loadTargetsIntoTaskList(targetDeckNode, systemTasks);

	// Load the environment
	Environment* systemEnvironment = new Environment(new Sun(true));

	// Load Assets and Subsystems into xml node list
	vector<XMLNode> AssetXMLNodeList, subsystemXMLNodeList;
	int n = mainXMLNode.nChildNode("ASSET");
	for(int m = 0; m < n; m++) {
		AssetXMLNodeList.push_back(mainXMLNode.getChildNode("ASSET", m));
		int j = mainXMLNode.getChildNode("ASSET", m).nChildNode("SUBSYSTEM");
		for(int i = 0; i < j; i++)
			subsystemXMLNodeList.push_back(mainXMLNode.getChildNode("ASSET", m).getChildNode("SUBSYSTEM", i));
	}
	
	// USER - Specify data output parameters
	scheduleDataWriter dataOut(outputPath, true, 2);
	
	// USER - Instantiate Assets here
	Matrix initPosition1 = atom(AssetXMLNodeList.at(0).getAttribute("initPos"));
	Matrix initVelocity1 = atom(AssetXMLNodeList.at(0).getAttribute("initVel"));
	Matrix y1_0(initPosition1);
	vertcat2(y1_0, initVelocity1);
	EOMS* sc1EOM = new orbital_EOMS_J2();
	Asset* sc1 = new Asset(new Position(POSITION_TYPE_PREDETERMINED_ECI, sc1EOM, y1_0));
	
	/*
	Matrix initPosition2 = atom(AssetXMLNodeList.at(1).getAttribute("initPos"));
	Matrix initVelocity2 = atom(AssetXMLNodeList.at(1).getAttribute("initVel"));
	Matrix y2_0(initPosition2);
	vertcat2(y2_0, initVelocity2);
	EOMS* sc2EOM = new orbital_EOMS_J2();
	Asset* sc2 = new Asset(new Position(POSITION_TYPE_PREDETERMINED_ECI, sc2EOM, y2_0));
*/
	// USER - Create Asset list here
	vector<const Asset*> assetList;
	assetList.push_back(sc1);
	//assetList.push_back(sc2);

	// USER - Instantiate Subsystems here
	Subsystem* access = new Access(subsystemXMLNodeList.at(0).getChildNode("ACCESS"));	
	SubsystemNode* accessNode1 = new SubsystemNode(access, sc1);

	Subsystem* adcs = new Adcs(subsystemXMLNodeList.at(0).getChildNode("ADCS"));
	SubsystemNode* adcsNode1 = new SubsystemNode(adcs, sc1);
	adcsNode1->addPreceedingNode(accessNode1);
	
	Subsystem* eosensor = new EOSensor(subsystemXMLNodeList.at(0).getChildNode("EOSensor"));
	SubsystemNode* eosensorNode1 = new SubsystemNode(eosensor, sc1);
	eosensorNode1->addPreceedingNode(adcsNode1);

	Subsystem* ssdr = new Ssdr(subsystemXMLNodeList.at(0).getChildNode("Ssdr"));
	SubsystemNode* ssdrNode1 = new SubsystemNode(ssdr, sc1);
	ssdrNode1->addDependency("SSDRSUB_getNewDataProfile", &Dependencies::Asset1_SSDRSUB_getNewDataProfile);
	ssdrNode1->addPreceedingNode(eosensorNode1);

	Subsystem* comm = new Comm();
	SubsystemNode* commNode1 = new SubsystemNode(comm, sc1);
	commNode1->addDependency("COMMSUB_getDataRateProfile", &Dependencies::Asset1_COMMSUB_getDataRateProfile);
	commNode1->addPreceedingNode(ssdrNode1);

	Subsystem* power = new Power();
	SubsystemNode* powerNode1 = new SubsystemNode(power, sc1);
	powerNode1->addDependency("POWERSUB_getPowerProfile", &Dependencies::Asset1_POWERSUB_getPowerProfile);
	powerNode1->addPreceedingNode(accessNode1);
	powerNode1->addPreceedingNode(adcsNode1);
	powerNode1->addPreceedingNode(eosensorNode1);
	powerNode1->addPreceedingNode(ssdrNode1);
	powerNode1->addPreceedingNode(commNode1);

	// USER - Create Subsystem list here
	vector<SubsystemNode*> subNodeList;
	subNodeList.push_back(accessNode1);
	subNodeList.push_back(adcsNode1);
	subNodeList.push_back(eosensorNode1);
	subNodeList.push_back(ssdrNode1);
	subNodeList.push_back(commNode1);
	subNodeList.push_back(powerNode1);

	// USER - Specify schedule evaluator method
	ScheduleEvaluator* systemEvaluator = new TargetValueScheduleEvaluator();

	// USER - Instantiate Constraints here
	//Constraint* dodcon = new maxBatteryDOD();
	//dodcon->addConstrianedSubNode(powerNode1);
	//Constraint* buffcon = new maxDataBuffer();
	//buffcon->addConstrianedSubNode(ssdrNode1);

	// USER - Create Constraint list here
	vector<const Constraint*> constraintsList;
	//constraintsList.push_back(buffcon);
	//constraintsList.push_back(dodcon);

	// Load the initial state as defined in the XML input file (good for batch jobs)
	XMLNode initialStateXMLNode = mainXMLNode.getChildNode("INITIAL_STATE");
	subsystemXMLNodeList.push_back(initialStateXMLNode);
	vector<State*>* systemInitialStateList = new vector<State*>;
	bool initialStateSet = setInitialSystemState(subsystemXMLNodeList, subNodeList, systemInitialStateList);

	// Load the environment, subsystems, and constraints into a system to simulate
	System* simSystem = new System(assetList, subNodeList, constraintsList, systemEnvironment);
 	
    //------------------------------ RUN THE MAIN ALGORITHM -------------------------------- */ 
	list<systemSchedule*> schedules;
	if(!simSystem->checkForCircularDependencies())
		schedules = systemScheduler->generateSchedules(*simSystem, *systemTasks, *systemInitialStateList, systemEvaluator);
	else
		cout << "System model contains circular dependencies, cannot run simulation" << endl;
    //------------------- ----------------------------------------------- ------------------ */	

	// *********************************Output selected data*************************************
	bool schedOutput = dataOut.writeAll(schedules, simSystem);
	// ******************************************************************************************

	// Clean up memory
	delete systemScheduler;
	// Delete the initial state vector
	delete systemInitialStateList;
	// Delete the tasks
	for(vector<const Task*>::iterator tIt = systemTasks->begin(); tIt != systemTasks->end(); tIt++) { delete *tIt; }
	// Delete the task vector
	delete systemTasks;
	// Delete the Evaluator
	delete systemEvaluator;
	// Delete the System
	delete simSystem;
	// Delete subsystems
	delete access;
	/*delete adcs;
	delete eosensor;
	delete ssdr;
	delete comm;
	delete power;*/
	// Delete the possible generated schedules
	for(list<systemSchedule*>::iterator sIt = schedules.begin(); sIt != schedules.end();) {
		delete *sIt;
		schedules.erase(sIt++);
	}	
	getchar();
	return 0;
}
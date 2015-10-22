#define _USE_MATH_DEFINES
// Horizon Framework includes - DO NOT MODIFY
#include "simParams.h"
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
#include "horizon/util/eoms/orbital_EOMS.h"
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

//Nearspace needed files.
#include "horizon/geom/Station.h"
#include "horizon/geom/StationList.h"
#include "horizon/sub/BalloonDynamics.h"
#include "horizon/util/eoms/balloon_EOMS.h"
#include "horizon/sub/ssDynamics.h"
#include "horizon/util/eoms/steerfloat_EOMS.h"
#include "horizon/sub/blimpDynamics.h"
#include "horizon/util/eoms/blimp_EOMS.h"


int main( int argc, char *argv[ ], char *envp[ ] )
{
	string inputFileName = argv[1];
	string targDeckFileName = argv[2];
	string stationListFileName = argv[3];
	string heightFileName = argv[4];
	string zonalVelFileName = argv[5];
	string meridonalVelFileName = argv[6];
	string outputPath = argv[7];

	// Find the main input node from the XML input files
	XMLNode mainXMLNode = XMLNode::openFileHelper(inputFileName.c_str() , "SCENARIO");
	cout << endl << "EXECUITING SCENARIO: " << mainXMLNode.getAttribute("scenarioName") << endl;

	// Load the information for the Station Data and Grid systm
	XMLNode scratchPadXMLNode = mainXMLNode.getChildNode("SCRATCHPAD_DATA");
	StationList* list_of_stations = StationList::Instance();
	list_of_stations->createGrid(scratchPadXMLNode);
	list_of_stations->LoadStationData(stationListFileName,heightFileName,zonalVelFileName,meridonalVelFileName);
	cout<< "DONE!" <<endl;
	
	// Load the simulation parameters from the XML input file
	XMLNode simParametersXMLNode = mainXMLNode.getChildNode("SIMULATION_PARAMETERS");
	bool simParamsLoaded = simParams::LoadSimParameters(simParametersXMLNode);
	
	// Load the scheduler parameters defined in the XML input file
	Scheduler* systemScheduler = new Scheduler;
	bool paramsLoaded = loadSchedulerParams(simParametersXMLNode, systemScheduler);

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
	
	// Load the initial state as defined in the XML input file
	vector<State*>* systemInitialStateList = new vector<State*>;
	bool initialStateSet = setInitialSystemState(subsystemXMLNodeList, systemInitialStateList);

	// USER - Specify data output parameters
	scheduleDataWriter dataOut(outputPath, true, 2);
	
	// USER - Instantiate Assets here
	Matrix initPosition1 = atom(AssetXMLNodeList.at(0).getAttribute("initPos"));
	Matrix initVelocity1 = atom(AssetXMLNodeList.at(0).getAttribute("initVel"));
	Matrix assetType = atom(AssetXMLNodeList.at(0).getAttribute("assetType"));
	Matrix initCond1 = atom(AssetXMLNodeList.at(0).getAttribute("initCond"));
	Matrix nsy1_0(initPosition1);
	vertcat2(nsy1_0,initVelocity1);
	vertcat2(nsy1_0,initCond1);


	//EOMS* ns1EOM = new balloon_EOMS();
	//Asset* ns1 = new Asset(new Position(POSITION_TYPE_DYNAMIC_LLA, ns1EOM,nsy1_0));

	//EOMS* ss1EOM = new ss_EOMS();
	//Asset* ss1 = new Asset(new Position(POSITION_TYPE_DYNAMIC_LLA, ss1EOM,nsy1_0));

	EOMS* blimp1EOM = new blimp_EOMS();
	Asset* blimp1 = new Asset(new Position(POSITION_TYPE_DYNAMIC_LLA, blimp1EOM,nsy1_0));

	
	// USER - Create Asset list here
	vector<const Asset*> assetList;
	assetList.push_back(blimp1);
	
	//USER - Instantiate Subsystems here
	//Subsystem* access = new Access();	
	//SubsystemNode* accessNode1 = new SubsystemNode(access, sc1);

	//Subsystem* balloonDynamics = new Balloon_Dynamics(subsystemXMLNodeList);
	//SubsystemNode* balloonDynamicsNode1 = new SubsystemNode (balloonDynamics,ns1);

	//Subsystem* ss_Dynamics = new SS_Dynamics(subsystemXMLNodeList);
	//SubsystemNode* ssDynamicsNode1 = new SubsystemNode (ss_Dynamics,ss1);

	Subsystem* blimp_Dynamics = new Blimp_Dynamics(subsystemXMLNodeList);
	SubsystemNode* blimpDynamicsNode1 = new SubsystemNode(blimp_Dynamics,blimp1); 
	
	// USER - Create Subsystem list here
	vector<SubsystemNode*> subNodeList;
	//subNodeList.push_back(balloonDynamicsNode1);
	//subNodeList.push_back(ssDynamicsNode1);
	subNodeList.push_back(blimpDynamicsNode1);
	
	// USER - Specify schedule evaluator method
	ScheduleEvaluator* systemEvaluator = new TargetValueScheduleEvaluator();

	// USER - Instantiate Constraints here
	
	// USER - Create Constraint list here
	vector<const Constraint*> constraintsList;

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
//	delete access;
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
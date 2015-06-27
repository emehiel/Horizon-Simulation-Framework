#define _WINSOCKAPI_ // Fix for redefinition errors
#define _USE_MATH_DEFINES
// Windows Includes
//#include <windows.h>
// HSF Includes
#include "simParams.h"
#include "schedParams.h"
#include "loadInputs.h"
#include "horizon/Scheduler.h"
#include "horizon/MultiThreadedScheduler.h"
#include "horizon/System.h"
#include "horizon/io/xmlParser.h"
#include "horizon/io/scheduleDataWriter.h"
// HSF Networking Includes
#include "horizon/net/NetworkDataClient.h"
// STL Includes
#include <list>
// Dependency Includes
#include "horizon/sub/dep/NodeDependencies.h"
// Subsystem Includes
#include "horizon/sub/script/ScriptedSubsystem.h"
// Constraints Includes
#include "horizon/constraints/GenericConstraint.h"
// Subsystem Usings
using namespace horizon::sub::script;
// Constraint Usings
using namespace horizon::constraints;
// Adapter Includes
#include "horizon/util/adapt/SubsystemAdapter.h"
#include "horizon/util/adapt/ConstraintAdapter.h"
#include "horizon/util/adapt/DependencyAdapter.h"
#include "horizon/util/adapt/EOMSAdapter.h"
#include "horizon/util/adapt/ScheduleEvaluatorAdapter.h"
// Adapter Usings
using namespace horizon::util::adapt;
using horizon::util::adapt::createEOMSObject;
using horizon::util::adapt::createScheduleEvaluator;
// Logger Includes
#include "horizon/log/Logger.h"
// Logger Usings
using namespace horizon::log;
// Networking Usings
using namespace horizon::net;
// LUA / Luabind Includes
#include <lua.hpp>
//#include "horizon/script/register.h"
#include "horizon/script/luaDefs.h"
// LUA / Luabind Usings
//using namespace lua;
using namespace luabind;
//using namespace horizon::script;

#include "horizon/util/eoms/ScriptedEOMS.h"
using namespace horizon::util::eoms;
//using namespace lua;
//using namespace luabind;



//#include "horizon/net/stub/NodeDependencies_.h"
//using namespace horizon::net::stub;
#include "horizon/net/sub/NetworkedSubsystem.h"
using namespace horizon::net::sub;


//#include "neuralnet/NeuralNetSet.h"

namespace lua {
	// This state used by
	// *?
	lua_State* L;
}

int main( int argc, char *argv[ ], char *envp[ ] ) {

	lua::L = lua_open();
	luaL_openlibs(lua::L);
	open(lua::L);
	register_LUAState(lua::L);
	set_pcall_callback(&luabind::luaErrFcn);

	// Load Lua Scripts
	loadLuaFunctionsFromFile(lua::L, "io/scripts/MT Lua Test/MT Test.txt");
	addLuaFile("io/scripts/MT Lua Test/MT Test.txt");


	lua_State* L2 = horizon::script::getNewLuaState();
	bool fEx = functionExists(L2, "init");





	ScriptedSubsystem* ss1 = new ScriptedSubsystem("ss1", "init", "canPerform", "canExtend");
	//ss1->setLuaState(lua::L);

	bool res1 = ss1->canPerform(NULL, NULL, NULL, NULL, NULL, NULL);



////// BEGIN TEST
	// Load lua files
	loadLuaFunctionsFromFile(lua::L, "io/scripts/EOMS test/orbital_EOMS.txt");
	loadLuaFunctionsFromFile(lua::L, "io/scripts/EOMS test/orbital_EOMS_J2.txt");
	loadLuaFunctionsFromFile(lua::L, "io/scripts/EOMS test/orbital_EOMS_J2_DRAG.txt");


	// XML Nodes for C++
	XMLNode posNode = XMLNode::createXMLTopNode("POSITION");
	XMLNode eomsNode = XMLNode::createXMLTopNode("EOMS");
	XMLNode intNode = XMLNode::createXMLTopNode("INTEGRATOR");
	posNode.addAttribute("PositionType", "PredeterminedECI");
	posNode.addAttribute("ICs", "Matrix(6,1,{7378.137, 0.0, 0.0, 0.0, 5.1973, 5.1973})");
	eomsNode.addAttribute("EOMSType", "orbital_EOMS_TEST");
	intNode.addAttribute("rtol", "1e-9");
	intNode.addAttribute("atol", "1e-9");
	posNode.addChild(eomsNode);
	posNode.addChild(intNode);

	// XML Nodes for Lua
	XMLNode posNodeLua = XMLNode::createXMLTopNode("POSITION");
	XMLNode eomsNodeLua = XMLNode::createXMLTopNode("EOMS");
	XMLNode intNodeLua = XMLNode::createXMLTopNode("INTEGRATOR");
	posNodeLua.addAttribute("PositionType", "PredeterminedECI");
	posNodeLua.addAttribute("ICs", "Matrix(6,1,{7378.137, 0.0, 0.0, 0.0, 5.1973, 5.1973})");
	eomsNodeLua.addAttribute("EOMSType", "scripted");
	eomsNodeLua.addAttribute("initFcn", "orbital_EOMS_J2_DRAG_init");
	eomsNodeLua.addAttribute("evalFcn", "orbital_EOMS_J2_DRAG_eval");
	intNodeLua.addAttribute("rtol", "1e-9");
	intNodeLua.addAttribute("atol", "1e-9");
	posNodeLua.addChild(eomsNodeLua);
	posNodeLua.addChild(intNodeLua);

	// Structure
	Position* position = new Position(posNode);

	// Lua Structure
	Position* positionLua = new Position(posNodeLua);

	// Setup
	double endT = 8*60*60;
	simParams::setSimEnd(endT);

	// C++
	DWORD startTime = GetTickCount();
	Matrix endStateLLA =  position->getStateECI(endT);
	DWORD stopTime = GetTickCount();
	DWORD runTime = stopTime - startTime;

	// Lua
	DWORD startTimeLua = GetTickCount();
	Matrix endStateLLALua =  positionLua->getStateECI(endT);
	DWORD stopTimeLua = GetTickCount();
	DWORD runTimeLua = stopTimeLua - startTimeLua;

	return 0;
////// END TEST

	// Get the input filenames
	string simulationInputFileName = argv[1];
	string targetDeckFileName = argv[2];
	string modelInputFileName = argv[3];
	string outputPath = argv[4];

	// Initialize critical section for dependencies
	InitializeCriticalSection(&horizon::sub::dep::NodeDependencies::cs);

	// Open the LUA state and register Lua/C++ bindings
	// TEST
	lua::L = lua_open();
	luaL_openlibs(lua::L);
	open(lua::L);
	register_LUAState(lua::L);
	set_pcall_callback(&luabind::luaErrFcn);

	// Find the main input node from the XML input files
	XMLNode simulationInputXMLNode = XMLNode::openFileHelper(simulationInputFileName.c_str() , "SCENARIO");
	cout << endl << "EXECUITING SCENARIO: " << simulationInputXMLNode.getAttribute("scenarioName") << endl;
	
	// Load the simulation parameters from the XML simulation input file
	XMLNode simParametersXMLNode = simulationInputXMLNode.getChildNode("SIMULATION_PARAMETERS");
	bool simParamsLoaded = simParams::LoadSimParameters(simParametersXMLNode, simulationInputXMLNode.getAttribute("scenarioName"));

	// Load the scheduler parameters defined in the XML simulation input file
	XMLNode schedParametersXMLNode = simulationInputXMLNode.getChildNode("SCHEDULER_PARAMETERS");
	//Scheduler* systemScheduler = new Scheduler();
	//bool schedParamsLoaded = loadSchedulerParams(schedParametersXMLNode, systemScheduler);

	bool paramsLoaded = schedParams::loadSchedulerParams(schedParametersXMLNode);
	//Scheduler* systemScheduler = new Scheduler;
	MultiThreadedScheduler* systemScheduler = new MultiThreadedScheduler;

	// Load the target deck into the targets list from the XML target deck input file
	XMLNode targetDeckXMLNode = XMLNode::openFileHelper(targetDeckFileName.c_str(),"TARGETDECK");
	vector<const Task*>* systemTasks = new vector<const Task*>;
	bool targetsLoaded = loadTargetsIntoTaskList(targetDeckXMLNode, systemTasks);

	// Find the main model node from the XML model input file
	XMLNode modelInputXMLNode = XMLNode::openFileHelper(modelInputFileName.c_str() , "MODEL");

	// Load the environment. First check if there is an ENVIRONMENT XMLNode in the input file
	Environment* systemEnvironment;
	int nEnv = modelInputXMLNode.nChildNode("ENVIRONMENT");
	if(nEnv != 0) {
		// Create the Environment based on the XMLNode
		XMLNode environmentNode = modelInputXMLNode.getChildNode("ENVIRONMENT");
		systemEnvironment = new Environment(environmentNode);
	}
	else
		systemEnvironment = new Environment();

	// Initialize NetworkDataClient
	int nDataClient = modelInputXMLNode.nChildNode("NETWORK_DATA_CLIENT");
	if(nDataClient != 0) {
		XMLNode NetworkDataClientNode = modelInputXMLNode.getChildNode("NETWORK_DATA_CLIENT");
		if(NetworkDataClientNode.isAttributeSet("host")) {
			NetworkDataClient::setHost(NetworkDataClientNode.getAttribute("host"));
		}
		if(NetworkDataClientNode.isAttributeSet("port")) {
			NetworkDataClient::setPort(atoi(NetworkDataClientNode.getAttribute("port")));
		}
		if(NetworkDataClientNode.isAttributeSet("connect")) {
			if(atob(NetworkDataClientNode.getAttribute("connect")))
				NetworkDataClient::Connect();
		}
	}

	// Initialize the SubsystemAdapter
	SubsystemAdapter subAdapter;
	subAdapter.initialize();

	// Initialize the ConstraintAdapter
	ConstraintAdapter constraintAdapter;
	constraintAdapter.initialize();

	// Initialize the Dependency Adapter (for static dependencies)
	DependencyAdapter depAdapter;
	depAdapter.addDoubleDependency("Asset1_COMMSUB_getDataRateProfile", &Dependencies::Asset1_COMMSUB_getDataRateProfile);
	depAdapter.addDoubleDependency("Asset1_POWERSUB_getPowerProfile", &Dependencies::Asset1_POWERSUB_getPowerProfile);
	depAdapter.addDoubleDependency("Asset1_SSDRSUB_getNewDataProfile", &Dependencies::Asset1_SSDRSUB_getNewDataProfile);
	depAdapter.addDoubleDependency("Asset2_COMMSUB_getDataRateProfile", &Dependencies::Asset2_COMMSUB_getDataRateProfile);
	depAdapter.addDoubleDependency("Asset2_POWERSUB_getPowerProfile", &Dependencies::Asset2_POWERSUB_getPowerProfile);
	depAdapter.addDoubleDependency("Asset2_SSDRSUB_getNewDataProfile", &Dependencies::Asset2_SSDRSUB_getNewDataProfile);

	// Initialize vectors to hold assets and subsystem nodes
	vector<const Asset*> assetList;
	vector<SubsystemNode*> subNodeList;

	// Maps used to set up preceeding nodes
	map<SubsystemNode*, XMLNode> subsystemNodeXMLNodeMap;
	map<int, SubsystemNode*> subsystemNodeMap;

	// Create Constraint list here
	vector<const Constraint*> constraintsList;

	// Enable Lua scripting support, add additional functions defined in input file
	bool enableScripting = false;
	int n = modelInputXMLNode.nChildNode("LUA");
	if(n != 0) {
		XMLNode luaXMLNode = modelInputXMLNode.getChildNode("LUA");
		if(luaXMLNode.isAttributeSet("enableScripting")) {
			if(_strcmpi(luaXMLNode.getAttribute("enableScripting"),"False") != 0)
				enableScripting = true;
		}
		// Loop through all the of the file nodes
		int m = luaXMLNode.nChildNode("LUA_FILE");
		for(int i = 0; i < m; i++) {
			XMLNode luaFileXMLNode = luaXMLNode.getChildNode("LUA_FILE", i);
			// If scripting is enabled, parse the script file designated by the src attribute
			if(enableScripting) {
				// Parse script file if the 'src' attribute exists
				if(luaFileXMLNode.isAttributeSet("src")) {
					string filename = luaFileXMLNode.getAttribute("src");
					bool loaded = loadLuaFunctionsFromFile(::lua::L, filename);
					//horizon::script::luaFiles.push_back(filename); // ADD LUA FILE NAME TO LIST OF THEM
					horizon::script::addLuaFile(filename);
				}
				// Parse all lua functions in the body of the XML node
				int n = luaFileXMLNode.nText();
				if(n != 0) {
					string text = luaFileXMLNode.getText();
					bool loaded = loadLuaFunctionsFromString(lua::L, text);
				}
			}
		}
	}

	//*********************************
	//*********************************
	//*********************************
	//*********************************
	// BEGIN TEST 2
	ScriptedObject sc1 = ScriptedObject();
	ScriptedObject sc2 = ScriptedObject(sc1);



	// END TEST 2
	//*********************************
	//*********************************
	//*********************************
	//*********************************



	// Set up Subsystem Nodes, first loop through the assets in the XML model input file
	n = modelInputXMLNode.nChildNode("ASSET");
	for(int i = 0; i < n; i++) {
		// Get the current Asset XML Node and initialize a new Asset with it
		XMLNode currAssetXMLNode = modelInputXMLNode.getChildNode("ASSET", i);		
		Asset* currAsset = new Asset(currAssetXMLNode);
		assetList.push_back(currAsset);
		// Loop through all the of the Subsystems for this Asset
		int m = currAssetXMLNode.nChildNode("SUBSYSTEM");
		for(int j = 0; j < m; j++) {
			// Get the current Subsystem XML Node, and create it using the SubsystemAdapter
			XMLNode currSubsystemXMLNode = currAssetXMLNode.getChildNode("SUBSYSTEM", j);
			Subsystem* currSubsystem = NULL;
			// Check if the type of the Subsystem is scripted, networked, or other
			if(_strcmpi(currSubsystemXMLNode.getAttribute("Type"), "scripted") == 0) {
				// Subsytem is a ScriptedSubsytem, get the LUA functions for the Subsytem
				string subName = currSubsystemXMLNode.getAttribute("Name");
				string initLUAFcn = currSubsystemXMLNode.getAttribute("initLUAFcn");
				string canPerformLUAFcn = currSubsystemXMLNode.getAttribute("canPerformLUAFcn");
				string canExtendLUAFcn = currSubsystemXMLNode.getAttribute("canExtendLUAFcn");
				currSubsystem = new ScriptedSubsystem(/*L, */subName, initLUAFcn, canPerformLUAFcn, canExtendLUAFcn);
			}
			else if(_strcmpi(currSubsystemXMLNode.getAttribute("Type"), "networked") == 0) {
				// Subsytem is a NetworkedSubsytem, get the infor needed to connect to server
				string subName = currSubsystemXMLNode.getAttribute("Name");
				string host = currSubsystemXMLNode.getAttribute("host");
				int port = atoi(currSubsystemXMLNode.getAttribute("port"));
				try {
					currSubsystem = new NetworkedSubsystem(host, port, subName);
				}
				catch(SocketException& se) {
					cout << "Error connecting to subsystem " << subName;
					cout << " on subystem server at " << host << ":" << port << endl;
					cout << se.what() << endl;
					return -1;
				}
				int nKey = currSubsystemXMLNode.nChildNode("KEY");
				for(int k = 0; k < nKey; k++) {
					XMLNode icXMLNode = currSubsystemXMLNode.getChildNode("KEY", k);
					string type = icXMLNode.getAttribute("type");
					string key = icXMLNode.getAttribute("key");
					if(_strcmpi(type.c_str(),"Int")==0) {
						currSubsystem->addKey(StateVarKey<int>(key));
					}
					else if(_strcmpi(type.c_str(),"Float")==0) {
						currSubsystem->addKey(StateVarKey<float>(key));
					}
					else if(_strcmpi(type.c_str(),"Double")==0) {
						currSubsystem->addKey(StateVarKey<double>(key));
					}
					else if(_strcmpi(type.c_str(),"Bool")==0) {
						currSubsystem->addKey(StateVarKey<bool>(key));
					}
					else if(_strcmpi(type.c_str(),"Matrix")==0) {
						currSubsystem->addKey(StateVarKey<Matrix>(key));
					}
					else if(_strcmpi(type.c_str(),"Quat")==0) {
						currSubsystem->addKey(StateVarKey<Quat>(key));
					}
				}
			}
			else {
				// Check if the XMLNode for this subsystem has attribute defaultConstructor="true".
				// If it does, call the create method of the subsystemAdapter that will call the default
				// constructor for the subsystem. Otherwise call the create method and pass in the XMLNode
				// for the current subsystem
				if(currSubsystemXMLNode.isAttributeSet("defaultConstructor")) {
					if(atob(currSubsystemXMLNode.getAttribute("defaultConstructor")))
						currSubsystem = subAdapter.create(currSubsystemXMLNode.getAttribute("Type"));
				}
				else
					currSubsystem = subAdapter.create(currSubsystemXMLNode.getAttribute("Type"), currSubsystemXMLNode);
			}
			// Initialize the new SubsystemNode
			SubsystemNode* currNode = new SubsystemNode(currSubsystem, currAsset);
			if(enableScripting) {
				currNode->enableScriptingSupport();
				currNode->setLuaState(lua::L);
			}
			subNodeList.push_back(currNode);
			// Get the current Subsystem's SubId and set up the maps used to set up the preceeding nodes in each SubsystemNode
			int subId = atoi(currSubsystemXMLNode.getAttribute("SubId"));			
			subsystemNodeXMLNodeMap.insert(make_pair(currNode, currSubsystemXMLNode));
			subsystemNodeMap.insert(make_pair(subId, currNode));			
		}
		// Loop through all the of the constraints for this Asset
		m = currAssetXMLNode.nChildNode("CONSTRAINT");
		for(int j = 0; j < m; j++) {
			// Get the Constraint XMLNode and its corresponding StateVar Node
			XMLNode currConstraintXMLNode = currAssetXMLNode.getChildNode("CONSTRAINT", j);
			XMLNode currStateVarXMLNode = currConstraintXMLNode.getChildNode("STATEVAR");
			int subId = atoi(currConstraintXMLNode.getAttribute("SubId"));
			// Determine the type of the Constraint, create the Generic Constraint
			string type = currStateVarXMLNode.getAttribute("type");
			string key = currStateVarXMLNode.getAttribute("key");
			Constraint* constraint;
			// Integer Constraint
			if(_strcmpi(type.c_str(),"Int")==0) {
				int val = atoi(currConstraintXMLNode.getAttribute("value"));
				StateVarKey<int> svk(key);				
				if(currConstraintXMLNode.isAttributeSet("type"))
					constraint = new SingleAssetGenericConstraint<int>(svk, val, currConstraintXMLNode.getAttribute("type"), "int", i+1);
				else
					constraint = new SingleAssetGenericConstraint<int>(svk, val, "int", i+1);
			}
			// Float Constraint
			else if(_strcmpi(type.c_str(),"Float")==0) {
				float val = atof(currConstraintXMLNode.getAttribute("value"));
				StateVarKey<float> svk(key);				
				if(currConstraintXMLNode.isAttributeSet("type"))
					constraint = new SingleAssetGenericConstraint<float>(svk, val, currConstraintXMLNode.getAttribute("type"), "float", i+1);
				else
					constraint = new SingleAssetGenericConstraint<float>(svk, val, "float", i+1);
			}
			// Double Constraint
			else if(_strcmpi(type.c_str(),"Double")==0) {
				double val = atof(currConstraintXMLNode.getAttribute("value"));
				StateVarKey<double> svk(key);				
				if(currConstraintXMLNode.isAttributeSet("type"))
					constraint = new SingleAssetGenericConstraint<double>(svk, val, currConstraintXMLNode.getAttribute("type"), "double", i+1);
				else
					constraint = new SingleAssetGenericConstraint<double>(svk, val, "double", i+1);
			}
			// Not a generic constriant, pass to ConstraintAdapter
			else {
				constraint = constraintAdapter.create(type);
			}
			if(constraint != NULL) {
				// Get the subsystem node that this constraint constrains and set it
				SubsystemNode* constrainedNode = subsystemNodeMap.find(subId)->second;
				constraint->addConstrianedSubNode(constrainedNode);
				constraintsList.push_back(constraint);
			}
		}
	}

	// Loop through each SubsystemNode so that the preceeding nodes can be added
	for(map<SubsystemNode*, XMLNode>::iterator subNodeIt = subsystemNodeXMLNodeMap.begin(); subNodeIt != subsystemNodeXMLNodeMap.end(); subNodeIt++) {
		// Get the XML Node used to initialize the subsystem contained in the current SubsystemNode
		XMLNode currNode = subNodeIt->second;
		// Loop through each dependency in the XML Node
		int n = currNode.nChildNode("DEPENDENCY");
		for(int i = 0; i < n; i++) {
			// Get the subId of the preceeding node, look up that SubsystemNode in the map, and add it to the current SubsystemNode's preceeding nodes
			XMLNode currDepXMLNode = currNode.getChildNode("DEPENDENCY", i);
			subNodeIt->first->addPreceedingNode(subsystemNodeMap.find(atoi(currDepXMLNode.getAttribute("subID")))->second);
		}
		// Loop through each dependency function in the XML Node
		n = currNode.nChildNode("DEPENDENCY_FCN");
		for(int i = 0; i < n; i++) {
			XMLNode currDepFcnXMLNode = currNode.getChildNode("DEPENDENCY_FCN", i);
			bool scripted = atob(currDepFcnXMLNode.getAttribute("scripted"));
			string key = currDepFcnXMLNode.getAttribute("key");
			varType type = strToVarType(currDepFcnXMLNode.getAttribute("type"));
			string callKey = currDepFcnXMLNode.getAttribute("callKey");
			if(!scripted){
				switch(type) {
					case varType::INT_:
						subNodeIt->first->addDependency(callKey, depAdapter.getIntDependency(key));
						break;
					case DOUBLE_:
						subNodeIt->first->addDependency(callKey, depAdapter.getDoubleDependency(key));
						break;
					case FLOAT_:
						subNodeIt->first->addDependency(callKey, depAdapter.getFloatDependency(key));
						break;
					case BOOL_:
						subNodeIt->first->addDependency(callKey, depAdapter.getBoolDependency(key));
						break;
					case MATRIX_:
						subNodeIt->first->addDependency(callKey, depAdapter.getMatrixDependency(key));
						break;
					case QUAT_:
						subNodeIt->first->addDependency(callKey, depAdapter.getQuatDependency(key));
						break;
				} // end switch
			} // end if
			else {
				switch(type) {
					case varType::INT_:
						subNodeIt->first->addIntScriptedDependency(callKey, key);
						break;
					case DOUBLE_:
						subNodeIt->first->addDoubleScriptedDependency(callKey, key);
						break;
					case FLOAT_:
						subNodeIt->first->addFloatScriptedDependency(callKey, key);
						break;
					case BOOL_:
						subNodeIt->first->addBoolScriptedDependency(callKey, key);
						break;
					case MATRIX_:
						subNodeIt->first->addMatrixScriptedDependency(callKey, key);
						break;
					case QUAT_:
						subNodeIt->first->addQuatScriptedDependency(callKey, key);
						break;
				}
			} // end else (scripted)
		} // end for
	}

	// Finalize the subsystem adapter
	subAdapter.finalize();

	// Finalize the constraint adapter
	constraintAdapter.finalize();
	
	// Load the initial state as defined in the XML input file
	vector<State*>* systemInitialStateList = new vector<State*>;
	bool initialStateSet = setInitialSystemState(modelInputXMLNode, systemInitialStateList);

	// USER - Specify data output parameters
	scheduleDataWriter dataOut(outputPath, true, 2);
	cout << endl;
	
	// Setup schedule evaluator method
	ScheduleEvaluator* systemEvaluator;
	int nSchedEval = modelInputXMLNode.nChildNode("SCHEDULE_EVALUATOR");
	if(nSchedEval != 0) {
		// Create the ScheduleEvaluator based on the XMLNode
		systemEvaluator = horizon::util::adapt::createScheduleEvaluator(lua::L, modelInputXMLNode.getChildNode("SCHEDULE_EVALUATOR"));
	}
	else {
		// If node doesnt exist, pass empty node in and the 
		// default Schedule Evaluator will be used
		systemEvaluator = createScheduleEvaluator(lua::L, XMLNode::emptyXMLNode);
	}

	// Load the environment, subsystems, and constraints into a system to simulate
	System* simSystem = new System(assetList, subNodeList, constraintsList, systemEnvironment);
	
    //------------------------------ RUN THE MAIN ALGORITHM -------------------------------- */ 
	list<systemSchedule*> schedules;
	if(!simSystem->checkForCircularDependencies())
		schedules = systemScheduler->generateSchedules(*simSystem, *systemTasks, *systemInitialStateList, systemEvaluator);
	else
		cout << "System model contains circular dependencies, cannot run simulation" << endl;
    //-------------------------------------------------------------------------------------- */	

	cout << endl << endl;
	cout << "Total Time: " << systemScheduler->totalTimeMs << endl;
	cout << "Pregen Time: " << systemScheduler->pregenTimeMs << endl;
	cout << "Sched Time: " << systemScheduler->schedTimeMs << endl;
	cout << "Accum Time: " << systemScheduler->accumSchedTimeMs << endl;

	// Delete critical section for dependencies
	DeleteCriticalSection(&NodeDependencies::cs);

	// Write error log
	Logger::write("SubFailures.log");
	Logger::writeConstraint("ConFailures.log");

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
	// Delete the possible generated schedules
	for(list<systemSchedule*>::iterator sIt = schedules.begin(); sIt != schedules.end();) {
		delete *sIt;
		schedules.erase(sIt++);
	}	
	getchar();
	return 0;
}
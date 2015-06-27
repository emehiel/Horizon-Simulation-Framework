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
#include "horizon/util/util.h"
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
#include "horizon/constraints/GenericConstraint.h"
#include "horizon/constraints/maxBatteryDOD.h"
#include "horizon/constraints/maxDataBuffer.h"
// USER - Subsystem Usings
using namespace horizon::sub;
// USER - Constraint Usings
using namespace horizon::constraints;

#include "horizon/geom/CloudCover.h"

// Adapter Includes
#include "horizon/util/adapt/SubsystemAdapter.h"
#include "horizon/util/adapt/DependencyAdapter.h"
#include "horizon/util/adapt/EOMSAdapter.h"
using namespace horizon::util::adapt;
using horizon::util::adapt::createEOMSObject;

// Lua Includes
#include <lua.hpp>
#include "horizon/script/register.h"
using namespace luabind;
using namespace horizon::script;

int main( int argc, char *argv[ ], char *envp[ ] )
{
	// Get the input filenames
	string simulationInputFileName = argv[1];
	string targetDeckFileName = argv[2];	
	string modelInputFileName = argv[3];
	string outputPath = argv[4];
	
	/*string ccFile = argv[5];
	// Load the Grid Data
	CloudCover* cc_data = CloudCover::Instance();
	cout << "Loading Percent Cloud Coverage Data... " << endl;
	cc_data->importCloudDataFromTextFile(ccFile);
	cout << "DONE!" << endl;*/

	// Open the LUA state and register Lua/C++ bindings
	lua_State *L = lua_open();
	luaL_openlibs(L);
	open(L);
	register_dependencyLuaState(L);

	///////////////////////GENERIC CONSTRIANT TEST////////////////////////
	/*StateVarKey<int> i_svk1("int1");
	StateVarKey<int> i_svk2("int2");
	StateVarKey<float> f_svk1("float1");
	StateVarKey<float> f_svk2("float2");
	StateVarKey<double> d_svk1("double1");
	StateVarKey<double> d_svk2("double2");

	vector<State*> initialStates;
	initialStates.push_back(new State());
	initialStates.push_back(new State());
	systemSchedule ss(initialStates);

	Task* t1 = new Task(TASK_TYPE_EMPTY,new Target("t1",new Position(POSITION_TYPE_STATIC_LLA,0,0,0)));
	Task* t2 = new Task(TASK_TYPE_EMPTY,new Target("t2",new Position(POSITION_TYPE_STATIC_LLA,0,0,0)));
	vector<const Task*> tasks;
	tasks.push_back(t1);
	tasks.push_back(t2);
	systemSchedule ss2(&ss,tasks,0);
	State* state1 = ss2.getEndStates().at(0);
	State* state2 = ss2.getEndStates().at(1);

	//State* state1 = new State();
	state1->setEventStart(0);
	state1->setTaskStart(5);
	state1->setTaskEnd(10);
	state1->setEventEnd(20);
	//State* state2 = new State();
	state2->setEventStart(0);
	state2->setTaskStart(5);
	state2->setTaskEnd(10);
	state2->setEventEnd(20);

	state1->addValue(i_svk1, make_pair(1, 1));
	state1->addValue(i_svk1, make_pair(3, 3));
	state1->addValue(i_svk1, make_pair(5, 5));
	state1->addValue(i_svk1, make_pair(7, 7));
	state1->addValue(i_svk1, make_pair(9, 5));
	state1->addValue(i_svk1, make_pair(11, 3));
	state1->addValue(i_svk1, make_pair(13, 1));

	state2->addValue(i_svk2, make_pair(1, 1));
	state2->addValue(i_svk2, make_pair(3, 3));
	state2->addValue(i_svk2, make_pair(5, 5));
	state2->addValue(i_svk2, make_pair(7, 7));
	state2->addValue(i_svk2, make_pair(9, 5));
	state2->addValue(i_svk2, make_pair(11, 3));
	state2->addValue(i_svk2, make_pair(13, 1));

	state1->addValue(f_svk1, make_pair(1, 1.1));
	state1->addValue(f_svk1, make_pair(3, 1.3));
	state1->addValue(f_svk1, make_pair(5, 1.5));
	state1->addValue(f_svk1, make_pair(7, 1.7));
	state1->addValue(f_svk1, make_pair(9, 1.5));
	state1->addValue(f_svk1, make_pair(11, 1.3));
	state1->addValue(f_svk1, make_pair(13, 1.1));

	state2->addValue(f_svk2, make_pair(1, 1.1));
	state2->addValue(f_svk2, make_pair(3, 1.3));
	state2->addValue(f_svk2, make_pair(5, 1.5));
	state2->addValue(f_svk2, make_pair(7, 1.7));
	state2->addValue(f_svk2, make_pair(9, 1.5));
	state2->addValue(f_svk2, make_pair(11, 1.3));
	state2->addValue(f_svk2, make_pair(13, 1.1));

	state1->addValue(d_svk1, make_pair(1, 1.1));
	state1->addValue(d_svk1, make_pair(3, 1.3));
	state1->addValue(d_svk1, make_pair(5, 1.5));
	state1->addValue(d_svk1, make_pair(7, 1.7));
	state1->addValue(d_svk1, make_pair(9, 1.5));
	state1->addValue(d_svk1, make_pair(11, 1.3));
	state1->addValue(d_svk1, make_pair(13, 1.1));

	state2->addValue(d_svk2, make_pair(1, 1.1));
	state2->addValue(d_svk2, make_pair(3, 1.3));
	state2->addValue(d_svk2, make_pair(5, 1.5));
	state2->addValue(d_svk2, make_pair(7, 1.7));
	state2->addValue(d_svk2, make_pair(9, 1.5));
	state2->addValue(d_svk2, make_pair(11, 1.3));
	state2->addValue(d_svk2, make_pair(13, 1.1));


	Constraint* i_c_1_h_f = new SingleAssetGenericConstraint<int>(i_svk1, 6, FAIL_IF_HIGHER,1);
	Constraint* i_c_2_h_t = new SingleAssetGenericConstraint<int>(i_svk1, 8, FAIL_IF_HIGHER,1);
	Constraint* i_c_3_l_f = new SingleAssetGenericConstraint<int>(i_svk1, 2, FAIL_IF_LOWER,1);
	Constraint* i_c_4_l_t = new SingleAssetGenericConstraint<int>(i_svk1, 0, FAIL_IF_LOWER,1);

	//Constraint* f_c_5_h_f = new GenericConstraint<float>(i_svk1, 6, FAIL_IF_HIGHER);
	//Constraint* f_c__h_t = new GenericConstraint<float>(i_svk1, 8, FAIL_IF_HIGHER);
	//Constraint* f_c_3_l_f = new GenericConstraint<float>(i_svk1, 2, FAIL_IF_LOWER);
	//Constraint* f_c_4_l_t = new GenericConstraint<float>(i_svk1, 0, FAIL_IF_LOWER);

	bool b1 = i_c_1_h_f->accepts(&ss2);//F
	bool b2 = i_c_2_h_t->accepts(&ss2);//T
	bool b3 = i_c_3_l_f->accepts(&ss2);//F
	bool b4 = i_c_4_l_t->accepts(&ss2);//T*/


	//////////////////////////////////////////////////////////////////////
	
	/*vector<State*> endStates;
	endStates.push_back(state1);
	endStates.push_back(state2);
	Dependencies* deps = Dependencies::Instance();
	deps->updateStates(endStates);*/
	//////////////////////////////LUA TESTING FOR END STATE////////////////
	const char * test1 = {
		"function test1(num)\n"
		"local s1 = getState(num)\n"
		"return s1\n"
		"end"
	};

	const char * test2 = {
		"function test2()\n"
		"svk = dStateVarKey('aaa')\n"
		"p = dPair(1,1)\n"
		"s = State()\n"
		"s:addValue(svk,dPair(1,1))\n"
		//"local p1 = s:getProfile(svk)\n"
		"local p1 = dProfile()\n"
		"return p1\n"
		"end"
	};

	const char * test3 = {
		"function test3()\n"
		"pair1 = bPair(1,true)\n"
		"io.write(pair1.first, '--', tostring(pair1.second))\n"
		"return pair1\n"
		"end"
	};
	
	//luaL_dostring(L, test1);
	//State* s1 = call_function<State*> (L, "test1", 1);
	//State* s2 = call_function<State*> (L, "test1", 2);
	//luaL_dostring(L, test2);
	//Profile<double> p1 = call_function<Profile<double>> (L, "test2");
	/*luaL_dostring(L, test3);
	State* s1;
	try {
		pair<double, bool> p1 = call_function<pair<double, bool>> (L, "test3");
		int i = 1+1;
	}
	catch(const luabind::error& err) {
		cerr << err.what() << endl;
	}*/



	///////////////////////////////////////////////////////////////////////




	// Find the main input node from the XML input files
	XMLNode simulationInputXMLNode = XMLNode::openFileHelper(simulationInputFileName.c_str() , "SCENARIO");
	cout << endl << "EXECUITING SCENARIO: " << simulationInputXMLNode.getAttribute("scenarioName") << endl;
	
	// Load the simulation parameters from the XML simulation input file
	XMLNode simParametersXMLNode = simulationInputXMLNode.getChildNode("SIMULATION_PARAMETERS");
	bool simParamsLoaded = simParams::LoadSimParameters(simParametersXMLNode);

	// Load the scheduler parameters defined in the XML simulation input file
	XMLNode schedParametersXMLNode = simulationInputXMLNode.getChildNode("SCHEDULER_PARAMETERS");
	Scheduler* systemScheduler = new Scheduler();
	bool schedParamsLoaded = loadSchedulerParams(schedParametersXMLNode, systemScheduler);

	// Load the target deck into the targets list from the XML target deck input file
	XMLNode targetDeckXMLNode = XMLNode::openFileHelper(targetDeckFileName.c_str(),"TARGETDECK");
	vector<const Task*>* systemTasks = new vector<const Task*>;
	bool targetsLoaded = loadTargetsIntoTaskList(targetDeckXMLNode, systemTasks);

	// Load the environment
	Environment* systemEnvironment = new Environment(new Sun(true));

	// Find the main model node from the XML model input file
	XMLNode modelInputXMLNode = XMLNode::openFileHelper(modelInputFileName.c_str() , "MODEL");

	// Initialize the SubsystemAdapter
	SubsystemAdapter subAdapter;
	subAdapter.initialize();

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
			if(luaXMLNode.getAttribute("enableScripting") != "false")
				enableScripting = true;
		}
		// Loop through all the of the file nodes
		int m = luaXMLNode.nChildNode("LUA_FILE");
		for(int i = 0; i < m; i++) {
			XMLNode luaFileXMLNode = luaXMLNode.getChildNode("LUA_FILE", i);
			// If scripting is enabled, parse the script file designated by the src attribute
			if(enableScripting) {
				string filename = luaFileXMLNode.getAttribute("src");
				bool loaded = loadLuaFunctionsFromFile(L, filename);
			}
			// Parse all lua functions in the body of the XML node
			int n = luaFileXMLNode.nText();
			if(n != 0) {
				string text = luaFileXMLNode.getText();
				bool loaded = loadLuaFunctionsFromString(L, text);
			}
		}
	}

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
			Subsystem* currSubsystem = subAdapter.create(currSubsystemXMLNode.getAttribute("Type"), currSubsystemXMLNode);
			// Initialize the new SubsystemNode
			SubsystemNode* currNode = new SubsystemNode(currSubsystem, currAsset);
			if(enableScripting) {
				currNode->enableScriptingSupport();
				currNode->setLuaState(L);
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
			int subId = atoi(currConstraintXMLNode.getAttribute("subId"));
			// Determine the type of the Constraint, create the Generic Constraint
			string type = currStateVarXMLNode.getAttribute("type");
			string key = currStateVarXMLNode.getAttribute("key");
			Constraint* constraint;
			// Integer Constraint
			if(strcmp(type.c_str(),"Int")==0) {
				int val = atoi(currConstraintXMLNode.getAttribute("value"));
				StateVarKey<int> svk(key);				
				if(currConstraintXMLNode.isAttributeSet("type"))
					constraint = new SingleAssetGenericConstraint<int>(svk, val, currConstraintXMLNode.getAttribute("type"), i+1);
				else
					constraint = new SingleAssetGenericConstraint<int>(svk, val, i+1);
			}
			// Float Constraint
			else if(strcmp(type.c_str(),"Float")==0) {
				float val = atof(currConstraintXMLNode.getAttribute("value"));
				StateVarKey<float> svk(key);				
				if(currConstraintXMLNode.isAttributeSet("type"))
					constraint = new SingleAssetGenericConstraint<float>(svk, val, currConstraintXMLNode.getAttribute("type"), i+1);
				else
					constraint = new SingleAssetGenericConstraint<float>(svk, val, i+1);
			}
			// Double Constraint
			else if(strcmp(type.c_str(),"Double")==0) {
				double val = atof(currConstraintXMLNode.getAttribute("value"));
				StateVarKey<double> svk(key);				
				if(currConstraintXMLNode.isAttributeSet("type"))
					constraint = new SingleAssetGenericConstraint<double>(svk, val, currConstraintXMLNode.getAttribute("type"), i+1);
				else
					constraint = new SingleAssetGenericConstraint<double>(svk, val, i+1);
			}
			else {
				constraint = NULL;
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
			bool scripted = toBool(currDepFcnXMLNode.getAttribute("scripted"));
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
	
	// Load the initial state as defined in the XML input file
	vector<State*>* systemInitialStateList = new vector<State*>;
	bool initialStateSet = setInitialSystemState(modelInputXMLNode, systemInitialStateList);

	// USER - Specify data output parameters
	scheduleDataWriter dataOut(outputPath, true, 2);
	
	// USER - Specify schedule evaluator method
	ScheduleEvaluator* systemEvaluator = new TargetValueScheduleEvaluator();

	// Load the environment, subsystems, and constraints into a system to simulate
	System* simSystem = new System(assetList, subNodeList, constraintsList, systemEnvironment);

	//////////////////////////////////////////////////////////////////////////////
	///////////////////////////// LUA Testing ////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	cout << endl << "----------------Begin Lua Testing Output----------------" << endl;
	// Test Pairs
	call_function<void> (L, "test_dPair");
	call_function<void> (L, "test_fPair");
	call_function<void> (L, "test_iPair");
	call_function<void> (L, "test_bPair");
	call_function<void> (L, "test_mPair");
	call_function<void> (L, "test_qPair");

	// Test StateVarKeys
	call_function<void> (L, "test_dStateVarKey");
	call_function<void> (L, "test_fStateVarKey");
	call_function<void> (L, "test_iStateVarKey");
	call_function<void> (L, "test_bStateVarKey");
	call_function<void> (L, "test_mStateVarKey");
	call_function<void> (L, "test_qStateVarKey");

	// Test simParams
	call_function<void> (L, "test_simParams");

	// Test matrixIndex
	call_function<void> (L, "test_matrixIndex");

	// Test Matrix
	call_function<void> (L, "test_matrix");

	//Test Quat
	call_function<void> (L, "test_Quat");

	//Test State
	call_function<void> (L, "test_State");


	cout << endl << "----------------End Lua Testing Output----------------" << endl;
	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////// End LUA Testing /////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
 	
    //------------------------------ RUN THE MAIN ALGORITHM -------------------------------- */ 
	list<systemSchedule*> schedules;
	if(!simSystem->checkForCircularDependencies())
		schedules = systemScheduler->generateSchedules(*simSystem, *systemTasks, *systemInitialStateList, systemEvaluator);
	else
		cout << "System model contains circular dependencies, cannot run simulation" << endl;
    //-------------------------------------------------------------------------------------- */	

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
	//delete access;
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
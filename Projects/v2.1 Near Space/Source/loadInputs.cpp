#include "loadInputs.h"

bool setInitialSystemState(vector<XMLNode>& subsystemXMLNode, vector<State*>* initialStateList)
{
	/*
	cout << endl << "Setting initial state... " << endl;
	StateVarKey<Matrix> p1(STATEVARNAME_BALLPOS);
	StateVarKey<Matrix> v1(STATEVARNAME_BALLVEL);
	StateVarKey<Matrix> g1(STATEVARNAME_BALLGEOM);
	
	State* state1 = new State();
	Matrix initPos = atom(subsystemXMLNode.front().getAttribute("initPos"));
	Matrix initVel = atom(subsystemXMLNode.front().getAttribute("initVel"));
	Matrix geomInfo = atom(subsystemXMLNode.front().getAttribute("geomInfo"));


	state1->addValue(p1, make_pair(0, initPos));
	state1->addValue(v1, make_pair(0, initVel));
	state1->addValue(g1, make_pair(0, geomInfo));
	initialStateList->push_back(state1);
	

	
	cout << endl << "Setting initial state... " << endl;
	StateVarKey<Matrix> p2(STATEVARNAME_SSPOS);
	StateVarKey<Matrix> v2(STATEVARNAME_SSVEL);
	StateVarKey<Matrix> g2(STATEVARNAME_SSGEOM);

	
	State* state2 = new State();
	Matrix initPos = atom(subsystemXMLNode.front().getAttribute("initPos"));
	Matrix initVel = atom(subsystemXMLNode.front().getAttribute("initVel"));
	Matrix ssGeomInfo = atom(subsystemXMLNode.front().getAttribute("ssGeomInfo"));

	state2->addValue(p2, make_pair(0, initPos));
	state2->addValue(v2, make_pair(0, initVel));
	state2->addValue(g2, make_pair(0, ssGeomInfo));
	initialStateList->push_back(state2);
	*/
	/**/
	cout << endl << "Setting initial state... " << endl;
	StateVarKey<Matrix> p3(STATEVARNAME_BLIMPPOS);
	StateVarKey<Matrix> v3(STATEVARNAME_BLIMPVEL);
	StateVarKey<double> t3(STATEVARNAME_BLIMPTHRUST);
	StateVarKey<double> gv3(STATEVARNAME_AIRVOL);

	State* state3 = new State();
	Matrix initPos = atom(subsystemXMLNode.front().getAttribute("initPos"));
	Matrix initVel = atom(subsystemXMLNode.front().getAttribute("initVel"));
	double initThrust = atof(subsystemXMLNode.front().getAttribute("initThrust"));
	

	state3->addValue(p3, make_pair(0, initPos));
	state3->addValue(v3, make_pair(0, initVel));
	state3->addValue(t3, make_pair(0, initThrust));
	initialStateList->push_back(state3);
	

	/*
	
	StateVarKey<double> s1(STATEVARNAME_DATABUFFERRATIO);
	StateVarKey<double> p1(STATEVARNAME_DOD);
	StateVarKey<double> c1(STATEVARNAME_DATARATE);
	StateVarKey<bool> eo3(STATEVARNAME_EOON);

	State* state1 = new State();
	state1->addValue(s1, make_pair(0, 0));
	state1->addValue(p1, make_pair(0, 0));
	state1->addValue(c1, make_pair(0, 0));
	state1->addValue(eo3, make_pair(0, false));
	initialStateList->push_back(state1);
	*/
	cout << "  State 1: 4 initial values set" << endl;

	return true;
}

bool loadSchedulerParams(XMLNode& simParametersNodeXML, Scheduler* scheduler)
{
	cout << endl << "Loading scheduler parameters... " << endl;
	scheduler->setMaxNumSchedules(atoi(simParametersNodeXML.getAttribute("maxSchedules")));
	cout << "  Maximum number of schedules: " << atoi(simParametersNodeXML.getAttribute("maxSchedules")) << endl;
	scheduler->setSchedCropTo(atoi(simParametersNodeXML.getAttribute("numCrop")));
	cout << "  Number of schedules to crop to: " << atoi(simParametersNodeXML.getAttribute("numCrop")) << endl;
	scheduler->setStepLength(atof(simParametersNodeXML.getAttribute("simStep")));
	cout << "  Scheduler time step: " << atof(simParametersNodeXML.getAttribute("simStep")) << " seconds" << endl;
	return true;
}

const bool loadTargetsIntoTaskList(XMLNode& targDeckFileName, vector<const Task*>* tasks)
{
	cout << endl << "Loading target deck..." << endl;
	int n = targDeckFileName.nChildNode("TARGET"); // this gets the number of "TARGET" tags:

	// this creates n targets:
	int i = 0, iterator = 0, nComm = 0, nImage = 0, nUnk = 0;
	XMLNode targetNode;
	for(; i<n; i++) {
		targetNode = targDeckFileName.getChildNode("TARGET", &iterator);

		// USER - Add other target properties in Target Constructor using XML input
		string type = targetNode.getAttribute("TargetType");

		if (type == TARGET_TYPE_EARTH_FACILITY) {
			tasks->push_back(new Task(TASK_TYPE_COMM, new Target(targetNode), 1000));
			nComm++;
		}
		else if (type == TARGET_TYPE_EARTH_LOCATION) {
			tasks->push_back(new Task(TASK_TYPE_IMAGE, new Target(targetNode), 1000));
			nImage++;
		}
		else {
			cout << "  ERROR: Incorrect target type: " << type << endl;
			nUnk++;
		}
	}
	cout << "  Comm targets loaded: " << nComm << endl;
	cout << "  Imaging targets loaded: " << nImage << endl;
	cout << "  Targets with bad target type: " << nUnk << endl;
	
	if(nUnk>0)
		return false;
	else
		return true;
}

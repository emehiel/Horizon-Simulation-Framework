#include "loadInputs.h"

bool setInitialSystemState(vector<XMLNode>& subsystemXMLNode, vector<SubsystemNode*>& subNodeList, vector<State*>* initialStateList)
{
	// TODO:  Do the smae sort of thing here as in scheduleDataWriter::writeStateData since
	// if this function is called after subsystems are instanciated, teh subsystem keys
	// will be definde AND added
	// USER:  Initialize ALL state variables here.  This is critical to batch jobs
	cout << endl << "Setting initial state... " << endl;
	State* state1 = new State();
	double sTime = simParams::SIMSTART_SECONDS();
	unsigned count = 0;

	// Iterate through each SubsystemNode and write out its data
	for(vector<SubsystemNode*>::const_iterator subNodeIt = subNodeList.begin(); subNodeIt != subNodeList.end(); subNodeIt++)
	{
		// Get the Subsystem corresponding to the SubsystemNode
		const Subsystem* subsystem = (*subNodeIt)->getSubsystem();
		// Get all of the StateVarKeys from the subsystem
		
		vector< StateVarKey<int> >		ikeys = subsystem->getIkeys();
		vector< StateVarKey<double> >	dkeys = subsystem->getDkeys();
		vector< StateVarKey<float> >	fkeys = subsystem->getFkeys();
		vector< StateVarKey<bool> >		bkeys = subsystem->getBkeys();
		vector< StateVarKey<Matrix> >	mkeys = subsystem->getMkeys();
		vector< StateVarKey<Quat> >		qkeys = subsystem->getQkeys();

		// Iterate through all the vectors of StateVarKeys, get the FullProfile for the corresponding State data and output it
		if(!ikeys.empty()){
			for(vector<StateVarKey<int>>::const_iterator keyIt = ikeys.begin(); keyIt != ikeys.end(); keyIt++){
				count++;
				if (subsystemXMLNode.back().isAttributeSet(keyIt->getNameString().c_str())){
					int temp = atoi(subsystemXMLNode.back().getAttribute(keyIt->getNameString().c_str()));
					state1->addValue(*keyIt, make_pair(sTime, temp));
				}
				else{
					cout << "Initial State Not Found: " << keyIt->getNameString() << endl;
					cout << "Setting Default Value as INT: 0" << endl;
					state1->addValue(*keyIt, make_pair(sTime, 0));
				}
			}
		}
		if(!dkeys.empty())
		{
			for(vector<StateVarKey<double>>::const_iterator keyIt = dkeys.begin(); keyIt != dkeys.end(); keyIt++)
			{				
				count++;
				if (subsystemXMLNode.back().isAttributeSet(keyIt->getNameString().c_str())){
					double temp = atof(subsystemXMLNode.back().getAttribute(keyIt->getNameString().c_str()));
					state1->addValue(*keyIt, make_pair(sTime, temp));
					cout << "Initial State for state variable: " << keyIt->getNameString() << " set to: " << temp << endl; 
				}
				else{
					cout << "Initial State Not Found: " << keyIt->getNameString() << endl;
					cout << "Setting Default Value as DOUBLE: 0.0" << endl;
					state1->addValue(*keyIt, make_pair(sTime, 0.0));
				}
			}
		}
		if(!fkeys.empty())
		{
			for(vector<StateVarKey<float>>::const_iterator keyIt = fkeys.begin(); keyIt != fkeys.end(); keyIt++)
			{
				count++;
				if (subsystemXMLNode.back().isAttributeSet(keyIt->getNameString().c_str())){
					float temp = atof(subsystemXMLNode.back().getAttribute(keyIt->getNameString().c_str()));
					state1->addValue(*keyIt, make_pair(sTime, temp));
					cout << "Initial State for state variable: " << keyIt->getNameString() << " set to: " << temp << endl; 
				}
				else{
					cout << "Initial State Not Found: " << keyIt->getNameString() << endl;
					cout << "Setting Default Value as FLOAT: 0.0" << endl;
					state1->addValue(*keyIt, make_pair(sTime, 0.0));
				}
			}
		}
		if(!bkeys.empty())
		{
			for(vector<StateVarKey<bool>>::const_iterator keyIt = bkeys.begin(); keyIt != bkeys.end(); keyIt++){
				count++;
				if (subsystemXMLNode.back().isAttributeSet(keyIt->getNameString().c_str()))
				{
					bool temp = bool(atoi(subsystemXMLNode.back().getAttribute(keyIt->getNameString().c_str())));
					state1->addValue(*keyIt, make_pair(sTime, temp));
					cout << "Initial State for state variable: " << keyIt->getNameString() << " set to: " << temp << endl; 
				}
				else
				{
					cout << "Initial State Not Found: " << keyIt->getNameString() << endl;
					cout << "Setting Default Value as BOOL: false" << endl;
					state1->addValue(*keyIt, make_pair(sTime, false));
				}
			}
		}
		if(!mkeys.empty())
		{
			for(vector<StateVarKey<Matrix>>::const_iterator keyIt = mkeys.begin(); keyIt != mkeys.end(); keyIt++)
			{
				count++;
				if (subsystemXMLNode.back().isAttributeSet(keyIt->getNameString().c_str())){
					Matrix temp = atom(subsystemXMLNode.back().getAttribute(keyIt->getNameString().c_str()));
					state1->addValue(*keyIt, make_pair(sTime, temp));
					cout << "Initial State for state variable: " << keyIt->getNameString() << " set to: " << temp << endl; 
				}
				else{
					cout << "Initial State Not Found: " << keyIt->getNameString() << endl;
					cout << "Setting Default Value as MATRIX: [0]" << endl;
					state1->addValue(*keyIt, make_pair(sTime, Matrix(1)));
				}
			}
		}
		if(!qkeys.empty())
		{
			for(vector<StateVarKey<Quat>>::const_iterator keyIt = qkeys.begin(); keyIt != qkeys.end(); keyIt++)
			{
				count++;
				if (subsystemXMLNode.back().isAttributeSet(keyIt->getNameString().c_str())){
					Quat temp = atoq(subsystemXMLNode.back().getAttribute(keyIt->getNameString().c_str()));
					state1->addValue(*keyIt, make_pair(sTime, temp));
				}
				else{
					cout << "Initial State Not Found: " << keyIt->getNameString() << endl;
					cout << "Setting Default Value as QUAT: 0" << endl;
					state1->addValue(*keyIt, make_pair(sTime, Quat(0.0,0.0,0.0,0.0)));
				}
			}
		}
	}

/*	
	StateVarKey<double> s1(STATEVARNAME_BITSONBUFFER);
	StateVarKey<double> p1(STATEVARNAME_DOD);
	StateVarKey<double> c1(STATEVARNAME_DATARATE);
	StateVarKey<bool> eo3(STATEVARNAME_EOON);

	state1->addValue(s1, make_pair(sTime, atof(subsystemXMLNode.back().getAttribute(STATEVARNAME_BITSONBUFFER))));
	state1->addValue(p1, make_pair(sTime, atof(subsystemXMLNode.back().getAttribute(STATEVARNAME_DOD))));
	state1->addValue(c1, make_pair(sTime, atof(subsystemXMLNode.back().getAttribute(STATEVARNAME_DATARATE))));
	state1->addValue(eo3, make_pair(sTime,bool(atoi(subsystemXMLNode.back().getAttribute(STATEVARNAME_EOON)))));
*/
	initialStateList->push_back(state1);
	cout << "  Initial State: " << count << " initial values set" << endl;

	return true;
}
/*
bool loadSchedulerParams(Scheduler* scheduler)
{
	schedParams::loadSchedulerParams(simParametersNodeXML);
	scheduler->setMaxNumSchedules(atoi(simParametersNodeXML.getAttribute("maxSchedules")));
	cout << "  Maximum number of schedules: " << atoi(simParametersNodeXML.getAttribute("maxSchedules")) << endl;
	scheduler->setSchedCropTo(atoi(simParametersNodeXML.getAttribute("numCrop")));
	cout << "  Number of schedules to crop to: " << atoi(simParametersNodeXML.getAttribute("numCrop")) << endl;
	scheduler->setStepLength(atof(simParametersNodeXML.getAttribute("simStep")));
	cout << "  Scheduler time step: " << atof(simParametersNodeXML.getAttribute("simStep")) << " seconds" << endl;
	return true;
}
*/
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
		size_t numTimes = 0;
		if(targetNode.isAttributeSet("numTimesCollectTarget"))
			numTimes = atoi(targetNode.getAttribute("numTimesCollectTarget"));
		else
			numTimes = 9999999; // if the value is not set, assume something large

		if (type == TARGET_TYPE_EARTH_FACILITY) {
			tasks->push_back(new Task(TASK_TYPE_COMM, new Target(targetNode), numTimes));
			nComm++;
		}
		else if (type == TARGET_TYPE_EARTH_LOCATION) {
			tasks->push_back(new Task(TASK_TYPE_IMAGE, new Target(targetNode), numTimes));
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

#include "loadInputs.h"

bool setInitialSystemState(XMLNode& modelInputXMLNode, vector<State*>* initialStateList)
{
	cout << endl << "Setting initial state... " << endl;
	// Set up Subsystem Nodes, first loop through the assets in the XML model input file
	int n = modelInputXMLNode.nChildNode("ASSET");
	for(int i = 0; i < n; i++) {
		// Get the current Asset XML Node, create a new State corresponding to this Asset
		XMLNode currAssetXMLNode = modelInputXMLNode.getChildNode("ASSET", i);
		State* state = new State();
		// Loop through all the of the Subsystems for this Asset
		int m = currAssetXMLNode.nChildNode("SUBSYSTEM");
		for(int j = 0; j < m; j++) {
			// Get the current Subsystem XML Node
			XMLNode currSubsystemXMLNode = currAssetXMLNode.getChildNode("SUBSYSTEM", j);
			// Loop through all the IC's in the current Subsystem XML Node, determine their type
			// create the StateVarKey for it, and add the IC to the initial state
			int p = currSubsystemXMLNode.nChildNode("IC");
			for(int k = 0; k < p; k++) {
				XMLNode icXMLNode = currSubsystemXMLNode.getChildNode("IC", k);
				string type = icXMLNode.getAttribute("type");
				string key = icXMLNode.getAttribute("key");
				if(strcmp(type.c_str(),"Int")==0) {
					int val = atoi(icXMLNode.getAttribute("value"));
					StateVarKey<int> svk(key);
					state->addValue(svk, make_pair(simParams::SIMSTART_SECONDS(), val));
				}
				else if(strcmp(type.c_str(),"Float")==0) {
					float val = atof(icXMLNode.getAttribute("value"));
					StateVarKey<float> svk(key);
					state->addValue(svk, make_pair(simParams::SIMSTART_SECONDS(), val));
				}
				else if(strcmp(type.c_str(),"Double")==0) {
					double val = atof(icXMLNode.getAttribute("value"));
					StateVarKey<double> svk(key);
					state->addValue(svk, make_pair(simParams::SIMSTART_SECONDS(), val));
				}
				else if(strcmp(type.c_str(),"Bool")==0) {
					string val = icXMLNode.getAttribute("value");
					bool val_ = false;
					if(strcmp(val.c_str(),"True")==0 || strcmp(val.c_str(),"1")==0)
						val_ = true;
					StateVarKey<bool> svk(key);
					state->addValue(svk, make_pair(simParams::SIMSTART_SECONDS(), val_));
				}
				else if(strcmp(type.c_str(),"Matrix")==0) {
					Matrix val = atom(icXMLNode.getAttribute("value"));
					StateVarKey<Matrix> svk(key);
					state->addValue(svk, make_pair(simParams::SIMSTART_SECONDS(), val));
				}
				else if(strcmp(type.c_str(),"Quat")==0) {
					// Quaternions still need an initializer from a string, like Matrices
				}
				else {
					return false;
				}
			}			
		}
		initialStateList->push_back(state);
	}
	cout << "  Initial states set" << endl;
	return true;
}

bool loadSchedulerParams(XMLNode& schedParametersNodeXML, Scheduler* scheduler)
{
	cout << endl << "Loading scheduler parameters... " << endl;
	scheduler->setMaxNumSchedules(atoi(schedParametersNodeXML.getAttribute("MaxSchedules")));
	cout << "  Maximum number of schedules: " << atoi(schedParametersNodeXML.getAttribute("MaxSchedules")) << endl;
	scheduler->setSchedCropTo(atoi(schedParametersNodeXML.getAttribute("CropTo")));
	cout << "  Number of schedules to crop to: " << atoi(schedParametersNodeXML.getAttribute("CropTo")) << endl;
	scheduler->setStepLength(atof(schedParametersNodeXML.getAttribute("SimStep")));
	cout << "  Scheduler time step: " << atof(schedParametersNodeXML.getAttribute("SimStep")) << " seconds" << endl;
	return true;
}

const bool loadTargetsIntoTaskList(XMLNode& targetDeckXMLNode, vector<const Task*>* tasks)
{
	cout << endl << "Loading target deck..." << endl;
	int n = targetDeckXMLNode.nChildNode("TARGET"); // this gets the number of "TARGET" tags:
	bool allLoaded = true;

	XMLNode targetNode;
	for(int i = 0; i < n; i++) {
		targetNode = targetDeckXMLNode.getChildNode("TARGET", i);
		string targetType, taskType;

		if(targetNode.isAttributeSet("TaskType"))
			taskType = targetNode.getAttribute("TaskType");
		else {
			allLoaded = false;
			continue;
		}
		if(targetNode.isAttributeSet("MaxTimes"))
			tasks->push_back(new Task(taskType, new Target(targetNode), atoi(targetNode.getAttribute("MaxTimes"))));
		else
			tasks->push_back(new Task(taskType, new Target(targetNode)));

		// USER - Add other target properties in Target Constructor using XML input
	}
	cout << "Number of Targets Loaded: " << n << endl;
	
	return allLoaded;
}

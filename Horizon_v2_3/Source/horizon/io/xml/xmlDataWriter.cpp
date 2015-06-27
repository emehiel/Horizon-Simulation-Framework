#include "xmlDataWriter.h"

namespace horizon {
namespace io {
namespace xml {

xmlDataWriter::xmlDataWriter() {}

xmlDataWriter::~xmlDataWriter() {}

bool xmlDataWriter::writeSchedule(systemSchedule* schedule, System* system, const string filename) {
	//get the vector of end states
	vector<const State*> endStates;
	vector<assetSchedule*> assetScheds = schedule->getAssetScheds();	
	for(vector<assetSchedule*>::const_iterator assIt = assetScheds.begin(); assIt != assetScheds.end(); assIt++) {
		endStates.push_back((*assIt)->getLastState());
	}

	xmlNode rootNode("Base");	
	const vector<SubsystemNode*>& subSystemNodes = system->getSubsystemNodes();

	set<const Subsystem*> subs;
	for(vector<SubsystemNode*>::const_iterator subNodeIt = subSystemNodes.begin(); subNodeIt != subSystemNodes.end(); subNodeIt++) {
		subs.insert((*subNodeIt)->getSubsystem());
	}

	for(set<const Subsystem*>::const_iterator subIt = subs.begin(); subIt != subs.end(); subIt++)
	{		
		vector< StateVarKey<int> >		ikeys = (*subIt)->getIkeys();
		vector< StateVarKey<double> >	dkeys = (*subIt)->getDkeys();
		vector< StateVarKey<float> >	fkeys = (*subIt)->getFkeys();
		vector< StateVarKey<bool> >		bkeys = (*subIt)->getBkeys();
		vector< StateVarKey<Matrix> >	mkeys = (*subIt)->getMkeys();
		vector< StateVarKey<Quat> >		qkeys = (*subIt)->getQkeys();

		//string outputFileName = schedulePath + "\\" + assetName + "_" + subsystem->getName() + "StateData.txt";
		if(!ikeys.empty()) {
			for(vector<StateVarKey<int>>::const_iterator keyIt = ikeys.begin(); keyIt != ikeys.end(); keyIt++) {
				SubsystemDataNode dataNode(NUMERIC, keyIt->getNameString());
				vector<const State*>::const_iterator stateIt = endStates.begin();
				for(int j = 1; stateIt != endStates.end(); stateIt++, j++) {
					stringstream assetName;
					assetName << "Asset" << j;
					AssetNode assetNode(assetName.str());
					map<double,int> stateData = (*stateIt)->getFullProfile(*keyIt).getProfileData();
					assetNode.addNumericData(stateData);
					dataNode.addChildNode(assetNode);
				}
				rootNode.addChildNode(dataNode);
			}
		}
		if(!dkeys.empty()) {
			for(vector<StateVarKey<double>>::const_iterator keyIt = dkeys.begin(); keyIt != dkeys.end(); keyIt++) {				
				SubsystemDataNode dataNode(NUMERIC, keyIt->getNameString());
				vector<const State*>::const_iterator stateIt = endStates.begin();
				for(int j = 1; stateIt != endStates.end(); stateIt++, j++) {
					stringstream assetName;
					assetName << "Asset" << j;
					AssetNode assetNode(assetName.str());
					map<double,double> stateData = (*stateIt)->getFullProfile(*keyIt).getProfileData();
					assetNode.addNumericData(stateData);
					dataNode.addChildNode(assetNode);
				}
				rootNode.addChildNode(dataNode);
			}
		}
		if(!fkeys.empty()) {
			for(vector<StateVarKey<float>>::const_iterator keyIt = fkeys.begin(); keyIt != fkeys.end(); keyIt++) {
				SubsystemDataNode dataNode(NUMERIC, keyIt->getNameString());
				vector<const State*>::const_iterator stateIt = endStates.begin();
				for(int j = 1; stateIt != endStates.end(); stateIt++, j++) {
					stringstream assetName;
					assetName << "Asset" << j;
					AssetNode assetNode(assetName.str());
					map<double,float> stateData = (*stateIt)->getFullProfile(*keyIt).getProfileData();
					assetNode.addNumericData(stateData);
					dataNode.addChildNode(assetNode);
				}
				rootNode.addChildNode(dataNode);
			}
		}
		if(!bkeys.empty()) {
			for(vector<StateVarKey<bool>>::const_iterator keyIt = bkeys.begin(); keyIt != bkeys.end(); keyIt++) {		
				SubsystemDataNode dataNode(NUMERIC, keyIt->getNameString());
				vector<const State*>::const_iterator stateIt = endStates.begin();
				for(int j = 1; stateIt != endStates.end(); stateIt++, j++) {
					stringstream assetName;
					assetName << "Asset" << j;
					AssetNode assetNode(assetName.str());
					map<double,bool> stateData = (*stateIt)->getFullProfile(*keyIt).getProfileData();
					assetNode.addNumericData(stateData);
					dataNode.addChildNode(assetNode);
				}
				rootNode.addChildNode(dataNode);
			}
		}
		if(!mkeys.empty()) {
			for(vector<StateVarKey<Matrix>>::const_iterator keyIt = mkeys.begin(); keyIt != mkeys.end(); keyIt++){
				//write this
			}
		}
		if(!qkeys.empty()) {
			for(vector<StateVarKey<Quat>>::const_iterator keyIt = qkeys.begin(); keyIt != qkeys.end(); keyIt++) {
				//write this
			}
		}
	}
	rootNode.writeFile(filename);
	return true;
}
}}} //end namespace horizon::io::xml
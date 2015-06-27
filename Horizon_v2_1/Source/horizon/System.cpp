#include "System.h"

namespace horizon {

System::System(const vector<const Asset*> assets, const vector<SubsystemNode*> subsystemNodes, 
			   const vector<const Constraint*> constraints, Environment* environment): assets(assets), 
			   subsystemNodes(subsystemNodes), constraints(constraints), environment(environment) {
					vector<SubsystemNode*>::const_iterator nIt = subsystemNodes.begin();
					size_t nAsset;
					for(nIt ; nIt != subsystemNodes.end(); nIt++) {
						for(nAsset = 0; nAsset < assets.size(); nAsset++) {
							if((*nIt)->getAsset() == assets[nAsset])
								(*nIt)->setAssetNum(nAsset);
						}
					}
				}

System::~System() {
	for(vector<const Asset*>::const_iterator aIt = assets.begin(); aIt != assets.end(); aIt++) delete *aIt;
	for(vector<SubsystemNode*>::const_iterator subIt = subsystemNodes.begin(); subIt != subsystemNodes.end(); subIt++) delete *subIt;
	for(vector<const Constraint*>::const_iterator cIt = constraints.begin(); cIt != constraints.end(); cIt++) delete *cIt;
	delete environment;
}

bool System::canPerform(systemSchedule* sysSched)  
{
	// Iterate through Subsystem Nodes and set that they havent run
	for(vector<SubsystemNode*>::const_iterator subNodeIt = subsystemNodes.begin(); subNodeIt != subsystemNodes.end(); subNodeIt++)
		(*subNodeIt)->reset();

	// Iterate through constraints
	for(vector<const Constraint*>::const_iterator constraintIt = constraints.begin(); constraintIt != constraints.end(); constraintIt++) {
		if(!(checkSubs((*constraintIt)->getSubsystemNodes(), sysSched, true)))
			return false;
		// If the constraint qualifier doesn't accept the new state data, the System fails
		if(!(*constraintIt)->accepts(sysSched))
			return false;
	}

	// Check the remaining Subsystems that aren't included in any Constraints
	if(!(checkSubs(subsystemNodes, sysSched, false)))
		return false;

	// If none of the Constraints failed and none of the remaining Subsytems failed, then the System passes!
	return true;
}

bool System::checkSubs(vector<SubsystemNode*> subNodeVec, systemSchedule* sysSched, const bool mustEvaluate) 
{
	size_t subAssetNum;
	for(vector<SubsystemNode*>::iterator subNodeIt = subNodeVec.begin(); subNodeIt != subNodeVec.end(); subNodeIt++) {
		subAssetNum = (*subNodeIt)->getAssetNum();
		if(!(**subNodeIt).canPerform(sysSched->getSubNewState(subAssetNum), sysSched->getSubNewTask(subAssetNum), environment, sysSched->getLastTaskStart(), mustEvaluate))
			return false;
	}
	return true;
}

bool System::checkForCircularDependencies() const
{
	bool hasCircDep = false;
	for(vector<SubsystemNode*>::const_iterator nodeIt = subsystemNodes.begin(); nodeIt != subsystemNodes.end(); nodeIt++) {
		SubsystemNode* currNode = *nodeIt;
		hasCircDep |= checkSubForCircularDependncies(*nodeIt, *nodeIt);
		if(hasCircDep)
			break;
	}
	return hasCircDep;
}

bool System::checkSubForCircularDependncies(SubsystemNode* currNode, SubsystemNode* beginNode) const
{
	bool hasCircDep = false;
	vector<SubsystemNode*> preceedingNodes = currNode->getPreceedingNodes();
	if(!preceedingNodes.empty()) {
		for(vector<SubsystemNode*>::const_iterator nodeIt = preceedingNodes.begin(); nodeIt != preceedingNodes.end(); nodeIt++) {
			hasCircDep |= (*nodeIt) == beginNode;
			if(hasCircDep)
				break;
			hasCircDep |= checkSubForCircularDependncies(*nodeIt, beginNode);
			if(hasCircDep)
				break;
		}
	}
	return hasCircDep;
}

} // end namespace horizon
#include "System.h"

namespace horizon {

System::System() {}

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

System::System(const System& other) : assets(other.assets), environment(other.environment) {
	// First make a deep copy of the SubsystemNode's
	vector<SubsystemNode*>::const_iterator oSubIt = other.subsystemNodes.begin();
	for(; oSubIt != other.subsystemNodes.end(); oSubIt++) {
		this->subsystemNodes.push_back(new SubsystemNode(**oSubIt));
	}
	// Now need to fill in the preceeding nodes
	// Iterate through original subsystems
	oSubIt = other.subsystemNodes.begin();
	vector<SubsystemNode*>::iterator currSubIt = subsystemNodes.begin();
	for(; oSubIt != other.subsystemNodes.end(); oSubIt++, currSubIt++) {
		// Iterate through the preceeding nodes
		vector<SubsystemNode*> preceedingNodes = (*oSubIt)->getPreceedingNodes();
		vector<SubsystemNode*>::iterator prevSubIt = preceedingNodes.begin();
		for(; prevSubIt != preceedingNodes.end(); prevSubIt++) {
			// Get index of current sub
			vector<SubsystemNode*>::const_iterator pos = find(other.subsystemNodes.begin(), other.subsystemNodes.end(), *prevSubIt);
			int index = pos - other.subsystemNodes.begin();
			// Add preceeding node
			(*currSubIt)->addPreceedingNode(subsystemNodes.at(index));
		}
	}
	// Next create a copy of the constraints (using clone() interface) and resetting previous nodes
	// In outer loop make a deep copy of the Constraint's
	vector<const Constraint*>::const_iterator oConIt = other.constraints.begin();
	for(; oConIt != other.constraints.end(); oConIt++) {
		Constraint* newConstraint = (*oConIt)->clone();
		// In inner loop reset the constrained nodes
		// Loop through constrained nodes
		vector<SubsystemNode*> constrainedNodes = (*oConIt)->getSubsystemNodes();
		vector<SubsystemNode*>::iterator oConNodeIt = constrainedNodes.begin();
		for(; oConNodeIt != constrainedNodes.end(); oConNodeIt++) {
			// Get index offset
			vector<SubsystemNode*>::const_iterator pos = find(other.subsystemNodes.begin(), other.subsystemNodes.end(), *oConNodeIt);
			int index = pos - other.subsystemNodes.begin();
			// Add constrained node
			newConstraint->addConstrianedSubNode(subsystemNodes.at(index));
		}
		// Add the Constraint
		constraints.push_back(newConstraint);
	}
}

System::~System() {
	for(vector<const Asset*>::const_iterator aIt = assets.begin(); aIt != assets.end(); aIt++) if(*aIt) { delete *aIt; }
	for(vector<SubsystemNode*>::const_iterator subIt = subsystemNodes.begin(); subIt != subsystemNodes.end(); subIt++) if(*subIt) { delete *subIt; }
	for(vector<const Constraint*>::const_iterator cIt = constraints.begin(); cIt != constraints.end(); cIt++) if(*cIt) { delete *cIt; }
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

void System::setThreadNum(int threadNum) {
	this->threadNum = threadNum;
	for(vector<SubsystemNode*>::const_iterator subIt = subsystemNodes.begin(); subIt != subsystemNodes.end(); subIt++) {
		(*subIt)->setThreadNum(threadNum);
	}
}

void System::setDependencies(Dependencies* deps) {
	vector<SubsystemNode*>::iterator subIt = subsystemNodes.begin();
	for(; subIt != subsystemNodes.end(); subIt++) {
		(*subIt)->setDependencies(deps);
	}
}



} // end namespace horizon
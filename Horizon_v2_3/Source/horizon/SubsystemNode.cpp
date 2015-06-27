#include "SubsystemNode.h"
#include "Asset.h"

namespace horizon {

SubsystemNode::SubsystemNode(const Subsystem* subsystem, Asset* asset) : subsystem(subsystem), asset(asset){ dependencies = NodeDependencies(); }

SubsystemNode::SubsystemNode(const SubsystemNode& other) {
	// Copy flag variables
	this->checked = other.checked;
	this->scriptingEnabled = other.scriptingEnabled;
	// Copy pointers
	this->subsystem = other.subsystem->clone();
	this->asset = other.asset;
	// Copy asset number
	this->nAsset = other.nAsset;
	// Copy NodeDependencies
	this->dependencies = NodeDependencies(other.dependencies);
}

SubsystemNode::~SubsystemNode() { dependencies = NodeDependencies();}

void SubsystemNode::reset() {
	checked = false;
}

void SubsystemNode::addPreceedingNode(SubsystemNode* node)
{
	preceedingNodes.push_back(node);
}

bool SubsystemNode::canPerform(State* newState, const Task* task, Environment* environment, const double evalToTime, 
							   const bool mustEvaluate)
{
	for(vector<SubsystemNode*>::iterator nodeIt = preceedingNodes.begin(); nodeIt != preceedingNodes.end(); nodeIt++) {
		if(!((*nodeIt)->checked))
			if(!(*nodeIt)->canPerform(newState, task, environment, evalToTime, mustEvaluate))
				return false; 
	}
	if(!checked) {
		checked = true;
		if(newState->getTaskStart() >= evalToTime && task != NULL)
			return subsystem->canPerform(newState->getPrevious(), newState, task, asset->getPos(), environment, &dependencies);
		else
		{
			if(mustEvaluate)
				return subsystem->canExtend(newState, asset->getPos(), environment, evalToTime, &dependencies);
			else
				return true;
		}
	}
	else
		return true;
}

} // end namespace horizon
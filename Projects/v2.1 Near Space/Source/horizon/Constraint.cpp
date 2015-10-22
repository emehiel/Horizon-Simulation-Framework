#include "Constraint.h"

namespace horizon {

Constraint::Constraint() {}

Constraint::~Constraint() {}

void Constraint::addConstrianedSubNode(SubsystemNode* node)
{
	subsystemNodes.push_back(node);
}

const vector<SubsystemNode*>& Constraint::getSubsystemNodes() const {
	return subsystemNodes;
}

} // end namespace horizon

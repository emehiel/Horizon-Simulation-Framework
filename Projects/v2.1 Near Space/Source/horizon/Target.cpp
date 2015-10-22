#include "Target.h"

namespace horizon {
Target::Target(XMLNode targetNode) : position(new Position(targetNode.getAttribute("PositionType"), atof(targetNode.getAttribute("Latitude")), 
		atof(targetNode.getAttribute("Longitude")), atof(targetNode.getAttribute("Altitude")))) 
{
// USER - Add other Target Attributes Here
	targetType = targetNode.getAttribute("TargetType");
	name = targetNode.getAttribute("name");
	value = atoi(targetNode.getAttribute("value"));
	/*
	minQualCM = atoi(targetNode.getAttribute("minQual_cm"));;
	freq_days = atoi(targetNode.getAttribute("freq_days"));;
	CC = targetNode.getAttribute("CC");;
	WX_Reg = targetNode.getAttribute("WX_Reg");;
	Comment = targetNode.getAttribute("Comment");;
	*/
}

Target::Target(const string& name, Position* position) : name(name), position(position), value(0) {}

Target::Target(const string& name, Position* position, int value) : name(name), position(position), value(value) {}

Position* Target::getPos() const {
	return position;
}
bool Target::operator==(const Target& other) const {
	return (name == other.name);
}
} // end namespace horizon

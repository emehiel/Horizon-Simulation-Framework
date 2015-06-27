#include "Target.h"

namespace horizon {
Target::Target(XMLNode targetXMLNode) : 
	name(targetXMLNode.getAttribute("TargetName")),
	targetType(targetXMLNode.getAttribute("TargetType")),
	position(new Position(targetXMLNode.getChildNode("POSITION"))),
	value(0)
{	
	if(targetXMLNode.isAttributeSet("MaxTimes"))
		value = atoi(targetXMLNode.getAttribute("Value"));
	// USER - Add other Target Attributes Here
	/*minQualCM = atoi(targetNode.getAttribute("minQual_cm"));;
	freq_days = atoi(targetNode.getAttribute("freq_days"));;
	CC = targetNode.getAttribute("CC");;
	WX_Reg = targetNode.getAttribute("WX_Reg");;
	Comment = targetNode.getAttribute("Comment");;*/
}

Target::Target(const string& name, Position* position) : name(name), position(position), targetType(TARGET_TYPE_EARTH_LOCATION), value(0) {}

Target::Target(const string& name, Position* position, int value) : name(name), position(position), targetType(TARGET_TYPE_EARTH_LOCATION), value(value) {}

Position* Target::getPos() const {
	return position;
}
bool Target::operator==(const Target& other) const {
	return (name == other.name);
}
} // end namespace horizon

#ifndef SUBSYSTEMDATANODE_H
#define SUBSYSTEMDATANODE_H

#include "xmlNode.h"
#include <string>

#define NUMERIC "numeric"
#define MATRIX	"matrix"
#define QUAT	"quaternion"

namespace horizon {
namespace io {
namespace xml {

class SubsystemDataNode : public xmlNode
{
	
public:
	SubsystemDataNode(string dataType, string dataName) : xmlNode("SubsystemData") {
		addAttribute("type", dataType);
		addAttribute("name", dataName);
	}

	~SubsystemDataNode() {}

};
}}} //end namespace horizon::io::xml
#endif
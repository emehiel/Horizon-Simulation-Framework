#ifndef ASSETNODE_H
#define ASSETNODE_H

#include "xmlNode.h"
#include <string>

namespace horizon {
namespace io {
namespace xml {

class AssetNode : public xmlNode
{
public:
	AssetNode(string assetName) : xmlNode("AssetData") {
		addAttribute("assetName", assetName);	
	}

	~AssetNode() {};

	template <typename T>
	void addNumericData(map<double,T> num_data) {
	if(!num_data.empty()) {
		for(map<double,T>::const_iterator dataIt = num_data.begin(); dataIt != num_data.end(); dataIt++) {
			xmlNode dataPoint("Data");
			dataPoint.addAttribute("time", dataIt->first);
			dataPoint.addAttribute("data", dataIt->second);
			addChildNode(dataPoint);
		}
	}
}
};
}}} //end namespace horizon::io::xml
#endif
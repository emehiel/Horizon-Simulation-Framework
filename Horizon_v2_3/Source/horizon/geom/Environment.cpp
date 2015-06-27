#include "Environment.h"

namespace horizon {
	namespace geom {

		Environment::Environment() : sun(new Sun(false)) {}

		Environment::Environment(XMLNode& environmentNode) {
			// Check the XMLNode for the presence of a child SUN node
			int nSun = environmentNode.nChildNode("SUN");
			if(nSun != 0) {
				// Create the Sun based on the XMLNode
				XMLNode sunNode = environmentNode.getChildNode("SUN");
				// Check the Sun XMLNode for the attribute
				if(sunNode.isAttributeSet("isSunVecConstant")) {
					bool sunVecConst = atob(sunNode.getAttribute("isSunVecConstant"));
					sun = new Sun(sunVecConst);
				}
				sun = new Sun();
			}
			else
				sun = new Sun();		
		}

		Environment::Environment(Sun* sun): sun(sun) {}

		Environment::~Environment() { delete sun; }

}}  // end namespace horizon::geom
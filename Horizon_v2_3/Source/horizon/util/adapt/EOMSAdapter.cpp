#include "EOMSAdapter.h"

namespace horizon {
	namespace util {
		namespace adapt {

EOMS* createEOMSObject(XMLNode& EOMSNode) {
	EOMS* ret = NULL;
	// Check if the 'EOMSType' attribute exists
	if(!EOMSNode.isAttributeSet("EOMSType")) {
		cout << "Attribute 'EOMSType' missing from <EOMS> node" << endl;
		return ret;
	}
	// Get the 'EOMSType' attribute
	string type = EOMSNode.getAttribute("EOMSType");
	if(_strcmpi(type.c_str(), "scripted") == 0) {
		// Scripted
		// Check that the attribute 'evalFcn' is set
		if(EOMSNode.isAttributeSet("evalFcn")) {
			// Check if the attribute 'initFcn' is set
			if(EOMSNode.isAttributeSet("initFcn")) {
				return new ScriptedEOMS(lua::L, EOMSNode.getAttribute("evalFcn"), EOMSNode.getAttribute("initFcn"));
			}
			else {
				return new ScriptedEOMS(lua::L, EOMSNode.getAttribute("evalFcn"));
			}
		}
		else {
			cout << "Attribute 'evalFcn' missing from <EOMS> node for scripted EOMS" << endl;
		}
	}
	else if(_strcmpi(type.c_str(), "orbital_EOMS") == 0) {
		// orbital_EOMS
		return new orbital_EOMS();
	} else if(_strcmpi(type.c_str(), "orbital_EOMS_J2") == 0) {
		return new orbital_EOMS_J2();
	} else if(_strcmpi(type.c_str(), "orbital_EOMS_TEST") == 0) {
		return new orbital_EOMS_TEST();
	}
	return ret;
}

}}} // horizon::util::adapt
#include "EOMSAdapter.h"

namespace horizon {
namespace util {
namespace adapt {

	EOMS* createEOMSObject(XMLNode& EOMSNode) {
		string type = EOMSNode.getAttribute("EOMSType");
		if(strcmp(type.c_str(),ORBITAL_EOMS)==0) {
			return new orbital_EOMS();
		}
		else if(strcmp(type.c_str(),STATESPACE_EOMS)==1) {
			EOMSInput* eoi = new polyEOMSInput(atom(EOMSNode.getAttribute("EOMSdata1")));
			Matrix a = atom(EOMSNode.getAttribute("EOMSdataA"));
			Matrix b = atom(EOMSNode.getAttribute("EOMSdataB"));
			Matrix c = atom(EOMSNode.getAttribute("EOMSdataC"));
			Matrix d = atom(EOMSNode.getAttribute("EOMSdataD"));
			return new stateSpace_EOMS(eoi,a,b,c,d);
		}
		else
			return NULL;
	}

}}}
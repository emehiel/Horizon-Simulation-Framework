#include "ScriptedEOMS.h"

namespace horizon {
	namespace util {
		namespace eoms {

ScriptedEOMS::ScriptedEOMS(lua_State* L, string evalEOMSfcn, string initEOMSfcn) : 
	EOMS(), L(L), evalEOMSfcn(evalEOMSfcn), initEOMSfcn(initEOMSfcn)
{
	if(!initEOMSfcn.empty()) {
		// Call the function, and catch any error message
		try {
			if(functionExists(L, initEOMSfcn))
				call_function<void>(L, initEOMSfcn.c_str());
			else {
				cout << "Initilization function '" << initEOMSfcn;
				cout << "' for Scripted EOMS does not exist!" << endl;
			}
		}
		catch(error& e) {
			object error_msg(from_stack(e.state(), -1));
			cout << "Error Initializing Scripted EOMS!" << endl;
			cout << error_msg << endl;
		}

	}
}
		
ScriptedEOMS::~ScriptedEOMS() {}

Matrix ScriptedEOMS::operator() ( const double &t, const Matrix &y ) {
	Matrix ret;
	static bool evalFcnExists = true;
	if(!initEOMSfcn.empty()) {
		// Call the function, and catch any error message
		try {
			if(evalFcnExists || functionExists(L, evalEOMSfcn)) {
				evalFcnExists = true;
				ret = call_function<Matrix>(L, evalEOMSfcn.c_str(), t, y);
			}
			else {
				cout << "EOMS function '" << evalEOMSfcn;
				cout << "' for Scripted EOMS does not exist!" << endl;
			}
		}
		catch(error& e) {
			object error_msg(from_stack(e.state(), -1));
			cout << "Error calling Scripted EOMS!" << endl;
			cout << error_msg << endl;
		}
	}
	return ret;
}

}}} // horizon::util::eoms
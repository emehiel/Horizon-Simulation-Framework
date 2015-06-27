#include "simParams.h"

namespace {
    double dSIMSTART_JD = 0.0;
	double dSIMSTART_SECONDS = 0.0;
    double dSIMEND_SECONDS = 0.0;
    bool isInit = FALSE;
}

namespace simParams {

	double SIMSTART_JD() { return dSIMSTART_JD; }

	double SIMSTART_SECONDS() { return dSIMSTART_SECONDS; }

	double SIMEND_SECONDS() { return dSIMEND_SECONDS; }

	bool LoadSimParameters(XMLNode& simParametersXMLNode) {
		if(!isInit) {
			isInit = TRUE;
			cout << endl << "Loading simulation parameters... " << endl ;
			dSIMSTART_JD = (atof(simParametersXMLNode.getAttribute("SimStartJD")));
			cout << "  Simulation Start Julian Date: " << dSIMSTART_JD << endl;
			dSIMSTART_SECONDS = (atof(simParametersXMLNode.getAttribute("SimStart")));
			cout << "  Start Epoch: " << dSIMSTART_SECONDS << " seconds" << endl;
			dSIMEND_SECONDS = (atof(simParametersXMLNode.getAttribute("SimEnd")));
			cout << "  End Epoch: " << dSIMEND_SECONDS << " seconds" << endl;
			return true;
		}
		else
			return false;
	}
}
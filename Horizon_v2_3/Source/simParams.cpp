/**
 * \file simParams.cpp
 * This file contains implementation for simParams
 */

#include "simParams.h"

namespace {
	/** Static variable to hold starting Julian Date for the simulation. */
    double dSIMSTART_JD = 0.0;
	/** Static variable to hold start time in seconds for the simulation. */
	double dSIMSTART_SECONDS = 0.0;
	/** Static variable to hold end time in seconds for the simulation. */
    double dSIMEND_SECONDS = 0.0;
	/** Static variable to hold the name of the scenario. */
	string scName;
	/** Static variable to the output directory. */
	string outDir;
	/** Static variable to hold whether the simulation parameters have been loaded sucessfully. */
    bool isInit = FALSE;
}

namespace simParams {

	double SIMSTART_JD() { return dSIMSTART_JD; }

	double SIMSTART_SECONDS() { return dSIMSTART_SECONDS; }

	double SIMEND_SECONDS() { return dSIMEND_SECONDS; }

	string getScenarioName() { return scName; }

	string getOutputDir() { return outDir; }

	void setOutputDir(string dir) { outDir = dir; }

	bool LoadSimParameters(XMLNode& simParametersXMLNode, string scenarioName) {
		if(!isInit) {
			isInit = TRUE;
			scName = scenarioName;
			cout << endl << "Loading simulation parameters... " << endl ;
			dSIMSTART_JD = (atof(simParametersXMLNode.getAttribute("SimStartJD")));
			cout << "  Simulation Start Julian Date: " << dSIMSTART_JD << endl;
			if(simParametersXMLNode.isAttributeSet("SimStart"))
				dSIMSTART_SECONDS = (atof(simParametersXMLNode.getAttribute("SimStart")));
			else
				dSIMSTART_SECONDS = 0;
			cout << "  Start Epoch: " << dSIMSTART_SECONDS << " seconds" << endl;
			dSIMEND_SECONDS = (atof(simParametersXMLNode.getAttribute("SimEnd")));
			cout << "  End Epoch: " << dSIMEND_SECONDS << " seconds" << endl;
			return true;
		}
		else
			return false;
	}

	void setSimEnd(double time) {
		dSIMEND_SECONDS = time;
	}
}
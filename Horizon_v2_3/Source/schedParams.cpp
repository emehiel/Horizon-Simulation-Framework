#include "schedParams.h"

namespace {
    bool isInit = FALSE;
    double dSIMSTEP_SECONDS = 30.0;
    size_t dMAX_SCHEDS = 0;
	size_t dNUM_CROP = 0;
}

namespace schedParams {

	double SIMSTEP_SECONDS() { return dSIMSTEP_SECONDS; }
	size_t MAX_SCHEDS() { return dMAX_SCHEDS; }
	size_t NUM_CROP() {return dNUM_CROP; }

	bool loadSchedulerParams(XMLNode& schedParametersXMLNode) {
		if(!isInit) {
			isInit = TRUE;
			cout << endl << "Loading scheduler parameters... " << endl;
			
			dSIMSTEP_SECONDS = atof(schedParametersXMLNode.getAttribute("simStep"));
			cout << "  Scheduler time step: " << dSIMSTEP_SECONDS << " seconds" << endl;

			dMAX_SCHEDS = atoi(schedParametersXMLNode.getAttribute("maxSchedules"));
			cout << "  Maximum number of schedules: " << dMAX_SCHEDS << endl;

			dNUM_CROP = atoi(schedParametersXMLNode.getAttribute("numCrop"));
			cout << "  Number of schedules to crop to: " << dNUM_CROP << endl;
			return true;	
		}
		else
			return false;
	}
}
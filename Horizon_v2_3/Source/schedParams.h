#ifndef SCHEDPARAMS_H
#define SCHEDPARAMS_H

#include "horizon/io/xmlParser.h"
#include <iostream>

using namespace std;
using namespace horizon::io;

namespace schedParams {
    double SIMSTEP_SECONDS();
    size_t MAX_SCHEDS();
	size_t NUM_CROP();

	/**
	 * Loads all the scheduler parameters into the scheduler from the XML input file
	 * @param simParametersNodeXML The node to read parameters from.
	 * @param scheduler The scheduler to write the parameters to.
	 * @return Whether the parameters were successfully set.
	 */
	bool loadSchedulerParams(XMLNode& simParametersNodeXML);
};

#endif /*SCHEDPARAMS_H*/
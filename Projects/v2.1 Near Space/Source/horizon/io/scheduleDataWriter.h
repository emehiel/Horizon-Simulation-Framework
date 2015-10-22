#ifndef SCEHDULEDATAWRITER_H_
#define SCEHDULEDATAWRITER_H_

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "direct.h"
#include "time.h"
#include <windows.h>
#include "../systemSchedule.h"
#include "../util/Matrix.h"
#include "../../simParams.h"
#include "../System.h"
#include "../util/Profile.h"
#include <boost/shared_ptr.hpp>
#include "xml/xmlDataWriter.h"

namespace horizon {
namespace io {

	using namespace std;
	using horizon::systemSchedule;
	using horizon::State;
	using boost::shared_ptr;
	using horizon::io::xml::xmlDataWriter;

/**
 * An output class that generates tab-delimited text files containing all the
 * recorded states for specific variables
 * @author Cory O'Connor
 */
class scheduleDataWriter
{
	/** The full path to the output directory */
	string dirOut;
	
	/** The relative path to the output directory */
	string relOut;

	/** The path to the output directory */
	bool cwOut;

	DWORD tickerStart;

	DWORD tickerEnd;

	double runTime;

	/**  */
	size_t nSched;

public:

	/** 
	 * Creates the scheduleDataWriter
	 * @param outputDir The subdirectory to put the output files
	 * @param simStartJD The julian date the simulation starts at. Used for LLA conversions.
	 */
	scheduleDataWriter(const string& outputDir, bool shouldOutputGoToCommandWindow, size_t numOutput);

	/** Destroys the scheduleDataWriter */
	~scheduleDataWriter();
	
	/** 
	 * Calls all data output schemes and puts them inside a time-stamped folder.
	 * @param schedules The list of schedules.
	 * @param shouldOutputGoToCommandWindow If true, overview schedule data also prints to the command window.
	 * @param schedulesToShow How many schedules to output data for.
	 * @return The relative data path to the output directory for logging purposes.
	 */
	bool writeAll(const list<systemSchedule*> schedules, System* system);

private:

	/** 
	 * Writes the targets captured in the schedule and the appropriate time and position data
	 * @param events The schedule to output the data for.
	 * @param schedulePath The path where the file should be written to
	 * @param assetName The name of the asset the list of events is for
	 */
	void writeTargetData(const vector<shared_ptr<Event>>& events, string schedulePath, string assetName);


	/** 
	 * Writes position and time data for all assets
	 * @param asset The Asset to output the position data for
	 * @param stepSize The step size to output data at
	 * @param schedulePath The path where the file should be written to
	 * @param assetName The name of the asset
	 */
	void writeAssetPositionData(const Asset* asset, double stepSize, string schedulePath, string assetName);

	/** 
	 * Writes velocity and time data for all assets
	 * @param asset The Asset to output the position data for
	 * @param stepSize The step size to output data at
	 * @param schedulePath The path where the file should be written to
	 * @param assetName The name of the asset
	 */
	void writeAssetVelocityData(const Asset* asset, double stepSize, string schedulePath, string assetName);

	/** 
	 * Writes all state data for an asset
	 * @param events The schedule to output the data for.
	 * @param system The simulation system
	 * @param schedulePath The path where the file should be written to
	 * @param assetName The name of the asset
	 */
	void writeStateData(const vector<shared_ptr<Event>>& events, System* system, string schedulePath, string assetName);

};

}} //end namespace horizon::io

#endif // SCEHDULEDATAWRITER_H_
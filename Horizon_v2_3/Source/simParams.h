/**
 * \file simParams.h
 * This file contains Task, Target, and Position types and functions for getting and setting simulation parameters.
 */

#ifndef SIMPARAMS_H
#define SIMPARAMS_H

#include "horizon/io/xmlParser.h"
#include <iostream>
#include <string>

using namespace std;
using namespace horizon::io;

// USER - Define Target types
#define TARGET_TYPE_EARTH_LOCATION "LocationTarget"
#define TARGET_TYPE_EARTH_FACILITY "FacilityTarget"
#define TARGET_TYPE_PERSONNEL "PersonnelTarget"
#define TARGET_TYPE_GROUND_VEHICLE "GroundVehicleTarget"
#define TARGET_TYPE_AIR_VEHICLE "AirVehicleTarget"
#define TARGET_TYPE_BALLISTIC "BallisticTarget"
#define TARGET_TYPE_SPACE_VEHICLE "SpaceVehicleTarget"
#define TARGET_TYPE_UNKNOWN "UnknownTarget"

// USER - Define Task types
#define TASK_TYPE_IMAGE "ImagingTask"
#define TASK_TYPE_COMM "CommTask"
#define TASK_TYPE_SELF "SelfTask"
#define TASK_TYPE_RENDEZVOUS "RendezvousTask"
#define TASK_TYPE_DESTROY "DestroyTask"
#define TASK_TYPE_EMPTY "NoTask"

// USER - Define Position types
#define POSITION_TYPE_STATIC_LLA "StaticLLA"
#define POSITION_TYPE_STATIC_ECI "StaticECI"
#define POSITION_TYPE_PREDETERMINED_LLA "PredeterminedLLA"
#define POSITION_TYPE_PREDETERMINED_ECI "PredeterminedECI"
#define POSITION_TYPE_DYNAMIC_LLA "DynamicLLA"
#define POSITION_TYPE_DYNAMIC_ECI "DynamicECI"

// Define Constants
#define EARTH_RADIUS 6378.137
#define STEPHAN_BOLTZMAN_CONST 5.670400e-8

namespace simParams {
	/**
	 * Gets the starting Julian Date of the Simulation.
	 * @return The Julian Date corresponding to the start of the simulation.
	 */
    double SIMSTART_JD();

	/**
	 * Gets the start of the simulation in seconds after the starting Julian Date.
	 * This represents the earliest possible time that Events can be scheduled in the simulation.
	 * @return The start of the simulation in seconds after the starting Julian Date.
	 */
	double SIMSTART_SECONDS();

	/**
	 * Gets the end of the simulation in seconds after the starting Julian Date.
	 * No scheduled Events will end past this time.
	 * @return The end of the simulation in seconds after the starting Julian Date.
	 */
    double SIMEND_SECONDS();

	/**
	 * Gets the name of the scenario.
	 * @return The name of the scenario.
	 */
	string getScenarioName();

	/**
	 * Gets the current output directory.
	 * @return The full path of the output directory.
	 */
	string getOutputDir();

	/**
	 * Sets the current output directory.
	 * @param dir The full path of the output directory.
	 */
	void setOutputDir(string dir);

	/**
	 * Loads simulation parameters from an XMLNode.
	 * @param simParametersXMLNode The XMLNode that contains the simulation parameters.
	 * This XMLNode must contain the following attributes:<BR>
	 * <CODE>SimStartJD</CODE> - Sets the starting Julian Date.<BR>
	 * <CODE>SimStart</CODE> - Sets the starting simulation time is seconds.<BR>
	 * <CODE>SimEnd</CODE> - Sets the ending simulation time is seconds.
	 * @param scenarioName The name of the scenario.
	 * @return true if the parameters were loaded sucessfully, false otherwise.
	 */
    bool LoadSimParameters(XMLNode& simParametersXMLNode, string scenarioName);

	void setSimEnd(double time);
};

#endif /*SIMPARAMS_H*/
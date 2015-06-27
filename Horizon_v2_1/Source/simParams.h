#ifndef SIMPARAMS_H
#define SIMPARAMS_H

#include "horizon/io/xmlParser.h"
#include <iostream>

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
    double SIMSTART_JD();
	double SIMSTART_SECONDS();
    double SIMEND_SECONDS();
    bool LoadSimParameters(XMLNode& simParametersXMLNode);
};

#endif /*SIMPARAMS_H*/
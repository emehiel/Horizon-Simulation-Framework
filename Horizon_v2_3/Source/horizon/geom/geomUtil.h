/**
 * \file geomUtil.h
 * This file contains functions for converting to and from different
 * coordinate and time systems.
 */

#ifndef GEOM_UTIL_H
#define GEOM_UTIL_H

#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>
#include <map>

#include "../util/Matrix.h"
#include "../util/umath.h"
#include "../../simParams.h"


namespace horizon {
	namespace geom {

		using horizon::util::Matrix;
		using horizon::util::norm;
		using horizon::util::umath::dot;
		using horizon::util::umath::cross;

/**
 * Returns whether the static or predetermined positions 1 and 2
 * have a line of sight to each other.
 * @param posECI1 The first Position.
 * @param posECI2 The second Position.
 * @return true if the two positions have line of sight, false otherwise.
 */
bool hasLOS(Matrix posECI1, Matrix posECI2);

/**
* Converts the Position of an object from LLA (Longitude, Latitude, Altitude) to 
* ECI (Earth-centered inertial) (XYZ) coordinate system.
* This function is based off a spherical model of the earth.
* @param LLA The Position of the object in LLA coordinates.
* @param JD Yhe Julian Date at which to find the XYZ position
* @return A column vector Matrix with X, Y, and Z position.
*/
Matrix LLA2ECI(const Matrix& LLA, const double& JD);

/**
* Converts the Position of an object from ECI (Earth-centered inertial) (XYZ) to 
* LLA (Longitude, Latitude, Altitude) coordinate system
* This function is based off a spherical model of the earth.
* @param ECI The Position of the object in ECI coordinates.
* @param JD The Julian Date at which to find the LLA position.
* @return A column vector Matrix with the Latitude, Longitude, Altitude.
*/
Matrix ECI2LLA(const Matrix& ECI, const double& JD);

/**
* Converts HMS (Hours, Minutes, Seconds) to UT (Universal Time).
* @param hour The time at which to return the state.
* @param minute the time at which to return the state.
* @param seconds the time at which to return the state.
* @return The Universal Time..
*/
double HMS2UT(const unsigned& hour, const unsigned& minute, const double& seconds);

/**
* Converts YMDUT (Year, Month, Day, Universal Time) to JD (Julian Day).
* @param year The year in YMDUT.
* @param month The month in YMDUT.
* @param day The day in YMDUT.
* @param UT The UT in YMDUT.
* @return A double representing the Julian Day.
*/
double YMDUT2JD(const unsigned& year, const unsigned& month, const unsigned& day, const double& UT);

/**
 * Converts from Local Civilian Time, (CT) to Local Mean Sidereal Time, (LST).
 * @param longitude The East Longitude of the location where the Local Sidereal Time is desired.
 * @param JD The Local Julian Date at the location where the Local Sidereal Time is desired.
 * @return The Local Civilian Time in degrees.
 */
double CT2LST(const double longitude, const double JD);

}} // end namespace horizon::geom
#endif /* GEOM_UTIL_H_ */
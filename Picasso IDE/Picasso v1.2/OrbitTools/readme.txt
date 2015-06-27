
Documentation for C# NORAD SGP4/SDP4 Implementation
Developed by Michael F. Henry

Copyright © 2003 - 2009. All rights reserved.
Permission to use for non-commercial purposes only. 
All other uses contact author at mfh@zeptomoby.com

The files in this package implement the SGP4 and SDP4 algorithms described 
in the December, 1980 NORAD document "Space Track Report No. 3". The report 
provides FORTRAN IV implementations of each. These two orbital models, one 
for "near-earth" objects and one for "deep space" objects, are widely used in 
satellite tracking software and can produce very accurate results when used 
with current NORAD two-line element datum.

The C# files are packaged as two projects in the OrbitTools and OrbitToolsDemo 
directories. The files in the OrbitTools directory are compiled to make the 
OrbitTools assembly, which defines the "Zeptomoby.OrbitTools" namespace. 
Within this namespace are the NORAD SGP4/SDP4 implementations and 
miscellaneous supporting classes (see below).

The OrbitToolsDemo directory contains the files to build a simple executable
that demonstrates how to use the assembly to calculate the ECI position of a 
satellite as well as determine the look angle from a location on the earth
to the satellite.

The project files were compiled using Microsoft Visual Studio .NET 2003

===============================================================================

A brief description of important classes in the Zeptomoby.OrbitTools namespace:

Tle – This class encapsulates a single set of NORAD two-line elements.

Eci – This class encapsulates Earth-Centered Inertial coordinates and 
      velocity for a given moment in time.
      
Orbit – Given a Tle object, this class provides information about the orbit
      of the described satellite, including inclination, perigee, 
      eccentricity, etc. Most importantly, it provides ECI coordinates and 
      velocity for the satellite.

Site – Describes a location on the earth. Given the ECI coordinates of a 
      satellite, this class can generate Azimuth/Elevation look angles to 
      the satellite.

NoradBase, NoradSGP4, NoradSDP4 – These classes implement the NORAD SGP4/SDP4
      algorithms. They are used by class Orbit to calculate the ECI 
      coordinates and velocity of its associated satellite.

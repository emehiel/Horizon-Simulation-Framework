#ifndef TOOLS_H
#define TOOLS_H


#define _USE_MATH_DEFINES
#include <map>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <math.h>
#include <stdio.h>
#include "horizon/util/Matrix.h"
#include "horizon/util/matrixIndex.h"
#include "horizon/util/Data/Atmosdata.h"
#include "horizon/util/Data/Windata.h"



	using namespace std;
	using std::pair;
	using std::make_pair;
	using std::list;
	using std::vector;
	using std::map;
	using horizon::util::Matrix;
	using horizon::util::matrixIndex;
	using horizon::util::Data::Atmosdata;
	using horizon::util::Data::Windata;
	




//===========================================================================================================================
const pair<Windata,Atmosdata> ExactValues(const double alt,map<double,Windata>* windmap, map<double,Atmosdata>* atmosmap); 
//======================================================================================================================
Matrix StationLocation(double lat, double lon, double alt);
//======================================================================================================================
pair<double,double> BallGeom(Matrix oldAttribs,pair<Windata,Atmosdata> exact, double GasMass);
//======================================================================================================================
double GeopotheightToGeometAlt(double geoPotHeight,double Lat);
//==========================================================================
double GeometAltToGeopotheight(double GeomAlt);
//====================================================================================
pair<double,double> ssGeom(Matrix oldAttribs,pair<Windata,Atmosdata>, double ssGasMass);
//====================================================================================






#endif
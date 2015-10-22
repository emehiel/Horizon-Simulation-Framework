#ifndef BLIMP_EOMS_H
#define BLIMP_EOMS_H

#define _USE_MATH_DEFINES
#include "EOMS.h"
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <cmath>
#include <stdexcept>
#include "horizon\geom\geomUtil.h"

namespace horizon {
namespace util {
namespace eoms {

	using namespace std;
	using std::vector;
	using horizon::geom::LLA2ECI;
	using horizon::geom::ECI2LLA;

	/**
	* @author James F. Sangillo
	*/
	class blimp_EOMS : public EOMS
	{
	public:

		blimp_EOMS(Matrix& A);

		blimp_EOMS();

		virtual ~blimp_EOMS();

		virtual Matrix operator() ( const double &t, const Matrix &y);

		pair<double,double> Thrustequations(double t,double ZonalDrag, double MeridionalDrag, double Thrust,double msys, double Lat_dist, double Long_dist,double ZonalInitVel,double MeridionalInitVel)const;

		private:

		const double g;
		};
} 
} 
} 

#endif 
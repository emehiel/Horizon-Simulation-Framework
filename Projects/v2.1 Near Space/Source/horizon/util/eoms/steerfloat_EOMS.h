#ifndef STEERFLOATEOMS_H
#define STEERFLOATEOMS_H
/*steered floater Equations of Motion
  Version 1 James Sangillo
  Calculates the drift of the steered floater*/

#include "EOMS.h"
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <stdexcept>
#include "horizon\geom\geomUtil.h"

namespace horizon{
	namespace util{
		namespace eoms{

	using namespace std;
	using std::vector;
	using horizon::util::Matrix;
	using horizon::util::matrixIndex;
	using horizon::geom::LLA2ECI;
	using horizon::geom::ECI2LLA;

	class ss_EOMS : public EOMS
	{

	public:

		ss_EOMS(Matrix& A);
		ss_EOMS();
		virtual ~ss_EOMS();
		virtual Matrix operator() (const double &t, const Matrix &y);

	private:

	const double g;
	};
		}
	}
}
#endif

#ifndef BALLOON_EOMS_H
#define BALLOON_EOMS_H

#include "EOMS.h"

#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
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

	class balloon_EOMS : public EOMS
	{

	public:

		balloon_EOMS(Matrix& A);

		balloon_EOMS();

		virtual ~balloon_EOMS();

		virtual Matrix operator() ( const double &t, const Matrix &y);

	private:

	const double g;
	};
} 
} 
} 

#endif 
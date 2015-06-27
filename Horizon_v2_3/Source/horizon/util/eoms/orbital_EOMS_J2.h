#ifndef ORBITAL_EOMSJ2_H
#define ORBITAL_EOMSJ2_H

#define _USE_MATH_DEFINES
#define EARTH_J2 1.08263E-3

#include "EOMS.h"
#include "../../geom/geomUtil.h"

using horizon::geom::ECI2LLA;

#define MU 398600.4418
#define J2 1.08263e-3
#define RE 6378.137

namespace horizon {
namespace util {
namespace eoms {

	class orbital_EOMS_J2 : public EOMS 
	{

	public:
		/**
		* 
		*/
		orbital_EOMS_J2( Matrix& A );

		orbital_EOMS_J2();

		virtual ~orbital_EOMS_J2();

		virtual Matrix operator() ( const double &t, const Matrix &y );

	private:
		const double mu_;
		Matrix A_;
	};

} // end namespace eoms
} // end namespace util
} // end namespace horizon

#endif // ORBITAL_EOMSJ2_H_

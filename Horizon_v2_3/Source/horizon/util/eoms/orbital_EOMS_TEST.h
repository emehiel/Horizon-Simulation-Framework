#ifndef ORBITAL_EOMS__TEST_H
#define ORBITAL_EOMS__TEST_H

#define MU 398600.4418
#define J2 1.08263e-3
#define RE 6378.137

#include "EOMS.h"

namespace horizon {
namespace util {
namespace eoms {

	class orbital_EOMS_TEST : public EOMS 
	{

	public:

		orbital_EOMS_TEST( Matrix& A );
		orbital_EOMS_TEST();
		virtual ~orbital_EOMS_TEST();

		virtual Matrix operator() ( const double &t, const Matrix &y );

	private:
		//const double mu_;
		Matrix A_;
		bool EFFECT_J2;
		bool EFFECT_DRAG;
	};

} // end namespace eoms
} // end namespace util
} // end namespace horizon

#endif // ORBITAL_EOMS_TEST_H

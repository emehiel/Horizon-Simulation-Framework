#include "orbital_EOMS.h"

namespace horizon {
	namespace util {
		namespace eoms {

			orbital_EOMS::orbital_EOMS( Matrix& A ) : EOMS(), A_(A), mu_(398600.4418) {}

			orbital_EOMS::orbital_EOMS() : EOMS(), mu_(398600.4418)
			{
				double Adata[] = {0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
								  0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
								  0.0, 0.0, 0.0, 0.0, 0.0, 1.0,
								  0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
								  0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
								  0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

				A_ = Matrix(6, 6, Adata);
			}

			orbital_EOMS::~orbital_EOMS() {}

			Matrix orbital_EOMS::operator() ( const double &t, const Matrix &y )
			{
				double r3 = pow(norm(y(colon(1,3))), 3);
				double mur3 = -mu_/r3;

				A_.setValue(4, 1, mur3);
				A_.setValue(5, 2, mur3);
				A_.setValue(6, 3, mur3);

				Matrix dy = A_*y;

				return dy;
			}

		}// end namespace eoms
	} // end namespace util
} // end namespace horizon

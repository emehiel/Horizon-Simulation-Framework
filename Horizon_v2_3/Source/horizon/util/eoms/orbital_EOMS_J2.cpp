#include "orbital_EOMS_J2.h"

namespace horizon {
	namespace util {
		namespace eoms {

			orbital_EOMS_J2::orbital_EOMS_J2( Matrix& A ) : EOMS(), A_(A), mu_(398600.4418) {}

			orbital_EOMS_J2::orbital_EOMS_J2() : EOMS(), mu_(398600.4418)
			{
				double Adata[] = {0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
								  0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
								  0.0, 0.0, 0.0, 0.0, 0.0, 1.0,
								  0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
								  0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
								  0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

				A_ = Matrix(6, 6, Adata);
			}

			orbital_EOMS_J2::~orbital_EOMS_J2() {}

			Matrix orbital_EOMS_J2::operator() ( const double &t, const Matrix &y )
			{
				double r3 = pow(norm(y(colon(1,3))), 3);
				double r = norm(y(colon(1,3)));

				double mur3 = -MU/r3;

				double r_x = y.getValue(1,1);
				double r_y = y.getValue(2,1);
				double r_z = y.getValue(3,1);
				double x__ = (1 - J2 * (3.0/2.0) * pow(RE/r, 2) * (5*pow(r_z/r,2) - 1));
				double y__ = x__;
				double z__ = (1 + J2 * (3.0/2.0) * pow(RE/r, 3) * (3 - 5*pow(r_z/r,2)));

				A_.setValue(4, 1, x__*mur3);
				A_.setValue(5, 2, y__*mur3);
				A_.setValue(6, 3, z__*mur3);

				Matrix dy = A_*y;

				return dy;
			}

		}// end namespace eoms
	} // end namespace util
} // end namespace horizon

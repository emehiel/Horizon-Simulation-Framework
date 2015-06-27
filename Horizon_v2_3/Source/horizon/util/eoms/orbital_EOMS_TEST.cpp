#include "orbital_EOMS_TEST.h"

namespace horizon {
	namespace util {
		namespace eoms {

			orbital_EOMS_TEST::orbital_EOMS_TEST( Matrix& A ) : EOMS(), A_(A)/*, mu_(398600.4418)*/ {
				EFFECT_J2 = true;
				EFFECT_DRAG = true;
			}

			orbital_EOMS_TEST::orbital_EOMS_TEST() : EOMS()/*, mu_(398600.4418)*/
			{
				double Adata[] = {0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
								  0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
								  0.0, 0.0, 0.0, 0.0, 0.0, 1.0,
								  0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
								  0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
								  0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

				A_ = Matrix(6, 6, Adata);
				EFFECT_J2 = true;
				EFFECT_DRAG = true;
			}

			orbital_EOMS_TEST::~orbital_EOMS_TEST() {}

			Matrix orbital_EOMS_TEST::operator() ( const double &t, const Matrix &y )
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

				// Drag Perturbations
				double Cd = 2.2; // kg
				double A = 40; // m^2
				double m = 100; // kg
				double h = r - RE;
				double h0 = 900; // km
				double H = 181.05; // km
				double rho0 = 5.245e-15; // kg/m^3
				double rho = rho0*exp(-(h-h0)/H);
				Matrix vel = y(colon(4, 6));
				Matrix vrel = vel; // km/s, doesn't take into account rotating wind
				Matrix aDrag = (-0.5)*(Cd*A/m)*rho*pow(norm(vrel), 2.0)*vrel/norm(vrel)*1000; // 1000 fixes units (m->km)

				// dy
				Matrix dy = A_*y;
				dy.setValue(4, 1, dy.getValue(4, 1) + aDrag.getValue(1, 1));
				dy.setValue(5, 1, dy.getValue(5, 1) + aDrag.getValue(2, 1));
				dy.setValue(6, 1, dy.getValue(6, 1) + aDrag.getValue(3, 1));
				return dy;
			}

		}// end namespace eoms
	} // end namespace util
} // end namespace horizon

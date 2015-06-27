#ifndef EOMS_H
#define EOMS_H

#include "..\Matrix.h"

namespace horizon {
	namespace util {
		namespace eoms {

			using horizon::util::Matrix;

			/**
			 * An equation of motion object with the parameters governing some
			 * sort of motion of an object.
			 * @author Cory O'Connor
			 */
			class EOMS
			{

			public:

				/**
				 * Creates an equation of motion object.
				 */
				EOMS();

				/** Destroys the equation of motion object */
				virtual ~EOMS() = 0;

				/**
				 * Computes one timestep of the equations of motion object from a specified
				 * state after a specified period of time
				 * @param t the time at which to evaluate the function
				 * @param y the current state of the object
				 * @return dy the matrix result
				 */
				virtual Matrix operator() ( const double &t, const Matrix &y ) = 0;
			};

		} // end namespace eoms
	} // end namespace util
} // end namespace horizon

#endif /*EOMS_H_*/

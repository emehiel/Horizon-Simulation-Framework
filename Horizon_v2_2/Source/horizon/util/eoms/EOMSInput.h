#ifndef EOMSINPUT_H
#define EOMSINPUT_H

#include "../Matrix.h"

namespace horizon {
	namespace util {
		namespace eoms {

			using horizon::util::Matrix;

			/**
			 * @author Cory O'Connor
			 */
			class EOMSInput
			{

			public:

				/**
				 * Creates an equation of motion object.
				 */
				EOMSInput();

				/** Destroys the equation of motion object */
				virtual ~EOMSInput() = 0;

				/**
				 */
				virtual Matrix eval( const double &t, const Matrix &y ) const = 0;
			};

		}// end namespace eoms
	} // end namespace util
} // end namespace horizon

#endif /*EOMSINPUT_H_*/

#ifndef POLYEOMSINPUT_H
#define POLYEOMSINPUT_H

#include "EOMSInput.h"

namespace horizon {
	namespace util {
		namespace eoms {

			using horizon::util::Matrix;

			/**
			* @author Cory O'Connor
			*/

			class polyEOMSInput : public EOMSInput 
			{

			public:
				/**
				* 
				*/
				polyEOMSInput( Matrix& min );

				/**
				* Virtual Destructor.
				*/
				~polyEOMSInput();

				/**
				*
				*/
				Matrix eval ( const double &t, const Matrix &y ) const;

			private:
				/**
				*
				*/
				double c1_, c2_, c3_, c4_, c5_, c6_, c7_, c8_;

			};

		} // end namespace eoms
	} // end namespace util
} // end namespace horizon

#endif // POLYEOMSINPUT_H_

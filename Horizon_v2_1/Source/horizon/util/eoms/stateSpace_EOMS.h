#ifndef STATESPACE_EOMS_H
#define STATESPACE_EOMS_H

#include "EOMS.h"
#include "EOMSInput.h"

namespace horizon {
	namespace util {
		namespace eoms {

			/**
			* @author Cory O'Connor
			*/

			class stateSpace_EOMS : public EOMS 
			{

			public:
				/**
				* 
				*/
				stateSpace_EOMS( const Matrix& A );
				
				/**
				* 
				*/
				stateSpace_EOMS( EOMSInput* EOMin, const Matrix& A, const Matrix& B );

				/**
				* 
				*/
				stateSpace_EOMS( EOMSInput* EOMin, const Matrix& A, const Matrix& B, const Matrix& C );

				/**
				* 
				*/
				stateSpace_EOMS( EOMSInput* EOMin, const Matrix& A, const Matrix& B, const Matrix& C, const Matrix& D );

				/**
				* Virtual Destructor.
				*/
				~stateSpace_EOMS();

				/**
				*
				*/
				Matrix operator() ( const double &t, const Matrix &y );
			
			private:
				/**
				*
				*/
				Matrix A_;
				Matrix B_;
				Matrix C_;
				Matrix D_;
				EOMSInput* input_;

			};

		} // end namespace eoms
	}	// end namespace util
}  // end namespace horizon

#endif // STATESPACE_EOMS_H_

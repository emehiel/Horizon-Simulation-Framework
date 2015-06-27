#include "stateSpace_EOMS.h"

namespace horizon {
	namespace util {
		namespace eoms {

			stateSpace_EOMS::stateSpace_EOMS( const Matrix& A ) : A_(A) {}

			stateSpace_EOMS::stateSpace_EOMS( EOMSInput* EOMin, const Matrix& A, const Matrix& B ) : 
			input_(EOMin), A_(A), B_(B)  {}

			stateSpace_EOMS::stateSpace_EOMS( EOMSInput* EOMin, const Matrix& A, const Matrix& B, const Matrix& C ) : 
			input_(EOMin), A_(A), B_(B), C_(C) {}

			stateSpace_EOMS::stateSpace_EOMS( EOMSInput* EOMin, const Matrix& A, const Matrix& B, const Matrix& C, const Matrix& D ) : 
			input_(EOMin), A_(A), B_(B), C_(C), D_(D) {}

			stateSpace_EOMS::~stateSpace_EOMS() {}

			Matrix stateSpace_EOMS::operator() ( const double &t, const Matrix &y )
			{
				Matrix u = input_->eval(t, y);
				Matrix dx = A_*y + B_*u;
				Matrix dy = C_*dx + D_*u;
				return dy;
			}
	
		} // end namespace eoms
	} // end namespace util
} // end namespace horizon

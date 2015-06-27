#include "polyEOMSInput.h"

namespace horizon {
	namespace util {
		namespace eoms {

			polyEOMSInput::polyEOMSInput( Matrix& min ) : c1_(min.getValue(1,1)), c2_(min.getValue(2,1)), 
			c3_(min.getValue(3,1)), c4_(min.getValue(4,1)), c5_(min.getValue(5,1)), 
			c6_(min.getValue(6,1)), c7_(min.getValue(7,1)), c8_(min.getValue(8,1)) {}

			polyEOMSInput::~polyEOMSInput() {}

			Matrix polyEOMSInput::eval ( const double &t, const Matrix &y ) const
			{
				double u =  c1_ + t*(c2_ + t*(c3_ + t*(c4_ + t*(c5_ + t*(c6_ + t*(c7_ + t*(c8_)))))));
				Matrix input(1, 1, u);
				return input;
			};

		} // end namespace eoms
	} // end namespace util
} // end namespace horizon

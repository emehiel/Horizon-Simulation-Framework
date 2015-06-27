#include "matrixIndex.h"

namespace horizon {
namespace util {


//matrixIndex colon(void) {
//	CMatrixIndex rv;
//	return (rv);
//}

matrixIndex colon(const unsigned idx) {
	return (matrixIndex(idx));
}

matrixIndex colon(const unsigned start,
		   const unsigned end) {
	return (matrixIndex(start, end));
}

	}}  // end namespace horizon::util
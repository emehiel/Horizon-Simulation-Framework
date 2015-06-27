#ifndef ORBITAL_EOMS_H
#define ORBITAL_EOMS_H

#include "EOMS.h"

namespace horizon {
namespace util {
namespace eoms {

	/**
	* @author Cory O'Connor
	*/

	class orbital_EOMS : public EOMS 
	{

	public:
		/**
		* 
		*/
		orbital_EOMS( Matrix& A );

		/**
		* 
		*/
		orbital_EOMS();

		/**
		* 
		*/
		virtual ~orbital_EOMS();

		/**
		*
		*/
		virtual Matrix operator() ( const double &t, const Matrix &y );

	private:
		/**
		*
		*/
		const double mu_;

		Matrix A_;

	};

} // end namespace eoms
} // end namespace util
} // end namespace horizon

#endif // ORBITAL_EOMS_H_

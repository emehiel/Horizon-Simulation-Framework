#include "Quadrant.h"

namespace horizon{
	namespace geom{

void Quadrant::setValues(int quadNum, Matrix TL,Matrix TR,Matrix BL,Matrix BR)
		{
			quadrantNumber = quadNum;
			quadBoundryTL = TL;
			quadBoundryTR = TR;
			quadBoundryBL = BL;
			quadBoundryBR = BR;
		};

Matrix Quadrant::writeTL()
		{
			return quadBoundryTL;
		};
Matrix Quadrant::writeTR()
		{
			return quadBoundryTR;
		};
Matrix Quadrant::writeBL()
		{
			return quadBoundryBL;
		};
Matrix Quadrant::writeBR()
		{
			return quadBoundryBR;
		};
//Quadrant::~Quadrant(){};
	}
}
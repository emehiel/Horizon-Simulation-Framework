#ifndef QUADLIST_H
#define QUADLIST_H

#include <list>
#include "horizon/geom/Station.h"

namespace horizon{
	namespace geom{

	using namespace std;
	using horizon::geom::Station;


class Quadrant
{
private:
	int quadrantNumber;
	Matrix quadBoundryTL;
	Matrix quadBoundryTR;
	Matrix quadBoundryBL;
	Matrix quadBoundryBR;

public:
	Quadrant() {};
	void setValues(int quadNum, Matrix TL,Matrix TR,Matrix BL,Matrix BR);
	Matrix writeTL();
	Matrix writeTR();
	Matrix writeBL();
	Matrix writeBR();
	~Quadrant(){}
};
	}
}
#endif
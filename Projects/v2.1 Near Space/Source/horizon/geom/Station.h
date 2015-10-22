#ifndef STATION_H
#define STATION_H


#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <map>
#include "horizon/util/Data/windata.h"
#include "horizon/util/Data/Atmosdata.h"
#include "horizon/util/Matrix.h"
#include "horizon/util/matrixIndex.h"
#include "horizon/util/Tools/Tools.h"


namespace horizon{
	namespace geom{



	using namespace std;

	using horizon::util::Matrix;
	using horizon::util::matrixIndex;
	using horizon::util::norm;
	using horizon::util::Data::Atmosdata;




class Station
	{
	private:
		int Quadrantnum;
		string Station_Name;
		Matrix Pos;

		list<double> Datelist;


	protected:
		
	public:
		Station() {};
		void setvalues(Matrix Position, int quadrant,string name, map<double,Windata> windmap, map<double,Atmosdata> atmosmap);
		map <double,Atmosdata> LoadAtmosData();
		map<double,Windata> Windmap;
		map<double,Atmosdata> Atmosmap;
		string writename();
		Matrix writePos();
		int writeQuadrant();
		map <double,Windata>* writeWindData();
		list<double> writeDateList();
		map <double,Atmosdata>* writeAtmosData();
		~Station(){}
	};
	}
}
#endif
	
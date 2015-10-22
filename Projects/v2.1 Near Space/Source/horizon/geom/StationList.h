#ifndef STATIONLIST_H
#define STATIONLIST_H

#include "horizon/geom/Station.h"
#include "horizon/util/Data/Atmosdata.h"
#include "horizon/util/Data/windata.h"
#include "horizon/util/Tools/tools.h"
#include "horizon/io/xmlParser.h"
#include "horizon/geom/Quadrant.h"
#include "horizon/util/Matrix.h"
#include <fstream>
#include <iostream>

using std::list;
using std::string;
using std::ifstream;
using horizon::util::Data::Windata;
using horizon::util::Matrix;
using horizon::geom::Quadrant;
using horizon::geom::Station;
using horizon::io::XMLNode;


class StationList
{
	

private:
	map<int,Quadrant> quadMap;
	XMLNode scratchPadXMLNode;
	static StationList* pinstance;
	double station_count;
	int MeridionalGrid;
	int ZonalGrid;
	int initnumCols;
	int initnumRows;
	double quadHeight;
	double quadWidth;

	vector<list<Station*>*>* quads;
	vector<list<int>*>* adjQuads;
	

public:
	static StationList* Instance();
	void LoadStationData(string stationlist,string heightdata,string zonaldata,string meridonaldata);
	map<string,map<double,Windata>> LoadWindData(const string& heightFileName,const string& zonalVelFileName,const string& meridonalVelFileName);
	vector<list<Station*>*>* writeQuads();
	map<int,Quadrant> writeQuadMap();
	Station* findstation(list<Station*>* local_list,double ballat,double ballong);
	int quadrantLocator(double lat, double lon);
	void proximityGrid();
	void createGrid(XMLNode scratchPadXMLNode);
	void findAdjacencies(int quadNum);
	vector<list<int>*>* writeAdjcntQuads();
protected:
	StationList();
	~StationList();

};

#endif

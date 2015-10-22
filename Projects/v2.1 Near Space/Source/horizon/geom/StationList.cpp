#include "StationList.h"
#include "horizon/io/xmlParser.h"


StationList* StationList::pinstance = 0; //initializes pointer

StationList* StationList::Instance()
{
	if (pinstance ==0)
	{
		pinstance = new StationList();
	}
	return pinstance;
}
//================================================================================================================
StationList::StationList()
{

}
//===============================================================================================================
vector<list<Station*>*>* StationList::writeQuads()
{
	return quads;
}
//================================================================================================================
map<int,Quadrant> StationList::writeQuadMap()
{
	return quadMap;
}
//================================================================================================================
void StationList::LoadStationData(string stationlist,string heightdata,string zonaldata,string meridonaldata)
{
	std::cout<<"Initializing Station List..."<<std::endl;
	Matrix Pos1;
	string line;
	string data_line;
	int quadrant;
	list<double> datelist;
	map<string ,map<double,Windata>> masterWindMap;
	map<double,Windata> localWindMap;
	map<double,Atmosdata> atmosmap;
	map<double,Windata>::reverse_iterator rit;
	pair<map<double,Windata>,list<double>> datewinddata;
	ifstream myfile;
	myfile.open(stationlist.c_str(), ifstream::in);
	Station* station1;
	double geopotHeight;
	double zonalVel;
	double meridonalVel;
	Windata winddata;
	
	masterWindMap = LoadWindData(heightdata,zonaldata,meridonaldata);
//	masterWindMap = (geopotential height in meters,meters/sec,meters/sec)

		if(myfile.is_open())
		{
			quads = new vector<list<Station*>*>();
			for (int i = 0; i <= 72; i++)
			{
				list<Station*>* temp =  new list<Station*>();
				quads->push_back(temp);
			}
			while (!myfile.eof())
			{
				char s[256];
				myfile.getline(s, 256);

				string line(s);


				if(!line.empty())
				//get station information from data
				//The latitude and longitude of the stations are in decimal degrees so the balloons
				//start location must be entered as such.
				{
					station1 = new Station();
					string stationname = line.substr(11,36);
					string filename = line.substr(4,5);

					printf("Loading Station:  %s\r", stationname.c_str());

					string station_lat = line.substr(47,6);
					double lat = (double) atof(station_lat.c_str());
									
					string station_long = line.substr(54,7);
					double lon = (double) atof(station_long.c_str());
					
					string station_alt = line.substr(62,4);
					double geomalt = (double) atof(station_alt.c_str());
					double alt = GeometAltToGeopotheight(geomalt);
					Pos1 = StationLocation(lat,lon,alt);	
	
					//datewinddata = station1->LoadWindData(filename);
					localWindMap = masterWindMap[filename];
					//rit = windmap.rbegin();
					//if(!windmap.empty())
					//	{
					//	double g = rit->first;
					//		if( g>=balalt)
					//		{*/
							//datelist = datewinddata.second;
							atmosmap = station1->LoadAtmosData();
							quadrant = quadrantLocator(lat,lon);
							bool localWindmapTest = localWindMap.empty();
							if(localWindmapTest == false)
							{
							station1->setvalues(Pos1,quadrant,stationname,localWindMap,atmosmap);
							quads->at(quadrant)->push_back(station1);
							}
				}
			}		
				myfile.close();
				masterWindMap.clear();

		}
		
}

//=========================================================================================================================
Station* StationList::findstation(list<Station*>* local_list,double ballat, double ballong)
{
	list<double> distlist;
	map<double,Station> distmap;
	list<Station>::iterator stationinfo;
	Matrix balLoc (2,1,0.0);	
	balLoc.setValue(1,1,ballat);
	balLoc.setValue(2,1,ballong);	
	Matrix StatLoc (2,1,0.0);
	double closest = 9999999999.9;
	Station* closestStation = *(local_list->begin());
	Matrix Pos = closestStation->writePos();

	int i = 0;
	for (list<Station*>::iterator stationinfo = local_list->begin(); stationinfo != local_list->end(); stationinfo++, i++)
	{

		Pos = (*stationinfo)->writePos();
		double statlat = Pos.getValue(1,1);
		double statlon = Pos.getValue(2,1);
		StatLoc.setValue(1,1,statlat);
		StatLoc.setValue(2,1,statlon);

		double dist = norm(balLoc - StatLoc);
		
		if (dist < closest)
		{
			closest = dist;
			closestStation = *stationinfo;
		}
	return closestStation;
	}
}

//====================================================================================================
map<string,map<double,Windata>> StationList::LoadWindData(const string& heightFileName,const string& zonalVelFileName,const string& meridonalVelFileName)
	{
		ifstream ghgt;
		list <double> ghgtList;
		ifstream uwind;
		ifstream vwind;
		Windata windata;
		double ID;
		double zonalVel;
		double meridonalVel;
		double geopotHeight;
		map<double,Windata> heightMap;
		map<string,map<double,Windata>> windMap;
		
		ghgt.open(heightFileName.c_str(), ifstream::in);
		uwind.open(zonalVelFileName.c_str(),ifstream::in);
		vwind.open(meridonalVelFileName.c_str(),ifstream::in);	

		string oldIDstring = "";

		if(ghgt.is_open())
		{
			if(uwind.is_open())
			{
				if(vwind.is_open())
				{
					while (!ghgt.eof())
					{
						char q[256];
						char w[256];
						char e[256];
						ghgt.getline(q,256);
						uwind.getline(w,256);
						vwind.getline(e,256);
						string line1(q);
						string line2(w);
						string line3(e);
						string IDstring = line1.substr(0,5);
						cout<<IDstring<<endl;
						string geopotHeightString = line1.substr(19,5);
						string zonalVelstring = line2.substr(21,3);
						string meridonalVelstring = line3.substr(21,3);
						//ID = (double)atof(IDstring.c_str());
						geopotHeight = (double)atof(geopotHeightString.c_str());
						zonalVel = ((double)atof(zonalVelstring.c_str()))/10;
						meridonalVel = ((double)atof(meridonalVelstring.c_str()))/10;
						windata.setvalues(zonalVel,meridonalVel);

						
						if (oldIDstring == "")
							oldIDstring = IDstring;

						if (IDstring != oldIDstring)
						{
							windMap.insert(make_pair(oldIDstring,heightMap));
							oldIDstring = IDstring;
							heightMap.clear();
						}
						heightMap.insert(pair<double,Windata>(geopotHeight,windata));
						
										
						//windMap.insert(pair<string,map<double,Windata>>(IDstring,heightMap));

					}
					vwind.close();
					uwind.close();
					ghgt.close();
					heightMap.insert(pair<double,Windata>(geopotHeight,windata));
				}
			}
		}
		return windMap;
}
//======================================================================================================================
int StationList::quadrantLocator(double lat, double lon)
{
	int numCols = initnumCols;
	int quadrant;
	lon+=180;
	lat-=90;
	lat*=(-1);


	double col = ceil(lon/quadWidth);
	double row = ceil(lat/quadHeight);

	quadrant = (row-1)*numCols+col;
	
	return quadrant;
}
//======================================================================================================================
void StationList::createGrid(XMLNode scratchPadXMLNode)
{
	initnumCols = atoi(scratchPadXMLNode.getAttribute("numCols"));
	initnumRows = atoi(scratchPadXMLNode.getAttribute("numRows"));
	int numRows = initnumRows;
	int numCols = initnumCols;
	quadHeight = (180/numRows);
	quadWidth = (360/numCols);
	list<Station> stations (station_count);
	adjQuads = new vector<list<int>*>();

	for (int i = 0; i <= (numCols*numRows); i++)
	{
		list<int>* temp =  new list<int>();
		adjQuads->push_back(temp);
	}

	int quadrant = 1;

	while (numRows>0)
	{
		while (numCols>0)
		{
			cout<<quadrant;
			findAdjacencies(quadrant);
			quadrant++;
			numCols--;		
		}
		quadrant +=numCols;
		numCols = atoi(scratchPadXMLNode.getAttribute("numCols"));
		numRows--;
	}
}
//=======================================================================================================
void StationList::findAdjacencies(int quadNum)
{

	int numRows = initnumRows;
	int numCols = initnumCols;
	int counter = 1;
	int adjQuad;
	
	int frstOfLstRow = (numRows*numCols-numCols)+1;
	int totGridNum = numRows*numCols;

	if (quadNum <= numCols)
	{
		while (counter<=numCols)
		{
			adjQuads->at(quadNum)->push_back(counter);
			counter++;
		}
		if(quadNum ==1)
		{
			adjQuad = quadNum+numCols;
			adjQuads->at(quadNum)->push_back(adjQuad);
			adjQuad = numCols*2;
			adjQuads->at(quadNum)->push_back(adjQuad);
			adjQuad = quadNum+numCols+1;
			adjQuads->at(quadNum)->push_back(adjQuad);
		}
		else if(quadNum == numCols)
		{
			adjQuad = quadNum+numCols;
			adjQuads->at(quadNum)->push_back(adjQuad);
			adjQuad = quadNum+numCols-1;
			adjQuads->at(quadNum)->push_back(adjQuad);
			adjQuad = quadNum+1;
			adjQuads->at(quadNum)->push_back(adjQuad);
		}
		else
		{
			adjQuad = quadNum+numCols;
			adjQuads->at(quadNum)->push_back(adjQuad);
			adjQuad = quadNum+numCols-1;
			adjQuads->at(quadNum)->push_back(adjQuad);
			adjQuad = quadNum+numCols+1;
			adjQuads->at(quadNum)->push_back(adjQuad);
		}
	}

	else if(quadNum >= frstOfLstRow && quadNum <= totGridNum)
	{
		while (counter<=numCols)
		{
			adjQuads->at(quadNum)->push_back(frstOfLstRow);
			frstOfLstRow+=1;
			counter++;
		}
		if(quadNum ==totGridNum-numCols+1)
		{
			adjQuad = quadNum-1;
			adjQuads->at(quadNum)->push_back(adjQuad);
			adjQuad = quadNum-numCols;
			adjQuads->at(quadNum)->push_back(adjQuad);
			adjQuad = quadNum-numCols+1;
			adjQuads->at(quadNum)->push_back(adjQuad);
		}
		else if(quadNum == totGridNum)
		{
			adjQuad = quadNum-numCols;
			adjQuads->at(quadNum)->push_back(adjQuad);
			adjQuad = quadNum-(numCols+1);
			adjQuads->at(quadNum)->push_back(adjQuad);
			adjQuad = quadNum-(2*numCols-1);
			adjQuads->at(quadNum)->push_back(adjQuad);
		}
		else
		{
			adjQuad = quadNum-numCols;
			adjQuads->at(quadNum)->push_back(adjQuad);
			adjQuad = quadNum-numCols+1;
			adjQuads->at(quadNum)->push_back(adjQuad);
			adjQuad = quadNum-numCols-1;
			adjQuads->at(quadNum)->push_back(adjQuad);
		}
	}
	else if(((quadNum-1)%numCols)==0)
	{
		adjQuad = quadNum - numCols;
		adjQuads->at(quadNum)->push_back(adjQuad);
		adjQuad = quadNum + numCols;
		adjQuads->at(quadNum)->push_back(adjQuad);
		adjQuad = quadNum - numCols+1;
		adjQuads->at(quadNum)->push_back(adjQuad);
		adjQuad = quadNum + numCols+1;
		adjQuads->at(quadNum)->push_back(adjQuad);
		adjQuad = quadNum+1;
		adjQuads->at(quadNum)->push_back(adjQuad);
		adjQuad = quadNum + numCols - 1;
		adjQuads->at(quadNum)->push_back(adjQuad);
		int LsideQuad = quadNum-numCols+(numCols-1);
		int RsideQuad = quadNum+numCols+(numCols-1);
		if(LsideQuad<0)
			LsideQuad = numCols;
		adjQuads->at(quadNum)->push_back(LsideQuad);
		if(RsideQuad>totGridNum)
			RsideQuad = totGridNum-numCols;
		adjQuads->at(quadNum)->push_back(RsideQuad);
	}
	else
	{
		adjQuad = quadNum -1;
		adjQuads->at(quadNum)->push_back(adjQuad);
		adjQuad = quadNum+1;
		adjQuads->at(quadNum)->push_back(adjQuad);
		adjQuad = quadNum-numCols;
		adjQuads->at(quadNum)->push_back(adjQuad);
		adjQuad = quadNum+numCols;
		adjQuads->at(quadNum)->push_back(adjQuad);
		adjQuad = (quadNum-numCols)+1;
		adjQuads->at(quadNum)->push_back(adjQuad);
		adjQuad = (quadNum-numCols)-1;
		adjQuads->at(quadNum)->push_back(adjQuad);
		adjQuad = (quadNum+numCols)+1;
		adjQuads->at(quadNum)->push_back(adjQuad);
		adjQuad = (quadNum+numCols)-1;
		adjQuads->at(quadNum)->push_back(adjQuad);
	}
}
//========================================================================================================
vector<list<int>*>* StationList::writeAdjcntQuads()
{
	return adjQuads;
}
//StationList::~StationList()
//{}

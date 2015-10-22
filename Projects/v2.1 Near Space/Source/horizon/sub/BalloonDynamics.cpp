#include "BalloonDynamics.h"
#include "dep/Dependencies.h"

namespace horizon{
namespace sub{

	using namespace horizon::sub::dep;

Balloon_Dynamics::Balloon_Dynamics(vector<XMLNode> subsystemXMLNodeList):Subsystem(SUBNAME_BALLOON)
{
	//stepNum = atof(subsystemXMLNodeList.at(0).getAttribute("stepNum"));
	balMass = atof(subsystemXMLNodeList.at(0).getAttribute("balloonMass"));
	gasMass = atof(subsystemXMLNodeList.at(0).getAttribute("gasMass"));
	rhoGas = atof(subsystemXMLNodeList.at(0).getAttribute("rhoGas"));
	maxDiameter = atof(subsystemXMLNodeList.at(0).getAttribute("maxDiameter"));
	mPayload = atof(subsystemXMLNodeList.at(0).getAttribute("payloadMass"));

	addKey(BALLGEOM_KEY);
	addKey(BALLPOS_KEY);
}	

Balloon_Dynamics::Balloon_Dynamics(XMLNode balloonNode):Subsystem (balloonNode.getAttribute("subsystemName")),
	balMass(atof(balloonNode.getAttribute("balloonMass"))),
	gasMass(atof(balloonNode.getAttribute("gasMass"))),
	rhoGas(atof(balloonNode.getAttribute("rhoGas"))),
	maxDiameter(atof(balloonNode.getAttribute("maxDiameter"))),
	mPayload(atof(balloonNode.getAttribute("payloadMass")))
	//stepNum(atof(balloonNode.getAttribute("stepNum")))
	
{
	addKey(BALLPOS_KEY);
	addKey(BALLGEOM_KEY);
}

Balloon_Dynamics::~Balloon_Dynamics(){}

bool Balloon_Dynamics::canPerform(const State* oldState, State* newState, const Task* task,Position* position, Environment* environment, NodeDependencies* deps) const
{
	double es = newState->getEventStart();
	double ts = newState->getTaskStart();
	double te = newState->getTaskEnd();
	double ee = newState->getEventEnd();
	double t = 5;

	
	list<int>* adj_list;
	list<int>::iterator adj_list_iterator;
	
	if(ee>simParams::SIMEND_SECONDS())
		return false;
	
	StationList* stations = StationList::Instance();
	
	//Get old Position
	Matrix oldPos = oldState->getLastValue(BALLPOS_KEY).second;

	//Get old Velocity
	Matrix oldVel = oldState->getLastValue(BALLVEL_KEY).second;

	//Get old atmospheric attributes
	Matrix oldAttribs = oldState->getLastValue(BALLGEOM_KEY).second;


	Station* nearby;
	const double bal_alt = oldPos.getValue(1,3);
	double geoPotHeight = GeometAltToGeopotheight(bal_alt);
	const double bal_lat = oldPos.getValue(1,1);
	const double bal_long = oldPos.getValue(1,2);
	const double merid_vel = oldVel.getValue(1,2);
	const double zonal_vel = oldVel.getValue(1,1);
	const double alt_vel = oldVel.getValue(1,3);
	int quadrant = stations->quadrantLocator(bal_lat,bal_long);
	list<Station*>* quad_list = stations->writeQuads()->at(quadrant);
	while(quad_list->empty())
	{
		adj_list = stations->writeAdjcntQuads()->at(quadrant);
		adj_list->sort();
		adj_list->unique();
		for(adj_list_iterator = adj_list->begin();adj_list_iterator != adj_list->end(); adj_list_iterator++)
		{
			list<Station*>* adjQuadStations = stations->writeQuads()->at(*adj_list_iterator);
			adjQuadStations->sort();
			quad_list->merge(*adjQuadStations);
		}
		if(quad_list->empty())
		{
			for(adj_list_iterator = adj_list->begin();adj_list_iterator != adj_list->end(); adj_list_iterator++)
			{
				adj_list = stations->writeAdjcntQuads()->at(*adj_list_iterator);
				for(adj_list_iterator = adj_list->begin();adj_list_iterator != adj_list->end(); adj_list_iterator++)
				{
					list<Station*>* adjQuadStations = stations->writeQuads()->at(*adj_list_iterator);
					quad_list->merge(*adjQuadStations);
				}

			}
		}
	}
	quad_list->unique();
	nearby = stations->findstation(quad_list,bal_lat,bal_long);
	if(bal_alt>0)
	{
		Matrix statPosition = nearby->writePos();
		double station_Alt = statPosition.getValue(1,3);
		pair<Windata,Atmosdata> exact = ExactValues(geoPotHeight+station_Alt,nearby->writeWindData(),nearby->writeAtmosData());
		pair<double,double> ballgeom = BallGeom(oldAttribs,exact,gasMass);
		const double vol_balloon = ballgeom.second;
		const double fnt_area = ballgeom.first;
		const double JD = simParams::SIMSTART_JD()+(es/86000);
		
		double equitorial_radius = pow(vol_balloon*(3/(4*M_PI)),(.33333333333333));;
		double maxRadius = maxDiameter/2;

		if (equitorial_radius <= maxRadius)
		{
			//const double rhogas = gasMass/vol_balloon;
			const double zone_wind = exact.first.writezonal_component();
			const double merid_wind = exact.first.writemeridional_component();
			const double density = exact.second.writedensity();
			const double visc = exact.second.writevis();




			double mdata[] = {bal_lat, bal_long, bal_alt,
							  zonal_vel,merid_vel,alt_vel, 
							  zone_wind,merid_wind, JD,
							  fnt_area,vol_balloon,equitorial_radius,
							  mPayload,balMass,gasMass,
							  visc,density,rhoGas};
					 
			if (alt_vel>20)
				cout<<"oops"<<endl;

			if (zonal_vel>50)
				cout<<"oops"<<endl;

			if (merid_vel>50)
				cout<<"oops"<<endl;		 

			Matrix balcurrstate = Matrix(6,3, mdata);
			Matrix  balProf= position->getEOMS()->operator ()(t,balcurrstate);
			
			double ballData[] = {exact.second.writetemp(),exact.second.writepressure(),balProf.getValue(4,2)};
			Matrix ballGeomChar = Matrix(3,1,ballData);


			if (balProf(1,2) < -360.0)
				int teieie = 7;

			newState->addValue(BALLPOS_KEY, make_pair(t+es,balProf.getRow(1)));
			newState->addValue(BALLVEL_KEY, make_pair(t+es,balProf.getRow(2)));
			newState->addValue(BALLGEOM_KEY, make_pair(t+es,ballGeomChar));
			
			newState->setTaskEnd(t+es);
			double newAlt = balProf.getValue(1,3);
			if(newAlt<1)
			{
				//cout<<"The Balloon has Crashed."<<endl;
				return false;
			}
			

			
			return true;
		}
		else
		{
			//cout<<"The balloon has exceeded it Maximum Radius and burst."<<endl;
			return false;
		}
	}
	else
	{ 
		//cout<<"The Balloon has Crashed."<<endl;
		return false;
	}


}

bool Balloon_Dynamics::canExtend(State* state, Position* position, Environment* environment, const double evalToTime, NodeDependencies* deps) const
{

	return true;
}
}
}

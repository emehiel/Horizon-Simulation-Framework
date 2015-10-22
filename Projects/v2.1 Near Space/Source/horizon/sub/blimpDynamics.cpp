#include "blimpDynamics.h"
#include "dep/Dependencies.h"

namespace horizon{
	namespace sub{

		using namespace horizon::sub::dep;

Blimp_Dynamics::Blimp_Dynamics(vector<XMLNode> subsystemXMLNodeList):Subsystem(SUBNAME_BLIMP)
{
	blimpLength = atof(subsystemXMLNodeList.at(0).getAttribute("blimpLength"));
	blimpDiam = atof(subsystemXMLNodeList.at(0).getAttribute("blimpDiam"));
	rhoGas = atof(subsystemXMLNodeList.at(0).getAttribute("rhoGas"));
	blimpPayMass = atof(subsystemXMLNodeList.at(0).getAttribute("blimpPayMass"));
	blimpMass = atof(subsystemXMLNodeList.at(0).getAttribute("initBlimpMass"));
	cruiseAlt = atof(subsystemXMLNodeList.at(0).getAttribute("cruiseAlt"));
	initVolGas = atof(subsystemXMLNodeList.at(0).getAttribute("initVolGas"));
	MolarMass = atof(subsystemXMLNodeList.at(0).getAttribute("MolarMass"))/1000;
	mSys = atof(subsystemXMLNodeList.at(0).getAttribute("mSys"));
	AltVel = atof(subsystemXMLNodeList.at(0).getAttribute("AltVel"));

	addKey(BLIMPPOS_KEY);
	addKey(BLIMPVEL_KEY);
	addKey(BLIMPTHRUST_KEY);

}
Blimp_Dynamics::Blimp_Dynamics(XMLNode blimpNode):Subsystem(blimpNode.getAttribute("subsystemName")),
blimpLength(atof(blimpNode.getAttribute("blimpLength"))),blimpDiam(atof(blimpNode.getAttribute("blimpDiam"))),
blimpPayMass(atof(blimpNode.getAttribute("blimpPayMass"))),blimpMass(atof(blimpNode.getAttribute("initBlimpMass"))),
rhoGas(atof(blimpNode.getAttribute("rhoGas"))),cruiseAlt(atof(blimpNode.getAttribute("cruiseAlt"))),
initVolGas(atof(blimpNode.getAttribute("initVolGas"))),MolarMass(atof(blimpNode.getAttribute("MolarMass"))),
mSys(atof(blimpNode.getAttribute("mSys"))),AltVel(atof(blimpNode.getAttribute("AltVel")))
{
	addKey (BLIMPPOS_KEY);
	addKey (BLIMPVEL_KEY);
	addKey (BLIMPTHRUST_KEY);
}

Blimp_Dynamics::~Blimp_Dynamics(){}

bool Blimp_Dynamics::canPerform(const State *oldState, State *newState, const Task *task, Position *position, Environment *environment, NodeDependencies *dependencies) const
{	double es = newState->getEventStart();
	double te = newState->getTaskEnd();
	double ee = newState->getEventEnd();
	double t = 30;
	double R = 8.314472;	//Ideal Gas Constant in J/molK
	list<int>* adj_list;
	list<int>::iterator adj_list_iterator;

	if(ee>simParams::SIMEND_SECONDS())
		return false;
	
	StationList* stations = StationList::Instance();

	//Get old Position
	Matrix oldPos = oldState->getLastValue(mkeys.front()).second;

	//Get old Velocity
	Matrix oldVel = oldState->getLastValue(BLIMPVEL_KEY).second;

	//Get old Thrust
	double oldThrust = oldState->getLastValue(BLIMPTHRUST_KEY).second;

	

	Station* nearby;
	double blimp_alt = oldPos.getValue(1,3);
	double blimp_lat = oldPos.getValue(1,1);
	double blimp_long = oldPos.getValue(1,2);
	double merid_vel = oldVel.getValue(1,1);
	double zonal_vel = oldVel.getValue(1,2);
	double alt_vel = oldVel.getValue(1,3);
	if (blimp_alt>cruiseAlt)
		alt_vel = -AltVel;
	else if(blimp_alt<cruiseAlt)
		alt_vel = AltVel;
	else
		alt_vel = 0;
//============================================================================================================
	int quadrant = stations->quadrantLocator(blimp_lat,blimp_long);
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
	nearby = stations->findstation(quad_list,blimp_lat,blimp_long);
	double csTime = newState->getTaskStart();
					
	//Get Target
	Matrix Target = task->getTarget()->getPos()->getPosLLA(csTime);

	double targetLat = Target.getValue(1,1);
	double targetLong = Target.getValue(2,1);				
	double Lat_dist_to_target = targetLat - blimp_lat;
	double Long_dist_to_target = targetLong - blimp_long;

	

		double geoPotHeight = GeometAltToGeopotheight(blimp_alt);
		if(blimp_alt>0)
		{
			
			pair<Windata,Atmosdata> exact = ExactValues(geoPotHeight,nearby->writeWindData(),nearby->writeAtmosData());
			double density = exact.second.writedensity();
			double zone_wind = exact.first.writezonal_component();
			double merid_wind = exact.first.writemeridional_component();
			double visc = exact.second.writevis();
			double JD = simParams::SIMSTART_JD()+(es/86000);
			double Pressure = exact.second.writepressure();
			double Temp = exact.second.writetemp();
			double rho_gas = (Pressure*MolarMass)/(R*Temp);
			double Mgas = initVolGas*rhoGas;
 
			

			double blimpdata[] = {blimp_lat, blimp_long,blimp_alt,
								  zonal_vel, merid_vel,alt_vel, 
								  zone_wind, merid_wind,JD,
								  blimpPayMass,Mgas,blimpMass,
								  visc,density,oldThrust,
								  blimpLength,blimpDiam,rho_gas,
								  Lat_dist_to_target,Long_dist_to_target,mSys};

			if (alt_vel>20)
				cout<<"oops"<<endl;

			if (zonal_vel>50)
				cout<<"oops"<<endl;

			if (merid_vel>50)
				cout<<"oops"<<endl;		

			
			Matrix blimpcurrstate = Matrix(7,3,blimpdata);
			Matrix blimpProf = position->getEOMS()->operator ()(t,blimpcurrstate);

			newState->addValue(BLIMPPOS_KEY, make_pair(t+es,blimpProf.getRow(1)));
			newState->addValue(BLIMPVEL_KEY, make_pair(t+es,blimpProf.getRow(2)));
			newState->addValue(BLIMPTHRUST_KEY, make_pair(t+es,blimpProf.getValue(5,3)));
			newState->setTaskEnd(t+es);

			return true;
		}
		else
		{
			//cout<<"The Blimp has Crashed"<<endl;
			return false;
		}
}
bool Blimp_Dynamics::canExtend(horizon::State *state, Position *position, Environment *environment, const double evalToTime, NodeDependencies *deps) const
{
	return true;
}
//=========================================================================================================================================================


}
}

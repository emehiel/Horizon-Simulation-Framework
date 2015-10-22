#include "ssDynamics.h"
#include "dep/Dependencies.h"

namespace horizon{
namespace sub{

	using namespace horizon::sub::dep;
		
SS_Dynamics::SS_Dynamics(std::vector<XMLNode> subsystemXMLNodeList):Subsystem(SUBNAME_SS)
{
	rhoGas = atof(subsystemXMLNodeList.at(0).getAttribute("rhoGas"));
	ssmPayload = atof(subsystemXMLNodeList.at(0).getAttribute("ssmPayload"));
	sailLength = atof(subsystemXMLNodeList.at(0).getAttribute("sailLength"));
	sailWidth = atof(subsystemXMLNodeList.at(0).getAttribute("sailWidth"));
	sailMass = atof(subsystemXMLNodeList.at(0).getAttribute("sailMass"));
	tao = atof(subsystemXMLNodeList.at(0).getAttribute("tao"));
	zeroLift = atof(subsystemXMLNodeList.at(0).getAttribute("zeroLift"));
	effFact = atof(subsystemXMLNodeList.at(0).getAttribute("effFact"));
	maxDiameter = atof(subsystemXMLNodeList.at(0).getAttribute("maxDiameter"));
	ssGasMass = atof(subsystemXMLNodeList.at(0).getAttribute("gasMass"));
	balloonMass = atof(subsystemXMLNodeList.at(0).getAttribute("balloonMass"));

	addKey(SSGEOM_KEY);
	addKey(SSPOS_KEY);
	}

SS_Dynamics::SS_Dynamics(XMLNode ssNode): Subsystem (ssNode.getAttribute("subsystemName")),
	rhoGas(atof(ssNode.getAttribute("rhoGas"))),sailLength(atof(ssNode.getAttribute("sailLength"))),
	sailWidth(atof(ssNode.getAttribute("sailWidth"))),tao(atof(ssNode.getAttribute("tao"))),
	zeroLift(atof(ssNode.getAttribute("zeroLift"))),effFact(atof(ssNode.getAttribute("effFact"))),
	ssmPayload(atof(ssNode.getAttribute("ssmPayload"))),maxDiameter(atof(ssNode.getAttribute("maxDiameter"))),
	ssGasMass(atof(ssNode.getAttribute("gasMass"))),balloonMass(atof(ssNode.getAttribute("balloonMass"))),
	sailMass(atof(ssNode.getAttribute("sailMass")))
	{
		addKey (SSGEOM_KEY);
		addKey (SSPOS_KEY);
	}


SS_Dynamics::~SS_Dynamics(){}

bool SS_Dynamics::canPerform(const State* oldState, State* newState, const Task* task,Position* position, Environment* environment, NodeDependencies* deps) const
{
	double es = newState->getEventStart();
	double te = newState->getTaskEnd();
	double ee = newState->getEventEnd();
	double t = 30;
	list<int>* adj_list;
	list<int>::iterator adj_list_iterator;

	if(ee>simParams::SIMEND_SECONDS())
		return false;
	
	StationList* stations = StationList::Instance();

	//Get old Position
	Matrix oldPos = oldState->getLastValue(SSPOS_KEY).second;

	//Get old Velocity
	Matrix oldVel = oldState->getLastValue(SSVEL_KEY).second;

	//Get old atmospheric attributes
	Matrix oldAttribs = oldState->getLastValue(SSGEOM_KEY).second;

	Station* nearby;
	double ss_alt = oldPos.getValue(1,3);
	double sail_Alt = ss_alt-15;
	double ss_lat = oldPos.getValue(1,1);
	double ss_long = oldPos.getValue(1,2);
	double merid_vel = oldVel.getValue(1,2);
	double zonal_vel = oldVel.getValue(1,1);
	double alt_vel = oldVel.getValue(1,3);


	int quadrant = stations->quadrantLocator(ss_lat,ss_long);
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
		if(quad_list->size()== 0)
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
	nearby = stations->findstation(quad_list,ss_lat,ss_long);	
	double geoPotHeight = GeometAltToGeopotheight(ss_alt);
	if(sail_Alt>0)
	{
		Matrix statPosition = nearby->writePos();
		double station_Alt = statPosition.getValue(1,3);
		pair<Windata,Atmosdata> exact = ExactValues(geoPotHeight+station_Alt,nearby->writeWindData(),nearby->writeAtmosData());
		pair<Windata,Atmosdata> sailSectionExact = ExactValues(geoPotHeight+sail_Alt,nearby->writeWindData(),nearby->writeAtmosData());
		pair<double,double> ssgeom = ssGeom(oldAttribs,exact,ssGasMass);
		double vol_balloon = ssgeom.second;
		double fnt_area = ssgeom.first;
		double JD = simParams::SIMSTART_JD()+(es/86000);

		double ssequitorial_radius = pow(((vol_balloon/M_PI)*(.75)),0.33333333333333333333333333333333);
		double MaxRadius = maxDiameter/2;

		if (ssequitorial_radius <= MaxRadius)
		{
			double rhogas = ssGasMass/vol_balloon;
			double SSA = sailWidth*sailLength;
			double AR = sailLength/sailWidth;
			double zone_wind = exact.first.writezonal_component();
			double merid_wind = exact.first.writemeridional_component();
			double sail_zone_wind = sailSectionExact.first.writezonal_component();
			double sail_merid_wind = sailSectionExact.first.writemeridional_component();
			double densityAtSail = sailSectionExact.second.writedensity();
			double density = exact.second.writedensity();
			double visc = exact.second.writevis();
			double csTime = newState->getTaskStart();

			double alpha = determine_Alpha(oldPos,task->getTarget()->getPos()->getPosLLA(csTime));


			double ssdata[] = {ss_lat, ss_long,ss_alt,
							  zonal_vel, merid_vel,alt_vel, 
							  zone_wind, merid_wind,JD,
							  sail_zone_wind,sail_merid_wind,alpha,
							  fnt_area, vol_balloon,ssequitorial_radius,
							  ssmPayload,sailMass,balloonMass,
							  visc,density,ssGasMass,
							  densityAtSail,SSA,AR,
							  tao,effFact,zeroLift};
			
			if (abs(alt_vel)>20)
				cout<<"oops"<<endl;

			if (abs(zonal_vel)>50)
				cout<<"oops"<<endl;

			if (abs(merid_vel)>50)
				cout<<"oops"<<endl;	
			
			
			Matrix sscurrstate = Matrix(9,3, ssdata);
			Matrix ssProf= position->getEOMS()->operator ()(t,sscurrstate);

			double ssData[] = {exact.second.writetemp(),exact.second.writepressure(),ssProf.getValue(4,2)};
			Matrix ssGeomChar = Matrix(3,1,ssData);
			
			newState->addValue(SSPOS_KEY, make_pair(t+es,ssProf.getRow(1)));
			newState->addValue(SSVEL_KEY, make_pair(t+es,ssProf.getRow(2)));
			newState->addValue(SSGEOM_KEY, make_pair(t+es,ssGeomChar));
			
			newState->setTaskEnd(t+es);
			double newAlt = ssProf.getValue(1,3)-15;
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
bool SS_Dynamics::canExtend(State* state, Position* position, Environment* environment, const double evalToTime, NodeDependencies* deps) const
{
	return true;
}
//=============================================================================================================================================
double SS_Dynamics::determine_Alpha(Matrix balPos,Matrix target) const
{
	double targetLat = target.getValue(1,1);
	double targetLong = target.getValue(2,1);
	double balLat = balPos.getValue(1,1);
	double balLong = balPos.getValue(1,2);
	double Long_dist = targetLong-balLong;
	double Lat_dist = targetLat-balLat;
	double Lat_dist_mag = abs(Lat_dist);
	double Long_dist_mag = abs(Long_dist);
	double alpha = atan((Lat_dist_mag/Long_dist_mag));
	return alpha;
};

	}
}
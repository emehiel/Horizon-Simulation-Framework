#ifndef BALLOONDYNAMICS_H
#define BALLOONDYNAMICS_H

#include "../Subsystem.h"
#include "../util/eoms/balloon_EOMS.h"
#include "../geom/Station.h"
#include "../geom/StationList.h"
#include "../util/Tools/tools.h"
#include "../util/Data/Atmosdata.h"
#include "../util/Data/windata.h"
#include "../io/xmlParser.h"
#include "../util/Matrix.h"

namespace horizon {
	namespace sub {
		
		using namespace std;
		//using std::map;

		using horizon::geom::Station;
		using horizon::util::eoms::balloon_EOMS;
		using horizon::util::Data::Atmosdata;
		using horizon::util::Matrix;
		using horizon::util::atom;



class Balloon_Dynamics : public Subsystem {

	XMLNode subNode;

public:

	Balloon_Dynamics(vector<XMLNode> subsystemXMLNodeList);
	Balloon_Dynamics(XMLNode balloonNode);
	virtual ~Balloon_Dynamics();
	
	virtual bool canPerform(const State* oldState, State* newState, const Task* task, Position* position, Environment* environment, NodeDependencies* dependencies) const;
	virtual bool canExtend(State* state, Position* position, Environment* environment, const double evalToTime, NodeDependencies* deps) const;

	

private:
	Matrix balcurrstate;
	double stepNum;
	double bal_lat;
	double merid_vel;
	double density;
	double zone_vel;
	double merid_wind;
	double fnt_area;
	double zone_wind;
	double merid_Cd;
	double mPayload;
	double zone_Cd;
	double bal_alt;
	double vol_balloon;
	double bal_long;
	double alt_vel;
	double rho_gas;
	double equitorial_radius;
	double msys;
	double rhoGas;
	double balMass;
	double maxDiameter;
	double gasMass;
	double payloadMass;
	map<double,map<double,Windata>>datemap;
	map<double,Windata>windmap;
	map<double,Windata>::iterator it;
	map<double,Atmosdata>atmosmap;

};
	}
}
#endif
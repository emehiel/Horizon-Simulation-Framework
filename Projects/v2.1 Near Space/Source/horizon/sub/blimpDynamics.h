#ifndef BLIMPDYNAMICS_H
#define BLIMPDYNAMICS_H

#include "../Subsystem.h"
#include "../util/eoms/blimp_EOMS.h"
#include "../geom/Station.h"
#include "../geom/StationList.h"
#include "../util/Tools/tools.h"
#include "../util/Data/Atmosdata.h"
#include "../util/Data/windata.h"
#include "../io/xmlParser.h"
#include "../util/Matrix.h"

namespace horizon{
	namespace sub{

		using namespace std;

		using horizon::geom::Station;
		using horizon::util::eoms::blimp_EOMS;
		using horizon::util::Data::Atmosdata;
		using horizon::util::Matrix;
		using horizon::util::atom;

class Blimp_Dynamics : public Subsystem {

	XMLNode subNode;

public:

	Blimp_Dynamics(vector<XMLNode> subsystemXMLNodeList);
	Blimp_Dynamics(XMLNode blimpNode);
	virtual ~Blimp_Dynamics();

	virtual bool canPerform(const State* oldState, State* newState, const Task* task, Position* position, Environment* environment, NodeDependencies* dependencies) const;
	virtual bool canExtend(State* state, Position* position, Environment* environment, const double evalToTime, NodeDependencies* deps) const;
	

private:
	Matrix blimpCurrState;
	double blimp_Lat;
	double merid_vel;
	double density;
	double zone_vel;
	double merid_wind;
	double blimpFrontArea;
	double zone_wind;
	double blimpPayMass;
	double blimp_alt;
	double AltVel;
	double rhoGas;
	double blimpGasMass;
	double blimpMass;
	double Thrust;
	double blimpLength;
	double blimpDiam;
	double blimpSideArea;
	double blimpVol;
	double initVolGas;
	double cruiseAlt;
	double Mgas;
	double MolarMass;
	double mSys;
	map<double,map<double,Windata>>datemap;
	map<double,Windata>windmap;
	map<double,Windata>::iterator it;
	map<double,Atmosdata>atmosmap;

};
	}
}
#endif
	
	
#ifndef SSDYNAMICS_H
#define SSDYNAMICS_H
#define _USE_MATH_DEFINES

#include "../Subsystem.h"
#include "../util/eoms/steerfloat_EOMS.h"
//#include "../sub/BalloonDynamics.h"
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

		using horizon::geom::ss_EOMS;
		using horizon::geom::Station;
		using horizon::util::Data::Atmosdata;
		using horizon::util::Matrix;
		using horizon::util::atom;

class SS_Dynamics : public Subsystem {

	XMLNode subNode;

public:

	SS_Dynamics(vector<XMLNode> subsystemXMLNodeList);
	SS_Dynamics(XMLNode ssNode);
	virtual ~SS_Dynamics();

	virtual bool canPerform(const State* oldState, State* newState, const Task* task, Position* position, Environment* environment, NodeDependencies* dependencies) const;
	virtual bool canExtend(State* state, Position* position, Environment* environment, const double evalToTime, NodeDependencies* deps) const;

	double  determine_Alpha(Matrix balPos,Matrix target) const;

private:
	Matrix sscurrstate;
	double SS_lat;
	double merid_vel;
	double density;
	double zone_vel;
	double merid_wind;
	double fnt_area;
	double zone_wind;
	double merid_Cd;
	double ssmPayload;
	double zone_Cd;
	double SS_alt;
	double vol_balloon;
	double SS_long;
	double alt_vel;
	double rhoGas;
	double ssmolMass;
	double ssGasMass;
	double ssequitorial_radius;
	double msys;
	double ssbalMass;
	double sailMass;
	double SSA;
	double sailLength;
	double sailWidth;
	double tao;
	double zeroLift;
	double effFact;
	double ssmsys;
	double maxDiameter;
	double balloonMass;
	map<double,map<double,Windata>>datemap;
	map<double,Windata>windmap;
	map<double,Windata>::iterator it;
	map<double,Atmosdata>atmosmap;
};
	}
}
#endif

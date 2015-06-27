#include "Access.h"
#include "../util/Matrix.h"
//#include "horizon/net/NetworkDataClient.h"

//using namespace horizon::net;

namespace horizon {
namespace sub {

Access::Access() : Subsystem(SUBNAME_ACCESS) {}

Access::Access(XMLNode& accessNode) : Subsystem(SUBNAME_ACCESS) {}

Access::~Access() {}

bool Access::canPerform(const State* oldState, State* newState, const Task* task, Position* position, Environment* environment, NodeDependencies* deps) const
{
	/*NetworkDataClient* ndc = NetworkDataClient::Instance();
	//if(!ndc->isOpen())
		//return false;
	bool result = ndc->requestData_Bool(task->getTarget()->getName(), newState->getTaskStart());
	return result;*/
	Matrix assetPosECI = position->getPosECI(newState->getTaskStart());
	Matrix targetPosECI = task->getTarget()->getPos()->getPosECI(newState->getTaskStart());
	bool result = hasLOS(assetPosECI, targetPosECI);
	return result;
	/*
// Check illumination at Target
	shadow_state shadow = environment->getSun()->castShadowOnPos(task->getTarget()->getPos(), newState->getTaskStart());
	if (shadow != NO_SHADOW)
		return false;

// Check gsd (min quality)
	double th = 1.008067269177633e-006; // pixel angular resolution based on publised WV1 data sheets

	double csTime = newState->getTaskStart();
	Matrix p = task->getTarget()->getPos()->getPosECI(csTime); // Target Location ECI
	Matrix r = position->getPosECI(csTime);  // Vehicle location ECI
	Matrix rho = p - r;
	double rho_n = norm(rho);
	double h = norm(r) - 6378.137;
	double a = acos(h/rho_n);
	double b = M_PI_2 - th - a;
	double gsd = rho_n*sin(th)/sin(b) * 1e5;
	
	if (gsd > task->getTarget()->getMinQualCM())
		return false;

// Now check for cloud cover
	double max = 100.0;
	// Seed the random-number generator with current time so that
	// the numbers will be different every time we run.
	//
	srand( (unsigned)time( NULL ) );

	CloudCover* cc_data = CloudCover::Instance();

	vector<string>::iterator match = find(cc_data->regions.begin(), cc_data->regions.end(), task->getTarget()->getWX_Reg());
	int region = match - cc_data->regions.begin();

	int day = (int)newState->getTaskStart()/86400.0 + 1;

	double percentChanceCC = cc_data->data[day][region];
	double currentPredictionCC = (double)rand()/(double)RAND_MAX * max;

	return ( currentPredictionCC > percentChanceCC );*/
}

bool Access::canExtend(State* state, Position* position, Environment* environment, const double evalToTime, NodeDependencies* deps) const
{
	if (state->getEventEnd() < evalToTime)
		state->setEventEnd(evalToTime);
	return true;
}

}} // end namespace horizon::sub
#include "EOSensor.h"
#include "dep/Dependencies.h"

namespace horizon {
namespace sub {

	using namespace horizon::sub::dep;

EOSensor::EOSensor() : Subsystem(SUBNAME_EOSENSOR)
{
	addKey(IMAGEDATAVOLUME_KEY);
	addKey(EOON_KEY);
	addKey(GSD_KEY);
}

EOSensor::EOSensor(XMLNode EONode) : Subsystem(SUBNAME_EOSENSOR), 
	subNode(EONode)
{
	addKey(IMAGEDATAVOLUME_KEY);
	addKey(EOON_KEY);
	addKey(GSD_KEY);
	th = atof(EONode.getAttribute("pitchPerPixel"));
	hPixels = atoi(EONode.getAttribute("horizontalPixels"));
	vPixels = atoi(EONode.getAttribute("verticalPixels"));
	pixels = hPixels*vPixels;
}

EOSensor::~EOSensor() {}

bool EOSensor::canPerform(const State* oldState, State* newState, const Task* task, Position* position, Environment* environment, NodeDependencies* deps) const
{
	if(task->getType() == TASK_TYPE_IMAGE)
	{
		// Check gsd (min quality)
		double csTime = newState->getTaskStart();
		Matrix p = task->getTarget()->getPos()->getPosECI(csTime); // Target Location ECI
		Matrix r = position->getPosECI(csTime);  // Vehicle location ECI
		Matrix rho = p - r;
		double rho_n = norm(rho);
		double h = norm(r) - EARTH_RADIUS;
		double a = acos(h/rho_n);
		double b = M_PI_2 - th - a;
		double gsd = rho_n*sin(th)/sin(b) * 1e5;
		
		//if (gsd > task->getTarget()->getMinQualCM())
		//	return false;

		newState->addValue(GSD_KEY, make_pair(csTime, gsd));

		//set pixels and time to caputre based on target value
		int value = task->getTarget()->getValue();
		double dataVolume = pixels*value/2;
		double timetocapture = value*3/10;

		// get event start and task start times
		double es = newState->getEventStart();
		double ts = newState->getTaskStart();		
		
		// set task end based upon time to capture
		double te = ts + timetocapture;
		newState->setTaskEnd(te);

		// calculate incidence angle
		// from Brown, Pp. 99

		newState->addValue(IMAGEDATAVOLUME_KEY, make_pair(csTime, dataVolume));
		newState->addValue(IMAGEDATAVOLUME_KEY, make_pair(csTime+1, 0.0));

		newState->addValue(EOON_KEY, make_pair(ts, true));
		newState->addValue(EOON_KEY, make_pair(te, false));

		return true;
	}
	else
		return false;
}

bool EOSensor::canExtend(State* newState, Position* position, Environment* environment, const double evalToTime, NodeDependencies* deps) const
{
	if (newState->getEventEnd() < evalToTime)
		newState->setEventEnd(evalToTime);
	return true;
}

}} // end namespace horizon::sub

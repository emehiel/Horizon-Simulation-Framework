#include "EOSensor.h"
#include "dep/Dependencies.h"

namespace horizon {
namespace sub {

	using namespace horizon::sub::dep;

EOSensor::EOSensor() : Subsystem(SUBNAME_EOSENSOR), lowQualityPixels(5000), lowQualityTime(3), midQualityPixels(10000), midQualityTime(5), 
highQualityPixels(15000), highQualityTime(7) 
{
	addKey(INCIDENCE_KEY);
	addKey(PIXELS_KEY);
	addKey(EOON_KEY);
}

EOSensor::EOSensor(XMLNode& EONode) : Subsystem(EONode.getAttribute("subsystemName")), 
	lowQualityPixels(atoi(EONode.getAttribute("lowQualityNumPixels"))),
	lowQualityTime(atoi(EONode.getAttribute("lowQualityCaptureTime"))),
	midQualityPixels(atoi(EONode.getAttribute("midQualityNumPixels"))),
	midQualityTime(atoi(EONode.getAttribute("midQualityCaptureTime"))),
	highQualityPixels(atoi(EONode.getAttribute("highQualityNumPixels"))),
	highQualityTime(atoi(EONode.getAttribute("highQualityCaptureTime"))),
	subNode(EONode)
{
	addKey(INCIDENCE_KEY);
	addKey(PIXELS_KEY);
	addKey(EOON_KEY);
}

EOSensor::~EOSensor() {}

bool EOSensor::canPerform(const State* oldState, State* newState, const Task* task, Position* position, Environment* environment, NodeDependencies* deps) const
{
	if(task->getType() == TASK_TYPE_IMAGE)
	{
		//set pixels and time to caputre based on target value
		size_t value = task->getTarget()->getValue();
		double pixels = lowQualityPixels;
		double timetocapture = lowQualityTime;
		if(value < 8 && value >=5)
		{
			pixels = midQualityPixels;
			timetocapture = midQualityTime;
		}
		if(value >= 8)
		{
			pixels = highQualityPixels;
			timetocapture = highQualityTime;
		}

		// get event start and task start times
		double es = newState->getEventStart();
		double ts = newState->getTaskStart();		
		
		// set task end based upon time to capture
		double te = ts + timetocapture;
		newState->setTaskEnd(te);		
		
		// calculate incidence angle
		// from Brown, Pp. 99
		double timage = ts + timetocapture/2;
		Matrix m_SC_pos_at_tf_ECI = position->getPosECI(timage);
		Matrix m_target_pos_at_tf_ECI = task->getTarget()->getPos()->getPosECI(timage);
		Matrix m_pv = m_target_pos_at_tf_ECI - m_SC_pos_at_tf_ECI;
		Matrix pos_norm = -m_SC_pos_at_tf_ECI/norm(-m_SC_pos_at_tf_ECI);
		Matrix pv_norm = m_pv/norm(m_pv);

		double incidenceang = 90-180/M_PI*acos(dot(pos_norm, pv_norm));		

		// set state data
		newState->addValue(INCIDENCE_KEY, make_pair(timage, incidenceang));
		newState->addValue(INCIDENCE_KEY, make_pair(timage+1, 0.0));

		newState->addValue(PIXELS_KEY, make_pair(timage, pixels));
		newState->addValue(PIXELS_KEY, make_pair(timage+1, 0.0));

		newState->addValue(EOON_KEY, make_pair(ts, true));
		newState->addValue(EOON_KEY, make_pair(te, false));

		return true;
	}
	else
		return true;
}

bool EOSensor::canExtend(State* newState, Position* position, Environment* environment, const double evalToTime, NodeDependencies* deps) const
{
	if (newState->getEventEnd() < evalToTime)
		newState->setEventEnd(evalToTime);
	return true;
}

}} // end namespace horizon::sub

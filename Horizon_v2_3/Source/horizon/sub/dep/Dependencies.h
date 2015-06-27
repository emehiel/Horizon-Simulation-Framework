#ifndef DEPENDENCIES_H
#define DEPENDENCIES_H

#include "horizon/util/Profile.h"
#include "horizon/State.h"
#include "../SubsystemKeys.h"
#include <vector>
#include <map>

namespace horizon {
	namespace sub {
		namespace dep {

/**
 * A class for interpreting data between Subsystems. State data between Subsystems can only be exchanged 
 * through a dependency. Dependencies have two parts. The dependency collector, and the dependency
 * functions. 
 
 * A dependency collector is called within the canPerform in a subsystem that depends on
 * other Subsystems. It collects the data from all its associated dependency functions

 * A dependency function is named in a specific convention following the order: sub1 type2 sub3
 * For example, for the above naming convention the function name would be: sub1type2sub3()

 * The appropriate way to read a dependency function is as follows: The dependency function sub1type2sub3
 * interprets data from what sub3 set to the state and changes it into data format type2, which sub1
 * is dependent on for its operation and understands what to do with.
 *
 * <I>For a complete example let's pretend we have 2 Subsystems, mixingbowl and cookiesheet. When
 * mixingbowl runs, it goes first, and it only cares about the volume of dough contained within.
 * It sets to the state doughvolume. Now cookiesheet has to run, but to calculate it's size it needs the
 * area the cookies are going to take up. The only way it can tell how much area it needs is to
 * do some operation on the mixingbowl's doughvolume state variable. But it can't ask mixingbowl directly,
 * and it <B>CERTAINLY</B> can't just ask the state what mixingbowl set directly. It's none of
 * cookiesheet's business after all... So, it needs to ask a neutral third party (read collector
 * function) that mediates between different Subsystems. That neutral party might be called 
 * COOKIESHEET_COLLECT_AREAS(). COOKIESHEET_COLLECT_AREAS() knows a dependency function COOKIESHEETareaMIXINGBOWL()
 * which understands how to convert directly from the states that mixingbowl sets to the state in terms of
 * volume and output it as a suggested area.
 
 * In this way, everyone is kept completely modular so that if a new subsystem (chocolatechipsbag) decided 
 * to come in at a later time or on a later batch of cookies, all that is needed is to add another function
 * COOKIESHEETareaCHOCOLATECHIPSBAG, and to call it inside COOKIESHEET_COLLECT_AREAS(), and then the result 
 * will be interpreted to cookiesheet in a way it already knows how to deal with! No need to disturb cookiesheet 
 * because someone else joined the party!!!</I>
 *
 * @author Brian Butler
 * @author Cory O'Connor
 * @author Travis Lockyer
 */

class Dependencies
{
	vector<State*> endStates;
	map<int, vector<State*>> stateMap;

public:
	//------------------------------------------------------------------------------------------------
	//--------------------------------- DECLARE DEPENDENCY COLLECTORS --------------------------------
	//------------------------------------------------------------------------------------------------

	Profile<double> Asset1_SSDRSUB_getNewDataProfile();

	Profile<double> Asset1_COMMSUB_getDataRateProfile();

	Profile<double> Asset1_POWERSUB_getPowerProfile();

	Profile<double> Asset2_SSDRSUB_getNewDataProfile();

	Profile<double> Asset2_COMMSUB_getDataRateProfile();

	Profile<double> Asset2_POWERSUB_getPowerProfile();

	//------------------------------------------------------------------------------------------------
	//--------------------------------- DECLARE DEPENDENCY FUNCTIONS ---------------------------------
	//------------------------------------------------------------------------------------------------

	Profile<double> SSDRSUB_NewDataProfile_EOSENSORSUB(State* state);

	Profile<double> COMMSUB_DataRateProfile_SSDRSUB(State* state);

	Profile<double> POWERSUB_PowerProfile_ADCSSUB(State* state);
	Profile<double> POWERSUB_PowerProfile_EOSENSORSUB(State* state);
	Profile<double> POWERSUB_PowerProfile_SSDRSUB(State* state);
	Profile<double> POWERSUB_PowerProfile_COMMSUB(State* state);

// Do not edit code below this line
public:
	static Dependencies* Instance();
	static Dependencies* Instance(int iNum);
	static State* getState(int assetNum);
	State* getAssetState(int assetNum);
	void updateStates(const vector<State*> newStates);
	void updateStates(int thread, const vector<State*> newStates);
	//void setThreadState(int thread);
protected:
	Dependencies();
	Dependencies(const Dependencies&);
	Dependencies& operator= (const Dependencies&);
private:
	static Dependencies* pinstance;
	static map<int, Dependencies*> pinstances;
};
}}} // end namespace horizon::sub::dep

#endif // DEPENDENCIES_H
#include "Dependencies.h"

namespace horizon {
	namespace sub {
		namespace dep {
//------------------------------------------------------------------------------------------------
//--------------------------------- DEFINE DEPENDENCY COLLECTORS ---------------------------------
//------------------------------------------------------------------------------------------------

Profile<double> Dependencies::Asset1_SSDRSUB_getNewDataProfile() {
	State* state = endStates.at(0);
	return SSDRSUB_NewDataProfile_EOSENSORSUB(state);
}

Profile<double> Dependencies::Asset1_COMMSUB_getDataRateProfile() {
	State* state = endStates.at(0);
	return COMMSUB_DataRateProfile_SSDRSUB(state);
}

Profile<double> Dependencies::Asset1_POWERSUB_getPowerProfile() {
	State* state = endStates.at(0);
	Profile<double> prof1 = POWERSUB_PowerProfile_ADCSSUB(state);
	Profile<double> prof2 = POWERSUB_PowerProfile_EOSENSORSUB(state);
	Profile<double> prof3 = POWERSUB_PowerProfile_SSDRSUB(state);
	Profile<double> prof4 = POWERSUB_PowerProfile_COMMSUB(state);
	return (prof1 + prof2 + prof3 + prof4);
}

Profile<double> Dependencies::Asset2_SSDRSUB_getNewDataProfile() {
	State* state = endStates.at(1);
	return SSDRSUB_NewDataProfile_EOSENSORSUB(state);
}

Profile<double> Dependencies::Asset2_COMMSUB_getDataRateProfile() {
	State* state = endStates.at(1);
	return COMMSUB_DataRateProfile_SSDRSUB(state);
}

Profile<double> Dependencies::Asset2_POWERSUB_getPowerProfile() {
	State* state = endStates.at(1);
	Profile<double> prof1 = POWERSUB_PowerProfile_ADCSSUB(state);
	Profile<double> prof2 = POWERSUB_PowerProfile_EOSENSORSUB(state);
	Profile<double> prof3 = POWERSUB_PowerProfile_SSDRSUB(state);
	Profile<double> prof4 = POWERSUB_PowerProfile_COMMSUB(state);
	return (prof1 + prof2 + prof3 + prof4);
}

//------------------------------------------------------------------------------------------------
//--------------------------------- DEFINE DEPENDENCY FUNCTIONS ----------------------------------
//------------------------------------------------------------------------------------------------
Profile<double> Dependencies::SSDRSUB_NewDataProfile_EOSENSORSUB(State* state) {
	StateVarKey<double> EOSENSORDATA(STATEVARNAME_PIXELS);
	return state->getProfile(EOSENSORDATA)/500;
}

Profile<double> Dependencies::COMMSUB_DataRateProfile_SSDRSUB(State* state) {
	StateVarKey<double> dataratio(STATEVARNAME_DATABUFFERRATIO);
	double datarate = 5000*(state->getValueAtTime(dataratio, state->getTaskStart()).second - state->getValueAtTime(dataratio, state->getTaskEnd()).second)/(state->getTaskEnd() - state->getTaskStart());
	Profile<double> prof1;
	if(datarate != 0) {
		prof1[state->getTaskStart()] = datarate;
		prof1[state->getTaskEnd()] = 0;
	}
	return prof1;
}

Profile<double> Dependencies::POWERSUB_PowerProfile_ADCSSUB(State* state) {
	Profile<double> prof1;
	prof1[state->getEventStart()] = 40;
	prof1[state->getTaskStart()] = 60;
	prof1[state->getTaskEnd()] = 40;
	return prof1;
}

Profile<double> Dependencies::POWERSUB_PowerProfile_EOSENSORSUB(State* state) {
	Profile<double> prof1;
	prof1[state->getEventStart()] = 10;
	StateVarKey<bool> eo3(STATEVARNAME_EOON);
	if(state->getValueAtTime(eo3, state->getTaskStart()).second) {
		prof1[state->getTaskStart()] = 60;
		prof1[state->getTaskEnd()] = 10;
	}
	return prof1;
}

Profile<double> Dependencies::POWERSUB_PowerProfile_SSDRSUB(State* state) {
	Profile<double> prof1;
	prof1[state->getEventStart()] = 15;
	return prof1;
}

Profile<double> Dependencies::POWERSUB_PowerProfile_COMMSUB(State* state) {
	StateVarKey<double> c1(STATEVARNAME_DATARATE);
	return state->getProfile(c1)*20;
}

// Do not edit code below here
Dependencies* Dependencies::pinstance = 0;// initialize pointer
map<int, Dependencies*> Dependencies::pinstances = map<int, Dependencies*>();
Dependencies* Dependencies::Instance() {
	if (pinstance == 0) {  // is it the first call?
		pinstance = new Dependencies; // create sole instance
    }
    return pinstance; // address of sole instance
}
Dependencies* Dependencies::Instance(int iNum) {
	map<int, Dependencies*>::iterator dIt = pinstances.find(iNum);
	Dependencies* ret = NULL;
	if(dIt == pinstances.end()) {
		ret = new Dependencies;
		pinstances.insert(make_pair(iNum, ret));
	} else {
		ret = dIt->second;
	}
	return ret;
}
State* Dependencies::getState(int assetNum) {
	return Dependencies::Instance()->endStates.at(assetNum-1);
}
Dependencies::Dependencies() {
    //... perform necessary instance initializations 
}
State* Dependencies::getAssetState(int assetNum) {
	return endStates[assetNum-1];
}
void Dependencies::updateStates(const vector<State*> newStates) {
	endStates.clear();
	for(vector<State*>::const_iterator newStateIt = newStates.begin(); newStateIt != newStates.end(); newStateIt++) {
		endStates.push_back(*newStateIt);
	}
}
void Dependencies::updateStates(int thread, const vector<State*> newStates) {
	stateMap[thread] = newStates;
}
/*void Dependencies::setThreadState(int thread) {
	endStates.clear();
	endStates = stateMap[thread];
}*/

}}} // end namespace horizon::sub::dep

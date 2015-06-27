#ifndef STATE_H
#define STATE_H

#include <map>
#include "util/Matrix.h"
#include "util/Quat.h"
#include "util/Profile.h"
#include "horizon/io/xmlParser.h"

////////////
#include <luabind/luabind.hpp>
///////////

namespace horizon {

using namespace std;
using horizon::util::Matrix;
using horizon::util::Quat;
using horizon::util::Profile;
using horizon::util::StateVarKey;
using namespace horizon::io;

/**
 * A storage class which holds Profiles containing state data through
 * the Event and the times it occurs for later access. 
 * @author Cory O'Connor
 * @author Einar Pehrson
 */
class State {

	/** The previous state, upon which this state is based */
	const State* previous;

	/** The start of the event associated with this State */
	double eventStart;

	/** The start of the task associated with this State */
	double taskStart;

	/** The end of the task associated with this State */
	double taskEnd;

	/** The end of the event associated with this State */
	double eventEnd;

	/** The map of integer Profiles. */
	map<StateVarKey<int>, Profile<int> > idata;

	/** The map of double precision Profiles. */
	map<StateVarKey<double>, Profile<double> > ddata;

	/** The map of floating point value Profiles. */
	map<StateVarKey<float>, Profile<float> > fdata;

	/** The map of boolean Profiles. */
	map<StateVarKey<bool>, Profile<bool> > bdata;

	/** The map of Matrix Profiles. */
	map<StateVarKey<Matrix>, Profile<Matrix> > mdata;

	/** The map of Quaternion Profiles. */
	map<StateVarKey<Quat>, Profile<Quat> > qdata;
	
public:

	/**
	 * Creates an initial State
	 */
	State();

	/**
	 * Copy constructor for exact state copies
	 */
	State(State* initialStateToCopy);

	/**
	 * Creates a new State based on a previous State and a new Task start time
	 */
	State(State* previous, const double newTaskStart);

	//~State();

	const State* getPrevious(void) const;

	/** 
	 * Sets the event start time.
	 @param time The time to set for the Event start. 
	 */
	void setEventStart(const double time);

	/** 
	 * Sets the task start time.
	 @param time The time to set for the Task start.
	 */
	void setTaskStart(const double time);

	/** 
	 * Sets the task end time.
	 @param time The time to set for the Task end.
	 */
	void setTaskEnd(const double time);

	/** 
	 * Sets the Event start time.
	 @param time The time to set for the Event end.
	 */
	void setEventEnd(const double time);

	/** 
	 * Gets the Event start time.
	 @return time The Event start time.
	 */
	const double getEventStart(void) const;

	/** 
	 * Gets the Task start time.
	 @return time The Task start time.
	 */
	const double getTaskStart(void) const;

	/** 
	 * Gets the Task end time.
	 @return time The Task end time.
	 */
	const double getTaskEnd(void) const;

	/** 
	 * Gets the Event end time.
	 @return time The Event end time.
	 */
	const double getEventEnd(void) const;

	/** 
	 * Gets the last int value set for the given state variable key in the state. If no value is found
	 * it checks the previous state, continuing all the way to the initial state.
	 * @param key The integer state variable key that is being looked up.
	 * @return A pair containing the last time the variable was set, and the integer value.
	 */
	pair<double, int> getLastValue(const StateVarKey<int>& key) const {
		if(!idata.empty()) { // Are there any Profiles in there?
			map<StateVarKey<int>, Profile<int> >::const_iterator mIt = idata.find(key); // Find the Profile matching the key.
			if(!(mIt == idata.end())) // Did you find a Profile with my key?
				return (*mIt).second.getLastPair(); // We have the right Profile, find that value!
		}
		return (*previous).getLastValue(key); // This isn't the right profile, go back one and try it out!
	}

	/** 
	 * Gets the last double value set for the given state variable key in the state. If no value is found
	 * it checks the previous state, continuing all the way to the initial state.
	 * @param key The double state variable key that is being looked up.
	 * @return A pair containing the last time the variable was set, and the double value.
	 */
	pair<double, double> getLastValue(const StateVarKey<double>& key) const {
		if(!ddata.empty()) { // Are there any Profiles in there?
			map<StateVarKey<double>, Profile<double> >::const_iterator mIt = ddata.find(key); // Find the Profile matching the key.
			if(!(mIt == ddata.end())) // Did you find a Profile with my key?
				return (*mIt).second.getLastPair(); // We have the right Profile, find that value!
		}
		return (*previous).getLastValue(key); // This isn't the right profile, go back one and try it out!
	}

	/** 
	 * Gets the last float value set for the given state variable key in the state. If no value is found
	 * it checks the previous state, continuing all the way to the initial state.
	 * @param key The float state variable key that is being looked up.
	 * @return A pair containing the last time the variable was set, and the float value.
	 */
	pair<double, float> getLastValue(const StateVarKey<float>& key) const {
		if(!fdata.empty()) { // Are there any Profiles in there?
			map<StateVarKey<float>, Profile<float> >::const_iterator mIt = fdata.find(key); // Find the Profile matching the key.
			if(!(mIt == fdata.end())) // Did you find a Profile with my key?
				return (*mIt).second.getLastPair(); // We have the right Profile, find that value!
		}
		return (*previous).getLastValue(key); // This isn't the right profile, go back one and try it out!
	}

	/** 
	 * Gets the last boolean value set for the given state variable key in the state. If no value is found
	 * it checks the previous state, continuing all the way to the initial state.
	 * @param key The boolean state variable key that is being looked up.
	 * @return A pair containing the last time the variable was set, and the boolean value.
	 */
	pair<double, bool> getLastValue(const StateVarKey<bool>& key) const {
		if(!bdata.empty()) { // Are there any Profiles in there?
			map<StateVarKey<bool>, Profile<bool> >::const_iterator mIt = bdata.find(key); // Find the Profile matching the key.
			if(!(mIt == bdata.end())) // Did you find a Profile with my key?
				return (*mIt).second.getLastPair(); // We have the right Profile, find that value!
		}
		return (*previous).getLastValue(key); // This isn't the right profile, go back one and try it out!
	}

	/** 
	 * Gets the last Matrix value set for the given state variable key in the state. If no value is found
	 * it checks the previous state, continuing all the way to the initial state.
	 * @param key The Matrix state variable key that is being looked up.
	 * @return A pair containing the last time the variable was set, and the Matrix value.
	 */
	pair<double, Matrix> getLastValue(const StateVarKey<Matrix>& key) const {
		if(!mdata.empty()) { // Are there any Profiles in there?
			map<StateVarKey<Matrix>, Profile<Matrix> >::const_iterator mIt = mdata.find(key); // Find the Profile matching the key.
			if(!(mIt == mdata.end())) // Did you find a Profile with my key?
				return (*mIt).second.getLastPair(); // We have the right Profile, find that value!
		}
		return (*previous).getLastValue(key); // This isn't the right profile, go back one and try it out!
	}

	/** 
	 * Gets the last Quaternion value set for the given state variable key in the state. If no value is found
	 * it checks the previous state, continuing all the way to the initial state.
	 * @param key The Quaternion state variable key that is being looked up.
	 * @return A pair containing the last time the variable was set, and the Quaternion value.
	 */
	pair<double, Quat> getLastValue(const StateVarKey<Quat>& key) const {
		if(!qdata.empty()) { // Are there any Profiles in there?
			map<StateVarKey<Quat>, Profile<Quat> >::const_iterator mIt = qdata.find(key); // Find the Profile matching the key.
			if(!(mIt == qdata.end())) // Did you find a Profile with my key?
				return (*mIt).second.getLastPair(); // We have the right Profile, find that value!
		}
		return (*previous).getLastValue(key); // This isn't the right profile, go back one and try it out!
	}

	/** 
	 * Gets the integer value of the state at a certain time. If the exact time is not found, the data is
	 * assumed to be on a zero-order hold, and the last value set is found.
	 * @param key The integer state variable key that is being looked up.
	 * @param time The time the value is looked up at.
	 * @return A pair containing the last time the variable was set, and the integer value.
	 */
	pair<double, int> getValueAtTime(const StateVarKey<int>& key, const double time) const {
		if(!idata.empty())  { // Are there any Profiles in there?
			map<StateVarKey<int>, Profile<int> >::const_iterator mIt = idata.find(key); // Find the Profile matching the key.
			if(!(mIt == idata.end())) { // Did you find a Profile with my key?
				if((*mIt).second.getBeginTime() <= time) // Is our time after that Profile's start time?
					return (*mIt).second.getPairAtTime(time); } // We have the right Profile, find that value!
		}
		return (*previous).getValueAtTime(key, time); // This isn't the right profile, go back one and try it out!
	}

	/** 
	 * Gets the double value of the state at a certain time. If the exact time is not found, the data is
	 * assumed to be on a zero-order hold, and the last value set is found.
	 * @param key The double state variable key that is being looked up.
	 * @param time The time the value is looked up at.
	 * @return A pair containing the last time the variable was set, and the double value.
	 */
	pair<double, double> getValueAtTime(const StateVarKey<double>& key, const double time) const {
		if(!ddata.empty())  { // Are there any Profiles in there?
			map<StateVarKey<double>, Profile<double> >::const_iterator mIt = ddata.find(key); // Find the Profile matching the key.
			if(!(mIt == ddata.end())) { // Did you find a Profile with my key?
				if((*mIt).second.getBeginTime() <= time) // Is our time after that Profile's start time?
					return (*mIt).second.getPairAtTime(time); } // We have the right Profile, find that value!
		}
		return (*previous).getValueAtTime(key, time); // This isn't the right profile, go back one and try it out!
	}

	/** 
	 * Gets the float value of the state at a certain time. If the exact time is not found, the data is
	 * assumed to be on a zero-order hold, and the last value set is found.
	 * @param key The float state variable key that is being looked up.
	 * @param time The time the value is looked up at.
	 * @return A pair containing the last time the variable was set, and the float value.
	 */
	pair<double, float> getValueAtTime(const StateVarKey<float>& key, const double time) const {
		if(!fdata.empty())  { // Are there any Profiles in there?
			map<StateVarKey<float>, Profile<float> >::const_iterator mIt = fdata.find(key); // Find the Profile matching the key.
			if(!(mIt == fdata.end())) { // Did you find a Profile with my key?
				if((*mIt).second.getBeginTime() <= time) // Is our time after that Profile's start time?
					return (*mIt).second.getPairAtTime(time); } // We have the right Profile, find that value!
		}
		return (*previous).getValueAtTime(key, time); // This isn't the right profile, go back one and try it out!
	}

	/** 
	 * Gets the boolean value of the state at a certain time. If the exact time is not found, the data is
	 * assumed to be on a zero-order hold, and the last value set is found.
	 * @param key The boolean state variable key that is being looked up.
	 * @param time The time the value is looked up at.
	 * @return A pair containing the last time the variable was set, and the boolean value.
	 */
	pair<double, bool> getValueAtTime(const StateVarKey<bool>& key, const double time) const {
		if(!bdata.empty())  { // Are there any Profiles in there?
			map<StateVarKey<bool>, Profile<bool> >::const_iterator mIt = bdata.find(key); // Find the Profile matching the key.
			if(!(mIt == bdata.end())) { // Did you find a Profile with my key?
				if((*mIt).second.getBeginTime() <= time) // Is our time after that Profile's start time?
					return (*mIt).second.getPairAtTime(time); } // We have the right Profile, find that value!
		}
		return (*previous).getValueAtTime(key, time); // This isn't the right profile, go back one and try it out!
	}

	/** 
	 * Gets the Matrix value of the state at a certain time. If the exact time is not found, the data is
	 * assumed to be on a zero-order hold, and the last value set is found.
	 * @param key The Matrix state variable key that is being looked up.
	 * @param time The time the value is looked up at.
	 * @return A pair containing the last time the variable was set, and the Matrix value.
	 */
	pair<double, Matrix> getValueAtTime(const StateVarKey<Matrix>& key, const double time) const {
		if(!mdata.empty())  { // Are there any Profiles in there?
			map<StateVarKey<Matrix>, Profile<Matrix> >::const_iterator mIt = mdata.find(key); // Find the Profile matching the key.
			if(!(mIt == mdata.end())) { // Did you find a Profile with my key?
				if((*mIt).second.getBeginTime() <= time) // Is our time after that Profile's start time?
					return (*mIt).second.getPairAtTime(time); } // We have the right Profile, find that value!
		}
		return (*previous).getValueAtTime(key, time); // This isn't the right profile, go back one and try it out!
	}

	/** 
	 * Gets the Quaternion value of the state at a certain time. If the exact time is not found, the data is
	 * assumed to be on a zero-order hold, and the last value set is found.
	 * @param key The Quaternion state variable key that is being looked up.
	 * @param time The time the value is looked up at.
	 * @return A pair containing the last time the variable was set, and the Quaternion value.
	 */
	pair<double, Quat> getValueAtTime(const StateVarKey<Quat>& key, const double time) const {
		if(!qdata.empty())  { // Are there any Profiles in there?
			map<StateVarKey<Quat>, Profile<Quat> >::const_iterator mIt = qdata.find(key); // Find the Profile matching the key.
			if(!(mIt == qdata.end())) { // Did you find a Profile with my key?
				if((*mIt).second.getBeginTime() <= time) // Is our time after that Profile's start time?
					return (*mIt).second.getPairAtTime(time); } // We have the right Profile, find that value!
		}
		return (*previous).getValueAtTime(key, time); // This isn't the right profile, go back one and try it out!
	}

	/** 
	 * Returns the integer Profile matching the key given. If no Profile is found, it goes back one Event
	 * and checks again until it gets to the initial state.
	 * @param key The integer state variable key that is being looked up.
	 * @return The Profile saved in the state.
	 */
	Profile<int> getProfile(const StateVarKey<int>& key) const {
		if(!idata.empty())  { // Are there any Profiles in there?
			map<StateVarKey<int>, Profile<int> >::const_iterator mIt = idata.find(key); // Find the Profile matching the key.
			if(!(mIt == idata.end())) { // Did you find a Profile with my key?
				return (*mIt).second; } // We have the right Profile!
		}
		return (*previous).getProfile(key); // This isn't the right profile, go back one and try it out!
	}

	/** 
	 * Returns the integer Profile for this state and all previous states merged into one Profile
	 * @param key The integer state variable key that is being looked up.
	 * @return The full Profile
	 */
	Profile<int> getFullProfile(const StateVarKey<int>& key) const {
		if(!idata.empty())  { // Are there any Profiles in there?
			map<StateVarKey<int>, Profile<int> >::const_iterator mIt = idata.find(key); // Find the Profile matching the key.			
			if(!(mIt == idata.end())) { // Did you find a Profile with my key?
				if(previous != NULL) // Check whether we are at the first state
					return mergeProfiles((Profile<int>)(*mIt).second, previous->getFullProfile(key));
				return (*mIt).second;				
			}
		}
		if(previous != NULL)
			return previous->getFullProfile(key); // If no data, return profile from previous states
		return Profile<int>();
	}

	/** 
	 * Returns the double Profile matching the key given. If no Profile is found, it goes back one Event
	 * and checks again until it gets to the initial state.
	 * @param key The double state variable key that is being looked up.
	 * @return The Profile saved in the state.
	 */
	Profile<double> getProfile(const StateVarKey<double>& key) const {
		if(!ddata.empty())  { // Are there any Profiles in there?
			map<StateVarKey<double>, Profile<double> >::const_iterator mIt = ddata.find(key); // Find the Profile matching the key.
			if(!(mIt == ddata.end())) { // Did you find a Profile with my key?
				return (*mIt).second; } // We have the right Profile!
		}
		return (*previous).getProfile(key); // This isn't the right profile, go back one and try it out!
	}

	/** 
	 * Returns the double Profile for this state and all previous states merged into one Profile
	 * @param key The double state variable key that is being looked up.
	 * @return The full Profile
	 */
	Profile<double> getFullProfile(const StateVarKey<double>& key) const {
		if(!ddata.empty())  { // Are there any Profiles in there?
			map<StateVarKey<double>, Profile<double> >::const_iterator mIt = ddata.find(key); // Find the Profile matching the key.			
			if(!(mIt == ddata.end())) { // Did you find a Profile with my key?
				if(previous != NULL) // Check whether we are at the first state
					return mergeProfiles((Profile<double>)(*mIt).second, previous->getFullProfile(key));
				return (*mIt).second;				
			}
		}
		if(previous != NULL)
			return previous->getFullProfile(key); // If no data, return profile from previous states
		return Profile<double>();
	}

	/** 
	 * Returns the float Profile matching the key given. If no Profile is found, it goes back one Event
	 * and checks again until it gets to the initial state.
	 * @param key The float state variable key that is being looked up.
	 * @return The Profile saved in the state.
	 */
	Profile<float> getProfile(const StateVarKey<float>& key) const {
		if(!fdata.empty())  { // Are there any Profiles in there?
			map<StateVarKey<float>, Profile<float> >::const_iterator mIt = fdata.find(key); // Find the Profile matching the key.
			if(!(mIt == fdata.end())) { // Did you find a Profile with my key?
				return (*mIt).second; } // We have the right Profile!
		}
		return (*previous).getProfile(key); // This isn't the right profile, go back one and try it out!
	}

	/** 
	 * Returns the float Profile for this state and all previous states merged into one Profile
	 * @param key The float state variable key that is being looked up.
	 * @return The full Profile
	 */
	Profile<float> getFullProfile(const StateVarKey<float>& key) const {
		if(!fdata.empty())  { // Are there any Profiles in there?
			map<StateVarKey<float>, Profile<float> >::const_iterator mIt = fdata.find(key); // Find the Profile matching the key.			
			if(!(mIt == fdata.end())) { // Did you find a Profile with my key?
				if(previous != NULL) // Check whether we are at the first state
					return mergeProfiles((Profile<float>)(*mIt).second, previous->getFullProfile(key));
				return (*mIt).second;				
			}
		}
		if(previous != NULL)
			return previous->getFullProfile(key); // If no data, return profile from previous states
		return Profile<float>();
	}

	/** 
	 * Returns the boolean Profile matching the key given. If no Profile is found, it goes back one Event
	 * and checks again until it gets to the initial state.
	 * @param key The boolean state variable key that is being looked up.
	 * @return The Profile saved in the state.
	 */
	Profile<bool> getProfile(const StateVarKey<bool>& key) const {
		if(!bdata.empty())  { // Are there any Profiles in there?
			map<StateVarKey<bool>, Profile<bool> >::const_iterator mIt = bdata.find(key); // Find the Profile matching the key.
			if(!(mIt == bdata.end())) { // Did you find a Profile with my key?
				return (*mIt).second; } // We have the right Profile!
		}
		return (*previous).getProfile(key); // This isn't the right profile, go back one and try it out!
	}

	/** 
	 * Returns the boolean Profile for this state and all previous states merged into one Profile
	 * @param key The boolean state variable key that is being looked up.
	 * @return The full Profile
	 */
	Profile<bool> getFullProfile(const StateVarKey<bool>& key) const {
		if(!bdata.empty())  { // Are there any Profiles in there?
			map<StateVarKey<bool>, Profile<bool> >::const_iterator mIt = bdata.find(key); // Find the Profile matching the key.			
			if(!(mIt == bdata.end())) { // Did you find a Profile with my key?
				if(previous != NULL) // Check whether we are at the first state
					return mergeProfiles((Profile<bool>)(*mIt).second, previous->getFullProfile(key));
				return (*mIt).second;				
			}
		}
		if(previous != NULL)
			return previous->getFullProfile(key); // If no data, return profile from previous states
		return Profile<bool>();
	}	

	/** 
	 * Returns the Matrix Profile matching the key given. If no Profile is found, it goes back one Event
	 * and checks again until it gets to the initial state.
	 * @param key The Matrix state variable key that is being looked up.
	 * @return The Profile saved in the state.
	 */
	Profile<Matrix> getProfile(const StateVarKey<Matrix>& key) const {
		if(!mdata.empty())  { // Are there any Profiles in there?
			map<StateVarKey<Matrix>, Profile<Matrix> >::const_iterator mIt = mdata.find(key); // Find the Profile matching the key.
			if(!(mIt == mdata.end())) { // Did you find a Profile with my key?
				return (*mIt).second; } // We have the right Profile!
		}
		return (*previous).getProfile(key); // This isn't the right profile, go back one and try it out!
	}

	/** 
	 * Returns the Matrix Profile for this state and all previous states merged into one Profile
	 * @param key The Matrix state variable key that is being looked up.
	 * @return The full Profile
	 */
	Profile<Matrix> getFullProfile(const StateVarKey<Matrix>& key) const {
		if(!mdata.empty())  { // Are there any Profiles in there?
			map<StateVarKey<Matrix>, Profile<Matrix> >::const_iterator mIt = mdata.find(key); // Find the Profile matching the key.			
			if(!(mIt == mdata.end())) { // Did you find a Profile with my key?
				if(previous != NULL) // Check whether we are at the first state
					return mergeProfiles((Profile<Matrix>)(*mIt).second, previous->getFullProfile(key));
				return (*mIt).second;				
			}
		}
		if(previous != NULL)
			return previous->getFullProfile(key); // If no data, return profile from previous states
		return Profile<Matrix>();
	}

	/** 
	 * Returns the Quaternion Profile matching the key given. If no Profile is found, it goes back one Event
	 * and checks again until it gets to the initial state.
	 * @param key The Quaternion state variable key that is being looked up.
	 * @return The Profile saved in the state.
	 */
	Profile<Quat> getProfile(const StateVarKey<Quat>& key) const {
		if(!qdata.empty())  { // Are there any Profiles in there?
			map<StateVarKey<Quat>, Profile<Quat> >::const_iterator mIt = qdata.find(key); // Find the Profile matching the key.
			if(!(mIt == qdata.end())) { // Did you find a Profile with my key?
				return (*mIt).second; } // We have the right Profile!
		}
		return (*previous).getProfile(key); // This isn't the right profile, go back one and try it out!
	}

	/** 
	 * Returns the Quaternion Profile for this state and all previous states merged into one Profile
	 * @param key The Quaternion state variable key that is being looked up.
	 * @return The full Profile
	 */
	Profile<Quat> getFullProfile(const StateVarKey<Quat>& key) const {
		if(!qdata.empty())  { // Are there any Profiles in there?
			map<StateVarKey<Quat>, Profile<Quat> >::const_iterator mIt = qdata.find(key); // Find the Profile matching the key.			
			if(!(mIt == qdata.end())) { // Did you find a Profile with my key?
				if(previous != NULL) // Check whether we are at the first state
					return mergeProfiles((Profile<Quat>)(*mIt).second, previous->getFullProfile(key));
				return (*mIt).second;				
			}
		}
		if(previous != NULL)
			return previous->getFullProfile(key); // If no data, return profile from previous states
		return Profile<Quat>();
	}

	/** 
	 * Sets the integer Profile in the state with its matching key. If a Profile is found already under
	 * that key, this will overwrite the old Profile.
	 * @param key The integer state variable key that is being set.\
	 * @param profIn The integer Profile being saved.
	 */
	void setProfile(const StateVarKey<int>& key, const Profile<int>& profIn){
		map<StateVarKey<int>, Profile<int> >::iterator mIt = idata.find(key); // Find the Profile matching the key.
		if(mIt == idata.end()) // If there's no Profile matching that key, insert a new one.
			idata.insert(make_pair(key, profIn)); 
		else { // Otherwise, erase whatever is there, and insert a new one.
			idata.erase(mIt);
			idata.insert(make_pair(key, profIn)); 
		}
	}

	/** 
	 * Sets the double Profile in the state with its matching key. If a Profile is found already under
	 * that key, this will overwrite the old Profile.
	 * @param key The double state variable key that is being set.\
	 * @param profIn The double Profile being saved.
	 */
	void setProfile(const StateVarKey<double>& key, const Profile<double>& profIn){
		map<StateVarKey<double>, Profile<double> >::iterator mIt = ddata.find(key); // Find the Profile matching the key.
		if(mIt == ddata.end()) // If there's no Profile matching that key, insert a new one.
			ddata.insert(make_pair(key, profIn)); 
		else { // Otherwise, erase whatever is there, and insert a new one.
			ddata.erase(mIt);
			ddata.insert(make_pair(key, profIn)); 
		}
	}

	/** 
	 * Sets the float Profile in the state with its matching key. If a Profile is found already under
	 * that key, this will overwrite the old Profile.
	 * @param key The float state variable key that is being set.\
	 * @param profIn The float Profile being saved.
	 */
	void setProfile(const StateVarKey<float>& key, const Profile<float>& profIn){
		map<StateVarKey<float>, Profile<float> >::iterator mIt = fdata.find(key); // Find the Profile matching the key.
		if(mIt == fdata.end()) // If there's no Profile matching that key, insert a new one.
			fdata.insert(make_pair(key, profIn)); 
		else { // Otherwise, erase whatever is there, and insert a new one.
			fdata.erase(mIt);
			fdata.insert(make_pair(key, profIn)); 
		}
	}

	/** 
	 * Sets the boolean Profile in the state with its matching key. If a Profile is found already under
	 * that key, this will overwrite the old Profile.
	 * @param key The boolean state variable key that is being set.\
	 * @param profIn The boolean Profile being saved.
	 */
	void setProfile(const StateVarKey<bool>& key, const Profile<bool>& profIn){
		map<StateVarKey<bool>, Profile<bool> >::iterator mIt = bdata.find(key); // Find the Profile matching the key.
		if(mIt == bdata.end()) // If there's no Profile matching that key, insert a new one.
			bdata.insert(make_pair(key, profIn)); 
		else { // Otherwise, erase whatever is there, and insert a new one.
			bdata.erase(mIt);
			bdata.insert(make_pair(key, profIn)); 
		}
	}

	/** 
	 * Sets the Matrix Profile in the state with its matching key. If a Profile is found already under
	 * that key, this will overwrite the old Profile.
	 * @param key The Matrix state variable key that is being set.\
	 * @param profIn The Matrix Profile being saved.
	 */
	void setProfile(const StateVarKey<Matrix>& key, const Profile<Matrix>& profIn){
		map<StateVarKey<Matrix>, Profile<Matrix> >::iterator mIt = mdata.find(key); // Find the Profile matching the key.
		if(mIt == mdata.end()) // If there's no Profile matching that key, insert a new one.
			mdata.insert(make_pair(key, profIn)); 
		else { // Otherwise, erase whatever is there, and insert a new one.
			mdata.erase(mIt);
			mdata.insert(make_pair(key, profIn)); 
		}
	}

	/** 
	 * Sets the Quaternion Profile in the state with its matching key. If a Profile is found already under
	 * that key, this will overwrite the old Profile.
	 * @param key The Quaternion state variable key that is being set.\
	 * @param profIn The Quaternion Profile being saved.
	 */
	void setProfile(const StateVarKey<Quat>& key, const Profile<Quat>& profIn){
		map<StateVarKey<Quat>, Profile<Quat> >::iterator mIt = qdata.find(key); // Find the Profile matching the key.
		if(mIt == qdata.end()) // If there's no Profile matching that key, insert a new one.
			qdata.insert(make_pair(key, profIn)); 
		else { // Otherwise, erase whatever is there, and insert a new one.
			qdata.erase(mIt);
			qdata.insert(make_pair(key, profIn)); 
		}
	}

	/** 
	 * Adds a integer Profile value pair to the state with the given key. If no Profile exists, a new Profile is created
	 * with the corresponding key. If a Profile exists with that key, the pair is appended onto the end of the Profile. 
	 * Ensure that the Profile is still time ordered if this is the case.
	 * @param key The key corresponding to the state variable.
	 * @param pairIn The pair to be added to the integer Profile.
	 */
	void addValue(const StateVarKey<int>& key, const pair<double, int>& pairIn){
		map<StateVarKey<int>, Profile<int> >::iterator mIt = idata.find(key); // Find the Profile matching the key.
		if(mIt == idata.end()) // If there's no Profile matching that key, insert a new one.
			idata.insert(make_pair(key, (Profile<int>) pairIn)); 
		else // Otherwise, add this data point to the existing Profile.
			(*mIt).second.addData(pairIn);
	}

	/** 
	 * Adds a integer Profile to the state with the given key. If no Profile exists, a new Profile is created
	 * with the corresponding key. If a Profile exists with that key, the Profile is appended onto the end of the Profile. 
	 * @param key The key corresponding to the state variable.
	 * @param profIn The Profile to be added to the integer Profile.
	 */
	void addValue(const StateVarKey<int>& key, const Profile<int>& profIn){
		map<StateVarKey<int>, Profile<int> >::iterator mIt = idata.find(key); // Find the Profile matching the key.
		if(mIt == idata.end()) // If there's no Profile matching that key, insert a new one.
			idata.insert(make_pair(key, profIn)); 
		else // Otherwise, add this data point to the existing Profile.
			(*mIt).second.addData(profIn);
	}

	/** 
	 * Adds a double Profile value pair to the state with the given key. If no Profile exists, a new Profile is created
	 * with the corresponding key. If a Profile exists with that key, the pair is appended onto the end of the Profile. 
	 * Ensure that the Profile is still time ordered if this is the case.
	 * @param key The key corresponding to the state variable.
	 * @param pairIn The pair to be added to the double Profile.
	 */
	void addValue(const StateVarKey<double>& key, const pair<double, double>& pairIn){
		map<StateVarKey<double>, Profile<double> >::iterator mIt = ddata.find(key); // Find the Profile matching the key.
		if(mIt == ddata.end()) // If there's no Profile matching that key, insert a new one.
			ddata.insert(make_pair(key, (Profile<double>) pairIn)); 
		else // Otherwise, add this data point to the existing Profile.
			(*mIt).second.addData(pairIn);
	}

	/** 
	 * Adds a double Profile to the state with the given key. If no Profile exists, a new Profile is created
	 * with the corresponding key. If a Profile exists with that key, the Profile is appended onto the end of the Profile. 
	 * @param key The key corresponding to the state variable.
	 * @param profIn The Profile to be added to the double Profile.
	 */
	void addValue(const StateVarKey<double>& key, const Profile<double>& profIn){
		map<StateVarKey<double>, Profile<double> >::iterator mIt = ddata.find(key); // Find the Profile matching the key.
		if(mIt == ddata.end()) // If there's no Profile matching that key, insert a new one.
			ddata.insert(make_pair(key, profIn));
		else // Otherwise, add this Profile to the existing Profile.
			(*mIt).second.addData(profIn);
	}

	/** 
	 * Adds a float Profile value pair to the state with the given key. If no Profile exists, a new Profile is created
	 * with the corresponding key. If a Profile exists with that key, the pair is appended onto the end of the Profile. 
	 * Ensure that the Profile is still time ordered if this is the case.
	 * @param key The key corresponding to the state variable.
	 * @param pairIn The pair to be added to the float Profile.
	 */
	void addValue(const StateVarKey<float>& key, const pair<double, float>& pairIn){
		map<StateVarKey<float>, Profile<float> >::iterator mIt = fdata.find(key); // Find the Profile matching the key.
		if(mIt == fdata.end()) // If there's no Profile matching that key, insert a new one.
			fdata.insert(make_pair(key, (Profile<float>) pairIn)); 
		else // Otherwise, add this data point to the existing Profile.
			(*mIt).second.addData(pairIn);
	}

	/** 
	 * Adds a float Profile to the state with the given key. If no Profile exists, a new Profile is created
	 * with the corresponding key. If a Profile exists with that key, the Profile is appended onto the end of the Profile. 
	 * @param key The key corresponding to the state variable.
	 * @param profIn The Profile to be added to the float Profile.
	 */
	void addValue(const StateVarKey<float>& key, const Profile<float>& profIn){
		map<StateVarKey<float>, Profile<float> >::iterator mIt = fdata.find(key); // Find the Profile matching the key.
		if(mIt == fdata.end()) // If there's no Profile matching that key, insert a new one.
			fdata.insert(make_pair(key, profIn));
		else // Otherwise, add this Profile to the existing Profile.
			(*mIt).second.addData(profIn);
	}

	/** 
	 * Adds a boolean Profile value pair to the state with the given key. If no Profile exists, a new Profile is created
	 * with the corresponding key. If a Profile exists with that key, the pair is appended onto the end of the Profile. 
	 * Ensure that the Profile is still time ordered if this is the case.
	 * @param key The key corresponding to the state variable.
	 * @param pairIn The pair to be added to the boolean Profile.
	 */
	void addValue(const StateVarKey<bool>& key, const pair<double, bool>& pairIn){
		map<StateVarKey<bool>, Profile<bool> >::iterator mIt = bdata.find(key); // Find the Profile matching the key.
		if(mIt == bdata.end()) // If there's no Profile matching that key, insert a new one.
			bdata.insert(make_pair(key, (Profile<bool>) pairIn)); 
		else // Otherwise, add this data point to the existing Profile.
			(*mIt).second.addData(pairIn);
	}

	/** 
	 * Adds a boolean Profile to the state with the given key. If no Profile exists, a new Profile is created
	 * with the corresponding key. If a Profile exists with that key, the Profile is appended onto the end of the Profile. 
	 * @param key The key corresponding to the state variable.
	 * @param profIn The Profile to be added to the boolean Profile.
	 */
	void addValue(const StateVarKey<bool>& key, const Profile<bool>& profIn){
		map<StateVarKey<bool>, Profile<bool> >::iterator mIt = bdata.find(key); // Find the Profile matching the key.
		if(mIt == bdata.end()) // If there's no Profile matching that key, insert a new one.
			bdata.insert(make_pair(key, profIn));
		else // Otherwise, add this Profile to the existing Profile.
			(*mIt).second.addData(profIn);
	}

	/** 
	 * Adds a Matrix Profile value pair to the state with the given key. If no Profile exists, a new Profile is created
	 * with the corresponding key. If a Profile exists with that key, the pair is appended onto the end of the Profile. 
	 * Ensure that the Profile is still time ordered if this is the case.
	 * @param key The key corresponding to the state variable.
	 * @param pairIn The pair to be added to the Matrix Profile.
	 */
	void addValue(const StateVarKey<Matrix>& key, const pair<double, Matrix>& pairIn){
		map<StateVarKey<Matrix>, Profile<Matrix> >::iterator mIt = mdata.find(key); // Find the Profile matching the key.
		if(mIt == mdata.end()) // If there's no Profile matching that key, insert a new one.
			mdata.insert(make_pair(key, (Profile<Matrix>) pairIn)); 
		else // Otherwise, add this data point to the existing Profile.
			(*mIt).second.addData(pairIn);
	}

	/** 
	 * Adds a Matrix Profile to the state with the given key. If no Profile exists, a new Profile is created
	 * with the corresponding key. If a Profile exists with that key, the Profile is appended onto the end of the Profile. 
	 * @param key The key corresponding to the state variable.
	 * @param profIn The Profile to be added to the Matrix Profile.
	 */
	void addValue(const StateVarKey<Matrix>& key, const Profile<Matrix>& profIn){
		map<StateVarKey<Matrix>, Profile<Matrix> >::iterator mIt = mdata.find(key); // Find the Profile matching the key.
		if(mIt == mdata.end()) // If there's no Profile matching that key, insert a new one.
			mdata.insert(make_pair(key, profIn));
		else // Otherwise, add this Profile to the existing Profile.
			(*mIt).second.addData(profIn);
	}

	/** 
	 * Adds a Quaternion Profile value pair to the state with the given key. If no Profile exists, a new Profile is created
	 * with the corresponding key. If a Profile exists with that key, the pair is appended onto the end of the Profile. 
	 * Ensure that the Profile is still time ordered if this is the case.
	 * @param key The key corresponding to the state variable.
	 * @param pairIn The pair to be added to the Quaternion Profile.
	 */
	void addValue(const StateVarKey<Quat>& key, const pair<double, Quat>& pairIn){
		map<StateVarKey<Quat>, Profile<Quat> >::iterator mIt = qdata.find(key); // Find the Profile matching the key.
		if(mIt == qdata.end()) // If there's no Profile matching that key, insert a new one.
			qdata.insert(make_pair(key, (Profile<Quat>) pairIn)); 
		else // Otherwise, add this data point to the existing Profile.
			(*mIt).second.addData(pairIn);
	}

	/** 
	 * Adds a Quaternion Profile to the state with the given key. If no Profile exists, a new Profile is created
	 * with the corresponding key. If a Profile exists with that key, the Profile is appended onto the end of the Profile. 
	 * @param key The key corresponding to the state variable.
	 * @param profIn The Profile to be added to the Quaternion Profile.
	 */
	void addValue(const StateVarKey<Quat>& key, const Profile<Quat>& profIn){
		map<StateVarKey<Quat>, Profile<Quat> >::iterator mIt = qdata.find(key); // Find the Profile matching the key.
		if(mIt == qdata.end()) // If there's no Profile matching that key, insert a new one.
			qdata.insert(make_pair(key, profIn));
		else // Otherwise, add this Profile to the existing Profile.
			(*mIt).second.addData(profIn);
	}
};

inline const State* State::getPrevious(void) const {
	return previous; }

inline void State::setEventStart(const double time) {
	eventStart = time; 
	taskStart = time;
	taskEnd = time;
	eventEnd = time;
}

inline void State::setTaskStart(const double time) {
	taskStart = time;
	taskEnd = time;
	eventEnd = time;
}

inline void State::setTaskEnd(const double time) {
	taskEnd = time;
	eventEnd = time;
}

inline void State::setEventEnd(const double time) {
	eventEnd = time;
}

inline const double State::getEventStart(void) const{
	return eventStart; }

inline const double State::getTaskStart(void) const{
	return taskStart; }

inline const double State::getTaskEnd(void) const{
	return taskEnd; }

inline const double State::getEventEnd(void) const{
	return eventEnd; }

} // end namespace horizon

#endif /*STATE_H_*/

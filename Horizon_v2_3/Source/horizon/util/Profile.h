#ifndef PROFILE_H
#define PROFILE_H

#include <map>
#include <list>
#include <iostream> // testing
#include <string>

using namespace std;

namespace horizon {
namespace util {

	//using namespace std;

/**
 * A Template Profile Class
 * @author Cory O'Connor
 */
template <typename T> class Profile
{ 
	// Variables
private:
	/** 
	 * A map containing the time-ordered values.
	 */
	typedef map<double, T> ProfileData;
	ProfileData data;
	
	// Construction
public:
	/**
	 * Creates a new Profile
	 */
	Profile<T>(void) {}

	/**
	 * Creates a new Profile using by copying existing profile
	 * @param otherProf the Profile to create a copy of
	 */
	Profile<T>(const Profile<T>& otherProf) : data(otherProf.data) { /*cout << "ctor1" << otherProf.data.size() << "\n";*/ }

	/**
	 * Creates a new Profile using an initial point
	 * @param pointIn the initial point in the profile
	 */
	Profile<T>(const pair<double, T>& pointIn) { data.insert(pointIn); /*cout << "ctor2\n";*/ }

	/**
	 * Creates a new Profile using an initial point
	 * @param tIn the time for the initial point
	 * @param valIn the value for the initial point
	 */
	Profile<T>(const double tIn, const T& valIn) { data[tIn] = valIn; }

	/**
	 */
	Profile<T>(const list<double>& tIn, const list<T>& valIn) {
		_ASSERTE(tIn.size() == valIn.size());
		list<double>::const_iterator lIt1 = tIn.begin();
		list<T>::const_iterator lIt2 = valIn.begin();

		if(tIn.size() == valIn.size()) {
			for(lIt1; lIt1 != tIn.end(); lIt1++) {
				data[(*lIt1)] = (*lIt2);
				lIt2++;
			}
		}
		else {
			cout << "Profile class constructor called with two lists of unequal length. Only matching values inserted." << endl;
			for(lIt1; (lIt1 != tIn.end() && lIt2 != valIn.end()); lIt1++) {
				data[(*lIt1)] = (*lIt2);
				lIt2++;
			}
		}
	}

	/**
	 * Creates a new Profile using a map
	 * @param dataIn the map of data input
	 */
	Profile<T>(const map<double, T>& dataIn) : data(dataIn) {}

	/**
	 * Default Destructor
	 */
	~Profile<T>(void) {}

	// Iterators
public:
	/**
	 * 
	 */
	typedef typename ProfileData::iterator iterator; 

	/**
	 * 
	 */
	typedef typename ProfileData::const_iterator const_iterator;

	/**
	 * 
	 */
	const_iterator begin() const { return data.begin(); }
	/**
	 * 
	 */
	const_iterator end() const { return data.end(); }
	/**
	 * 
	 */
	const_iterator itback() const { return --data.end(); }
	/**
	 * 
	 */
	const pair <double, T> front() const { return *data.begin(); }
	/**
	 * 
	 */
	const pair <double, T> back() const { return *data.rbegin(); }
	/**
	 * 
	 */
	const size_t size(void) const { return data.size(); }

	const bool empty(void) const { return size() == 0; }

	// Functions
public:
	/**
	 * Adds a new data point to an existing profile
	 * @param pairIn the Pair<double, T> to add to the Profile
	 */
	void addData(const pair<double, T>& pairIn) { data.insert(pairIn); }

	/**
	 * Adds a new data point to an existing profile
	 * @param tIn the time of the point to add
	 * @param valIn the value of the point to add
	 */
	void addData(const double tIn, const T& valIn) { data[tIn] = valIn; }
	/**
	 * Adds new data to an existing profile
	 * @param profToMergeIn the Profile to add data in from
	 */
	void addData(const Profile<T>& profToMergeIn) {
		if(!profToMergeIn.empty())
			data.insert(profToMergeIn.begin(), profToMergeIn.end());
	}
	/**
	 * Returns the last value in the profile
	 * @return the last value in the profile
	 */
	const T getLastValue(void) const { return getLastPair().second; }

	/**
	 * Returns the value of the profile at the specified time
	 * @param time the time to get the value from the Profile
	 * @return the value of the Profile at the specified time
	 */
	const T getValueAtTime(double time) const {	return getPairAtTime(time).second; }
	
	/**
	 * Removes all entries unnecessary to represent a zero-order hold
	 */
	void removeDuplicates() {
		map<double, T>::iterator it = data.begin();
		for(; it != data.end(); ++it) {
			bool erased = true;
			T curr_T = it->second;
			map<double, T>::iterator it2 = it;
			while(erased && it != --data.end())
			{			
				T next_T = (++it2)->second;
				if(next_T == curr_T) {
					data.erase(it2--);
					continue;
				}
				erased = false;
			}
		}
	}

	/**
	 * Returns the last pair in the profile
	 * @return the last Pair<double, T> in the Profile
	 */
	pair<double, T> getLastPair(void) const 
	{
//		_ASSERTE(data.size()>0);
		if(data.size()>0) {
			return *data.rbegin();
		}
		else {
			cout << "getLastPair: Attemping to get data point from empty profile. Returning null pair." << endl;
			return make_pair(0.0,(T)0);
		}
	}

	/**
	 *
	 */
	pair<double, T> getPairAtTime(const double& time) const {
		_ASSERTE(data.size()>0);
		if(data.size()>0)
		{
			map<double, T>::const_iterator returnIt = upper_bound(data.begin(), data.end(), 
				make_pair(time,(data.begin())->second), profTimeLesser<T>);
			if(returnIt != data.begin())
				return (*(--(returnIt)));
			else
			{
				cout << "getPairAtTime: Attemping to reference time before first data point. Returning first pair." << endl;
				return *data.begin();
			}
		}
		else
		{
			cout << "getPairAtTime: Attemping to get data point from empty profile. Returning null pair." << endl;
			return make_pair(0.0,(T)0);
		}
	}

	/**
	 * 
	 */
	const pair<double, T> operator()(const size_t i) const 
	{
		_ASSERTE(i<=data.size() && i>=1);
		if(i<=data.size() && i>=1)
		{
			size_t add;
			map<double, T>::const_iterator lIt = data.begin();
			for(add=1; add<i; add++)
				lIt++;
			return *lIt;
		}
		else 
			return make_pair(0,0);
	}

	/**
	 * Defines the equal operator.  Sets one Profile equal to other
	 * @param other the Profile used as the source to copy data from
	 */
	const Profile operator=(const Profile& other) {
		this->data = other.data;
		return *this;
	}
	/**
	 * Get the minimum value in the Profile
	 * @ return the minimum value in the Profile
	 */
	const T pmin() const {
		return std::min_element(data.begin(), data.end(), profValLesser<T>)->second; }
	/**
	 * Get the maximum value in the Profile
	 * @ return the maximum value in the Profile
	 */
	const T pmax() const {
		return std::max_element(data.begin(), data.end(), profValLesser<T>)->second; }
	/**
	 * Get the sum of all values in the Profile
	 * @ return the sum of all Profile values
	 */
	const T sum() const {
		T sumtot = data.begin()->second;
		map<double, T>::const_iterator lIt = data.begin();
		for(++lIt; lIt!= data.end(); lIt++)
			sumtot = sumtot + (*lIt).second;
		return sumtot; 
	}

	ProfileData getProfileData() {
		return data;
	}

	/// \todo TODO: Put integration methods into the Integrator class!
	/// \todo TODO: Clean up and run tests

	/**
	* Integrates the Profile with an initial value from the start to end time
	*
	* @param start the start of the integral
	* @param end the end of the integral
	* @param iv the initial value of the Profile that is used if any region of the integral falls before the first point in the Profile
	*/
	const T integrate(double start, double end, T iv = T()) const {
		if(end < start)
			return (-1)*integrate(end, start, iv);
		if(size() == 0 || end <= data.begin()->first)
			return (end-start)*iv;
		if(end == start)
			return 0;

		// Find begin iterator (first data point after or equal to begin) and ICs
		ProfileData::const_iterator beginIt = data.begin();
		while(beginIt->first < start) {
			iv = beginIt->second;
			if(++beginIt == data.end())
				break;
		}
		// Find end iterator (last data point before end)
		ProfileData::const_iterator endIt = data.begin();
		while(endIt->first < end) {
			if(++endIt == data.end())
				break;
		}
		endIt--;

		// If the end iterator is before begin iterator there are no data points between them
		if(beginIt == data.end())
			return (end-start)*iv;
		if(beginIt->first > endIt->first)
			return (end-start)*iv;

		// Find intersected begin integral
		T intOut = iv*(beginIt->first - start);
		// Add intersected end integral
		intOut = intOut + (end - endIt->first)*endIt->second;
		// Add middle profile segments
		for(ProfileData::const_iterator valIt = beginIt; valIt != endIt; valIt++) {
			double begintime = valIt->first;
			T val = valIt->second;
			valIt++;
			double endtime = valIt->first;	
			intOut = intOut + (endtime-begintime)*val;
			valIt--;
		}

		return intOut;
	}

	/**
	* Integrates the Profile with an initial value and condition from the start to end time
	*
	* @param start the start of the integral
	* @param end the end of the integral
	* @param saveFreq the frequence to evaluate the integral
	* @param iv the initial value of the Profile that is used if any region of the integral is evaluated before the first point in the Profile
	* @param ic the initial condition
	*/
	const Profile integrateToProf(double start, double end, double saveFreq, T iv = T(), T ic = T()) const {
		Profile prof;
		T last = ic;
		double time;
		for(time = start + saveFreq; time < end; time += saveFreq) {
			T result = last + integrate(time - saveFreq, time, iv);
			if (result != last) {
				prof[time] = result;
			}
			last = result;
		}
		T result = last + integrate(time -= saveFreq, end, iv);
		if (result != last)
			prof[end] = result;
		
		if(prof.empty())
			prof[start] = ic;
		return prof;
	}

	/**
	* Integrates the Profile with a lower limit given an initial value and condition from the start to end time
	*
	* If the integral falls below the lower limit, the value for the integral is set to the lower limit
	*
	* @param start the start of the integral
	* @param end the end of the integral
	* @param saveFreq the frequence to evaluate the integral
	* @param lowerBound the lower bound for the integral, the output will never be below this value
	* @param exceeded gets set to true if the value of the integral falls belwo the lowerBound at any time
	* @param iv the initial value of the Profile that is used if any region of the integral is evaluated before the first point in the Profile
	* @param ic the initial condition
	*/
	const Profile lowerLimitIntegrateToProf(double start, double end, double saveFreq, T lowerBound, bool& exceeded, T iv = T(), T ic = T()) const {
		exceeded = false;
		Profile prof;
		T last = ic;
		double time;
		for(time = start + saveFreq; time < end; time += saveFreq) {
			T result = last + integrate(time - saveFreq, time, iv);
			if(result < lowerBound) {
				result = lowerBound;
				exceeded = true;
			}
			if (result != last) {
				prof[time] = result;
			}
			last = result;
		}
		T result = last + integrate(time -= saveFreq, end, iv);
		if(result < lowerBound) {
				result = lowerBound;
				exceeded = true;
		}
		if (result != last)
			prof[end] = result;

		if(prof.empty())
			prof[start] = ic;
		return prof;
	}

	/**
	* Integrates the Profile with an upper limit given an initial value and condition from the start to end time
	*
	* If the integral rises above the upper limit, the value fo the integral is set to the upper limit
	*
	* @param start the start of the integral
	* @param end the end of the integral
	* @param saveFreq the frequence to evaluate the integral
	* @param upperBound the upper bound for the integral, the output will never be above this value
	* @param exceeded gets set to true if the value of the integral falls belwo the lowerBound at any time
	* @param iv the initial value of the Profile that is used if any region of the integral is evaluated before the first point in the Profile
	* @param ic the initial condition
	*/
	const Profile upperLimitIntegrateToProf(double start, double end, double saveFreq, T upperBound, bool& exceeded, T iv = T(), T ic = T()) const {
		exceeded = false;
		Profile prof;
		T last = ic;
		double time;
		for(time = start + saveFreq; time < end; time += saveFreq) {
			T result = last + integrate(time - saveFreq, time, iv);
			if(result > upperBound) {
				result = upperBound;
				exceeded = true;
			}
			if (result != last) {
				prof[time] = result;
			}
			last = result;
		}
		T result = last + integrate(time -= saveFreq, end, iv);
		if(result > upperBound) {
				result = upperBound;
				exceeded = true;
		}
		if (result != last)
			prof[end] = result;

		if(prof.empty())
			prof[start] = ic;
		return prof;
	}

	const Profile limitIntegrateToProf(double start, double end, double saveFreq, T lowerBound, T upperBound, bool& exceeded_lower, bool& exceeded_upper, T iv = T(), T ic = T()) const {
		exceeded_lower = false;
		exceeded_upper = false;
		Profile prof;
		T last = ic;
		double time;
		for(time = start + saveFreq; time < end; time += saveFreq) {
			T result = last + integrate(time - saveFreq, time, iv);
			if(result > upperBound) {
				result = upperBound;
				exceeded_upper = true;
			}
			if(result < lowerBound) {
				result = lowerBound;
				exceeded_lower = true;
			}
			if (result != last) {
				prof[time] = result;
			}
			last = result;
		}
		T result = last + integrate(time -= saveFreq, end, iv);
		if(result > upperBound) {
			result = upperBound;
			exceeded_upper = true;
		}
		if(result < lowerBound) {
			result = lowerBound;
			exceeded_lower = true;
		}
		if (result != last)
			prof[end] = result;

		if(prof.empty())
			prof[start] = ic;
		return prof;
	}

	/**
	 * Need to time average this... 
	 */
	//const T avg() const {
	//	return this->sum()/(int)this->size(); }


	const double getBeginTime(void) const {	return data.begin()->first; }

	T& operator[](const double& key) { return data[key]; }

	Profile<T>& operator*=(const double &rhs) {
		for(Profile::iterator it = data.begin(); it != data.end(); it++)
			data[it->first] = it->second*rhs;
		return *this;
	}

	Profile<T>& operator+=(const Profile &rhs) {
		Profile<T> profOut;
		if(rhs.data.size() == 0)
			return *this;
		if(data.size() == 0) {
			data = rhs.data;
			return *this;
		}
		list<double> uniquetimes;
		list<double>::const_iterator lIt;
		Profile::const_iterator dataIt, rhsIt;
		// Gets unique union of the times from the lists
		for(dataIt = data.begin(); dataIt != data.end(); dataIt++) { uniquetimes.push_back((*dataIt).first); }
		for(rhsIt = rhs.begin(); rhsIt != rhs.end(); rhsIt++) { uniquetimes.push_back((*rhsIt).first); }
		uniquetimes.sort();
		uniquetimes.unique();
		// Iterate through and copy (and add) the right values
		for(lIt= uniquetimes.begin(); lIt != uniquetimes.end(); lIt++)
		{
			dataIt = upper_bound(data.begin(), data.end(), make_pair(*lIt ,data.begin()->second), profTimeLesser<T>);
			rhsIt = upper_bound(rhs.begin(), rhs.end(), make_pair(*lIt ,rhs.begin()->second), profTimeLesser<T>);
			if(dataIt == data.begin()) { profOut.addData(*lIt, (*(--rhsIt)).second); }
			else if(rhsIt == rhs.begin()) { profOut.addData(*lIt, (*(--dataIt)).second); }
			else { profOut.addData(*lIt, (*(--dataIt)).second + (*(--rhsIt)).second); }
		}
		data = profOut.data;
		return *this;
	}

	// Friend Functions
public:
	/**
	 * Profile addition
	 */
	friend const Profile operator+(const Profile &A, const Profile &B)
	{
		//cout << "op+ PP\n";

		Profile<T> profOut;
		if(A.size() == 0)
			return B;
		if(B.size() == 0)
			return A;
		list<double> uniquetimes;
		list<double>::const_iterator lIt;
		Profile::const_iterator aIt, bIt;
		// Gets unique union of the times from the lists
		for(aIt = A.begin(); aIt != A.end(); aIt++) { uniquetimes.push_back((*aIt).first); }
		for(bIt = B.begin(); bIt != B.end(); bIt++) { uniquetimes.push_back((*bIt).first); }
		uniquetimes.sort();
		uniquetimes.unique();
		// Iterate through and copy (and add) the right values
		for(lIt= uniquetimes.begin(); lIt != uniquetimes.end(); lIt++)
		{
			aIt = upper_bound(A.begin(), A.end(), make_pair(*lIt ,A.begin()->second), profTimeLesser<T>);
			bIt = upper_bound(B.begin(), B.end(), make_pair(*lIt ,B.begin()->second), profTimeLesser<T>);
			if(aIt == A.begin()) { profOut[*lIt] = (*(--bIt)).second; }
			else if(bIt == B.begin()) { profOut[*lIt] = (*(--aIt)).second; }
			else { profOut[*lIt] = (*(--aIt)).second + (*(--bIt)).second; }
		}
		return profOut;

	}

	friend const Profile operator+(const Profile &A, const double &B)
	{
		//cout << "op+ PD\n";

		Profile<T> profOut;
		for(Profile::const_iterator aIt = A.begin(); aIt != A.end(); aIt++)
			profOut[aIt->first] = aIt->second+B; 
		return profOut;

	}

	friend const Profile operator+(const double &B, const Profile &A)
	{
		//cout << "op+ DP\n";

		Profile<T> profOut;
		for(Profile::const_iterator aIt = A.begin(); aIt != A.end(); aIt++)
			profOut[aIt->first] = aIt->second+B; 
		return profOut;

	}

	friend const Profile operator-(const Profile &A, const double &B)
	{
		//cout << "op- PD\n";

		Profile<T> profOut;
		for(Profile::const_iterator aIt = A.begin(); aIt != A.end(); aIt++)
			profOut[aIt->first] = aIt->second-B; 
		return profOut;

	}

	friend const Profile operator-(const double &B, const Profile &A)
	{
		//cout << "op- DP\n";

		Profile<T> profOut;
		for(Profile::const_iterator aIt = A.begin(); aIt != A.end(); aIt++)
			profOut[aIt->first] = B - aIt->second; 
		return profOut;

	}

	friend const Profile operator*(const Profile &A, const double &B)
	{
		//cout << "op* PD\n";

		Profile<T> profOut;
		for(Profile::const_iterator aIt = A.begin(); aIt != A.end(); aIt++)
			profOut[aIt->first] = aIt->second*B; 
		return profOut;

	}

	friend const Profile operator*(const double &B, const Profile &A)
	{
		//cout << "op* DP\n";

		Profile<T> profOut;
		for(Profile::const_iterator aIt = A.begin(); aIt != A.end(); aIt++)
			profOut[aIt->first] = aIt->second*B; 
		return profOut;

	}

	friend const Profile operator/(const Profile &A, const double &B)
	{
		_ASSERTE(B != 0);
		Profile<T> profOut;
		for(Profile::const_iterator aIt = A.begin(); aIt != A.end(); aIt++)
			profOut[aIt->first] = aIt->second/B; 
		return profOut;
	}

	friend const Profile operator/(const double &B, const Profile &A)
	{
		Profile<T> profOut;
		for(Profile::const_iterator aIt = A.begin(); aIt != A.end(); aIt++) {
			_ASSERTE(aIt->second != 0);
			profOut[aIt->first] = B/aIt->second;
		}
		return profOut;
	}

	/**
	 *
	 */
	friend const Profile operator-(const Profile &A) {
		//cout << "op- P\n";

		Profile profOut(A);
		map<double, T>::iterator lIt = profOut.data.begin();
		for(lIt; lIt != profOut.data.end(); lIt++)
			(*lIt).second = -(*lIt).second;
		return profOut;

	}

	/**
	 *
	 */
	friend const Profile operator-(const Profile &A, const Profile &B) {
		//cout << "op- PP\n";

		return (A + (-B));

	}

	/**
	 * Tests if two Profiles are equal element by element
	 */
	friend const bool operator==(const Profile &A, const Profile &B) 
	{
		map<double, T>::const_iterator aIt = A.data.begin();
		map<double, T>::const_iterator bIt = B.data.begin();
		for(aIt; aIt != A.data.end(); aIt++, bIt++) {
			if(!(*aIt == *bIt))
				return false;			
		}
		return true;
	}
	/**
	 * Merges profiles together.
	 * @param prof1 the first profile to merge into the result
	 * @param prof2 the second profile to merge into the result
	 * @return a profile containing the unique union of the two data sets
	 */
	friend const Profile<T> mergeProfiles(Profile<typename T>& prof1, Profile<typename T>& prof2)
	{
		Profile<T> profOut = prof1;
		profOut.data.insert(prof2.data.begin(), prof2.data.end());
		return profOut;
	}
};

/**
 *
 */
template<typename T> bool profTimeLesser(pair<double, T> elem1, pair<double, T> elem2) {
   return elem1.first < elem2.first; }

/**
 *
 */
template<typename T> bool profValLesser(pair<double, T> elem1, pair<double, T> elem2) {
   return elem1.second < elem2.second; }

/**
 *
 */
template <typename T> 
class StateVarKey 
{
	string varName; 

public:
	StateVarKey(string name) : varName(name) {}

	~StateVarKey() {}

	const string getNameString() const { return this->varName; }

	bool operator==(const StateVarKey& rhs) const {
		return this->varName == rhs.varName; }

	bool operator<(const StateVarKey& rhs) const {
		return this->varName < rhs.varName; }
};

}} // end namespace horizon::util

#endif //PROFILE_H
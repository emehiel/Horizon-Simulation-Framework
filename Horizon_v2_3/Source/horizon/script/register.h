/**
 * \file register.h
 * This file contains functions for all the LUA/C++ bindings via Luabind.
 * @author Brian Butler
 */

#ifndef REGISTER_H
#define REGISTER_H

// HSF Includes
#include "simParams.h"
//#include "horizon/assetSchedule.h"
#include "horizon/Event.h"
#include "horizon/State.h"
#include "horizon/Subsystem.h"
#include "horizon/systemSchedule.h"
#include "horizon/Target.h"
#include "horizon/Task.h"
#include "horizon/geom/Environment.h"
#include "horizon/geom/Sun.h"
#include "horizon/geom/geomUtil.h"
#include "horizon/geom/Position.h"
#include "horizon/sub/dep/Dependencies.h"
#include "horizon/sub/dep/NodeDependencies.h"
#include "horizon/sub/script/SubsystemParameters.h"
#include "horizon/util/Matrix.h"
#include "horizon/util/Profile.h"
#include "horizon/util/Quat.h"
#include "horizon/util/umath.h"
#include "horizon/util/eoms/EOMS.h"
#include "horizon/net/NetworkDataClient.h"
//#include "horizon/eval/SystemScheduleWrapper.h"

// C++ Includes
#include <iostream>
#include <fstream>
#include <sstream>

// LUA / Luabind Includes
#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include <luabind/out_value_policy.hpp>
#include <luabind/adopt_policy.hpp>
#include <luabind/iterator_policy.hpp>

// Boost Includes
#include <boost/shared_ptr.hpp>

// HSF Usings
using namespace horizon;
using namespace horizon::util;
using namespace horizon::util::eoms;
using namespace horizon::sub::dep;
//using namespace horizon::eval;
//using horizon::Subsystem;
using namespace horizon::geom;
using horizon::sub::script::SubsystemParameters;
using namespace horizon::net;

// C++ Usings
using namespace std;

// LUA / Luabind Usings
using namespace luabind;

namespace luabind {
	// These functions are needed to allow luabind to convert
	// smart pointer to raw pointer
	/*template<class T>
	T* get_pointer(boost::shared_ptr<T>& p) {
		return p.get();
	}

	template<class A>
	boost::shared_ptr<const A>* get_const_holder(boost::shared_ptr<A>*) {
		return 0;
	}*/

	/**
	 * This function is the callback error function used when Luabind calls lua_pcall.
	 */
	int luaErrFcn(lua_State* L);
}

namespace horizon {
	namespace script {

/**
 * Performs all Luabind bindings.
 * All the register_XXXX(lua_State *L) functions are called from this function.
 *
 * In addition, the following C++ to LUA binding is performed via Luabind.
 * This function is used to get the State corresponding to Asset <CODE>assetNum</CODE>
 * and is used for dependency functions.<BR>
 * <CODE>Dependencies::getState(int assetNum) -> getState(int assetNum)</CODE>
 *
 * @param L A pointer to the LUA state.
 * @author Brian Butler
 * @see register_State(lua_State *L)
 * @see register_Matrix(lua_State *L)
 * @see register_matrixIndex(lua_State *L)
 * @see register_Quat(lua_State *L)
 * @see register_simParams(lua_State *L)
 * @see register_allPairs(lua_State *L)
 * @see register_allProfiles(lua_State *L)
 * @see register_allStateVarKeys(lua_State *L)
 * @see register_Subsystem(lua_State *L)
 * @see register_SubsystemParameters(lua_State *L)
 * @see register_NodeDependencies(lua_State *L)
 * @see register_Environment(lua_State *L)
 * @see register_Sun(lua_State *L)
 * @see register_geomUtil(lua_State *L)
 * @see register_Position(lua_State *L)
 * @see register_EOMS(lua_State *L)
 * @see register_Task(lua_State *L)
 * @see register_Target(lua_State *L)
 * @see register_umath(lua_State *L)
 * @see register_Event(lua_State *L)
 * @see register_NetworkDataClient(lua_State *L)
 */
void register_LUAState(lua_State* L);

void register_Dependencies(lua_State* L);

/**
 * Performs all Luabind bindings for the State class.
 * The C++ State class is made available using the following LUA class name:<BR>
 * <CODE>State -> State</CODE><BR>
 *
 * The State class has the following constructors exposed to LUA:<BR>
 * <CODE>State()</CODE><BR>
 * <CODE>State(State stateToCopy)</CODE><BR>
 * <CODE>State(State prevState, double newTaskStart)</CODE><BR>
 *
 * The State class has the following methods exposed to LUA(accessible using the ':' operator in LUA):<BR>
 * <CODE>getPrevious()</CODE><BR>
 * <CODE>setEventStart(double es)</CODE><BR>
 * <CODE>setTaskStart(double ts)</CODE><BR>
 * <CODE>setTaskEnd(double te)</CODE><BR>
 * <CODE>setEventEnd(double ee)</CODE><BR> 
 * <CODE>getEventStart()</CODE><BR>
 * <CODE>getTaskStart()</CODE><BR>
 * <CODE>getTaskEnd()</CODE><BR>
 * <CODE>getEventEnd()</CODE><BR>
 *
 * <CODE>getLastValue(dStateVarKey key)</CODE><BR>
 * <CODE>getLastValue(fStateVarKey key)</CODE><BR>
 * <CODE>getLastValue(iStateVarKey key)</CODE><BR>
 * <CODE>getLastValue(bStateVarKey key)</CODE><BR>
 * <CODE>getLastValue(mStateVarKey key)</CODE><BR>
 * <CODE>getLastValue(qStateVarKey key)</CODE><BR>
 *
 * <CODE>getValueAtTime(dStateVarKey key, double time)</CODE><BR>
 * <CODE>getValueAtTime(fStateVarKey key, double time)</CODE><BR>
 * <CODE>getValueAtTime(iStateVarKey key, double time)</CODE><BR>
 * <CODE>getValueAtTime(bStateVarKey key, double time)</CODE><BR>
 * <CODE>getValueAtTime(mStateVarKey key, double time)</CODE><BR>
 * <CODE>getValueAtTime(qStateVarKey key, double time)</CODE><BR>
 *
 * <CODE>getProfile(dStateVarKey key)</CODE><BR>
 * <CODE>getProfile(fStateVarKey key)</CODE><BR>
 * <CODE>getProfile(iStateVarKey key)</CODE><BR>
 * <CODE>getProfile(bStateVarKey key)</CODE><BR>
 * <CODE>getProfile(mStateVarKey key)</CODE><BR>
 * <CODE>getProfile(qStateVarKey key)</CODE><BR>
 *			
 * <CODE>getFullProfile(dStateVarKey key)</CODE><BR>
 * <CODE>getFullProfile(fStateVarKey key)</CODE><BR>
 * <CODE>getFullProfile(iStateVarKey key)</CODE><BR>
 * <CODE>getFullProfile(bStateVarKey key)</CODE><BR>
 * <CODE>getFullProfile(mStateVarKey key)</CODE><BR>
 * <CODE>getFullProfile(qStateVarKey key)</CODE><BR>
 *
 * <CODE>setProfile(dStateVarKey key, dProfile prof)</CODE><BR>
 * <CODE>setProfile(fStateVarKey key, fProfile prof)</CODE><BR>
 * <CODE>setProfile(iStateVarKey key, iProfile prof)</CODE><BR>
 * <CODE>setProfile(bStateVarKey key, bProfile prof)</CODE><BR>
 * <CODE>setProfile(mStateVarKey key, mProfile prof)</CODE><BR>
 * <CODE>setProfile(qStateVarKey key, qProfile prof)</CODE><BR>
 *
 * <CODE>addValue(dStateVarKey key, dPair pair)</CODE><BR>
 * <CODE>addValue(fStateVarKey key, fPair pair)</CODE><BR>
 * <CODE>addValue(iStateVarKey key, iPair pair)</CODE><BR>
 * <CODE>addValue(bStateVarKey key, bPair pair)</CODE><BR>
 * <CODE>addValue(mStateVarKey key, mPair pair)</CODE><BR>
 * <CODE>addValue(qStateVarKey key, qPair pair)</CODE><BR>
 *
 * <CODE>addValue(dStateVarKey key, dProfile prof)</CODE><BR>
 * <CODE>addValue(fStateVarKey key, fProfile prof)</CODE><BR>
 * <CODE>addValue(iStateVarKey key, iProfile prof)</CODE><BR>
 * <CODE>addValue(bStateVarKey key, bProfile prof)</CODE><BR>
 * <CODE>addValue(mStateVarKey key, mProfile prof)</CODE><BR>
 * <CODE>addValue(qStateVarKey key, qProfile prof)</CODE><BR>
 *
 * @param L A pointer to the LUA state.
 * @author Brian Butler
 */
void register_State(lua_State* L);

/**
 * Performs all Luabind bindings for the Matrix class.
 * The C++ Matrix class is made available using the following LUA class name:<BR>
 * <CODE>Matrix -> Matrix</CODE><BR>
 *
 * The Matrix class has the following constructors exposed to LUA:<BR>
 * <CODE>Matrix()</CODE><BR>
 * <CODE>Matrix(unsigned n) --creates an n x n Matrix, all elements initialized to 0</CODE><BR>
 * <CODE>Matrix(unsigned n, unsigned m) --creates an n x m Matrix, all elements initialized to 0</CODE><BR>
 * <CODE>Matrix(unsigned n, unsigned m, double val) --creates an n x m Matrix, all elements initialized to val</CODE><BR>
 * <CODE>Matrix(Matrix mat)</CODE><BR>
 * <CODE>Matrix(Matrix mat, unsigned start, unsigned end)</CODE><BR>
 * <CODE>Matrix(Matrix mat, unsigned rStart, unsigned rEnd, unsigned cStart, unsigned cEnd)</CODE><BR>
 *
 * The Matrix class has the following methods exposed to LUA(accessible using the ':' operator in LUA):<BR>
 * <CODE>getValue(unsigned i, unsigned j)</CODE><BR>
 * <CODE>setValue(unsigned i, unsigned j, unsigned val)</CODE><BR>
 * <CODE>getNumRows()</CODE><BR>
 * <CODE>getNumCols()</CODE><BR>
 * <CODE>setNumRows(unsigned rows)</CODE><BR>
 * <CODE>setNumCols(unsigned cols)</CODE><BR>
 * <CODE>getNumElems()</CODE><BR>
 * <CODE>isSquare(double q2)</CODE><BR>
 *
 * The next function must make use of multiple return values in LUA. The C++ function writes to the values row and col passed in as a reference.
 * LUA cannot do this, instead the luabind policy <CODE>out_value</CODE> is used to add the return values as an additional outputs.<BR>
 * <CODE>getSize(unsigned row, unsigned col)</CODE><BR>
 * Ex:<BR>
 * <CODE>mat = Matrix(3,3)</CODE><BR>
 * <CODE>row = 0</CODE><BR>
 * <CODE>col = 0</CODE><BR>
 * <CODE>row, col = mat:getSize(row, col)</CODE><BR>
 * <CODE>-- row and col will have value 3</CODE><BR>
 *
 * <CODE>getColumn(unsigned col)</CODE><BR>
 * <CODE>setColumn(unsigned col, Matrix colData)</CODE><BR>
 * <CODE>getRow(unsigned row)</CODE><BR>
 * <CODE>setRow(unsigned row, Matrix rowData)</CODE><BR>
 * <CODE>getSubMatrix(matrixIndex ridx, matrixIndex cidx)</CODE><BR>
 * <CODE>setSubMatrix(matrixIndex ridx, matrixIndex cidx, Matrix subMatrix)</CODE><BR>
 *
 * The Matrix class has the following operators exposed to LUA:<BR>
 * <CODE>operator (unsigned idx)</CODE><BR>
 * <CODE>operator (unsigned row, unsigned col)</CODE><BR>
 * <CODE>operator Matrix + Matrix</CODE><BR>
 * <CODE>operator Matrix + double</CODE><BR>
 * <CODE>operator double + Matrix</CODE><BR>
 * <CODE>operator Matrix - Matrix</CODE><BR>
 * <CODE>operator Matrix - double</CODE><BR>
 * <CODE>operator double - Matrix</CODE><BR>
 * <CODE>operator Matrix * Matrix</CODE><BR>
 * <CODE>operator double * Matrix</CODE><BR>
 * <CODE>operator Matrix * double</CODE><BR>
 * <CODE>operator Matrix / Matrix</CODE><BR>
 * <CODE>operator Matrix / double</CODE><BR>
 * <CODE>operator ==</CODE><BR>
 *
 * The following free functions are also exposed to LUA:<BR>
 * <CODE>trace(Matrix mat)</CODE><BR>
 * <CODE>transpose(Matrix mat)</CODE><BR>
 * <CODE>meye(Matrix mat)</CODE><BR>
 * <CODE>vertcat(Matrix mat1, Matrix mat2)</CODE><BR>
 * <CODE>vertcat2(Matrix mat1, Matrix mat2)</CODE><BR>
 * <CODE>horzcat(Matrix mat1, Matrix mat2)</CODE><BR>
 * <CODE>horzcat2(Matrix mat1, Matrix mat2)</CODE><BR>
 * <CODE>norm(Matrix mat)</CODE><BR>
 * <CODE>mmax(Matrix mat)</CODE><BR>
 * <CODE>mmax(Matrix mat1, Matrix mat2)</CODE><BR>
 * <CODE>mmax(Matrix mat, double val)</CODE><BR>
 * <CODE>mmax(double val, Matrix mat)</CODE><BR>
 * <CODE>mmin(Matrix mat)</CODE><BR>
 * <CODE>mmin(Matrix mat1, Matrix mat2)</CODE><BR>
 * <CODE>mmin(Matrix mat, double val)</CODE><BR>
 * <CODE>mmin(double val, Matrix mat)</CODE><BR>
 * <CODE>mabs(Matrix mat)</CODE><BR>
 * <CODE>inv(Matrix mat)</CODE><BR>
 * <CODE>atom(string inMatrix)</CODE><BR>
 *
 * @param L A pointer to the LUA state.
 * @author Brian Butler
 */
void register_Matrix(lua_State* L);

/**
 * Performs all Luabind bindings for the matrixIndex class.
 * The C++ matrixIndex class is made available using the following LUA class name:<BR>
 * <CODE>matrixIndex -> matrixIndex</CODE><BR>
 *
 * The matrixIndex class has the following constructors exposed to LUA:<BR>
 * <CODE>Quat(unsigned idx)</CODE><BR>
 * <CODE>Quat(unsigned start_idx, unsigned end_idx)</CODE><BR>
 *
 * The matrixIndex class has the following methods exposed to LUA(accessible using the ':' operator in LUA):<BR>
 * <CODE>STARTIDX()</CODE><BR>
 * <CODE>ENDIDX()</CODE><BR>
 *
 * The following free functions are also exposed to LUA:<BR>
 * <CODE>colon(unsigned idx)</CODE><BR>
 * <CODE>colon(unsigned start_idx, unsigned end_idx)</CODE><BR> 
 *
 * @param L A pointer to the LUA state.
 * @author Brian Butler
 */
void register_matrixIndex(lua_State* L);

/**
 * Performs all Luabind bindings for the Quat class.
 * The C++ Quat class is made available using the following LUA class name:<BR>
 * <CODE>Quat -> Quat</CODE><BR>
 *
 * The Quat class has the following constructors exposed to LUA:<BR>
 * <CODE>Quat()</CODE><BR>
 * <CODE>Quat(double q0)</CODE><BR>
 * <CODE>Quat(double q1, double q2, double q3)</CODE><BR>
 * <CODE>Quat(double q0, double q1, double q2, double q3)</CODE><BR>
 * <CODE>Quat(Matrix v, double theta)</CODE><BR>
 * <CODE>Quat(Matrix c, Matrix d)</CODE><BR>
 *
 * The Quat class has the following methods exposed to LUA(accessible using the ':' operator in LUA):<BR>
 * <CODE>comp_q0()</CODE><BR>
 * <CODE>comp_q1()</CODE><BR>
 * <CODE>comp_q2()</CODE><BR>
 * <CODE>comp_q3()</CODE><BR>
 * <CODE>print()</CODE><BR>
 * <CODE>set_q0(double q0)</CODE><BR>
 * <CODE>set_q1(double q1)</CODE><BR>
 * <CODE>set_q2(double q2)</CODE><BR>
 * <CODE>set_q3(double q3)</CODE><BR>
 * <CODE>conjugate()</CODE><BR>
 * <CODE>norm()</CODE><BR>
 * <CODE>normalize()</CODE><BR>
 *
 * The Quat class has the following operators exposed to LUA:<BR>
 * <CODE>operator Quat + Quat</CODE><BR>
 * <CODE>operator Quat + double</CODE><BR>
 * <CODE>operator double + Quat</CODE><BR>
 * <CODE>operator Quat - Quat</CODE><BR>
 * <CODE>operator Quat - double</CODE><BR>
 * <CODE>operator double - Quat</CODE><BR>
 * <CODE>operator double * Quat</CODE><BR>
 * <CODE>operator Quat * double</CODE><BR>
 * <CODE>operator Quat * Quat</CODE><BR>
 * <CODE>operator ==</CODE><BR>
 *
 * @param L A pointer to the LUA state.
 * @author Brian Butler
 */
void register_Quat(lua_State* L);

/**
 * Performs all Luabind bindings for the functions in namespace <CODE>simParams</CODE>.
 *
 * The C++ function call and the equivalent function in LUA are shown below:<BR>
 * <CODE>simParams::SIMSTART_JD() -> SIMSTART_JD()</CODE><BR>
 * <CODE>simParams::SIMSTART_SECONDS() -> SIMSTART_SECONDS()</CODE><BR>
 * <CODE>simParams::SIMEND_SECONDS() -> SIMEND_SECONDS()</CODE><BR>
 *
 * Ex:<BR>
 * <CODE>startJD = SIMSTART_JD()</CODE><BR>
 * <CODE>startSeconds = SIMSTART_SECONDS()</CODE><BR>
 * <CODE>endSeconds = SIMEND_SECONDS()</CODE><BR>
 *
 * @param L A pointer to the LUA state.
 * @author Brian Butler
 */
void register_simParams(lua_State* L);

/**
 * Performs all Luabind bindings for Pair classes.
 * Since Luabind cannot create bindings to templated classes, only specific instances,
 * only the types of Pair that will be used in simulations are binded.
 * The following instances of the Pair class are available using the following LUA class names:<BR>
 * <CODE>Pair<double, double> -> dPair</CODE><BR>
 * <CODE>Pair<double, float> -> fPair</CODE><BR>
 * <CODE>Pair<double, int> -> iPair</CODE><BR>
 * <CODE>Pair<double, bool> -> bPair</CODE><BR>
 * <CODE>Pair<double, Matrix> -> mPair</CODE><BR>
 * <CODE>Pair<double, Quat> -> qPair</CODE><BR>
 *
 * The Pair classes have the following constructors exposed to LUA:<BR>
 * <CODE>Pair()</CODE><BR>
 * <CODE>Pair(double first, T second)</CODE><BR>
 * Ex:<BR>
 * <CODE>p1 = dPair()</CODE><BR>
 * <CODE>p2 = dPair(1.0, 3.46)</CODE><BR>
 *
 * The Pair classes have the following properties exposed to LUA(accessible using the '.' operator in LUA):<BR>
 * <CODE>first</CODE><BR>
 * <CODE>second</CODE><BR>
 * Ex:<BR>
 * <CODE>p1 = dPair()</CODE><BR>
 * <CODE>p1.first = 1.0</CODE><BR>
 * <CODE>p1.second = 3.46</CODE><BR>
 * <CODE>p2 = dPair(p1.first, p1.second)</CODE><BR>
 *
 * The Pair classes have the following methods exposed to LUA(accessible using the ':' operator in LUA):<BR>
 * <CODE>swap(Pair)</CODE><BR>
 * Ex:<BR>
 * <CODE>p1 = dPair(1.0, 2.0)</CODE><BR>
 * <CODE>p2 = dPair(1.0, 3.0)</CODE><BR>
 * p1:swap(p2)
 * -- p1.first now equals 1.0
 * -- p1.second now equals 3.0
 *
 * @param L A pointer to the LUA state.
 * @author Brian Butler
 */
void register_allPairs(lua_State* L);

/**
 * Performs all Luabind bindings for Profile classes.
 * Since Luabind cannot create bindings to templated classes, only specific instances,
 * only the types of Profile that will be used in simulations are binded.
 * The following instances of the Profile class are available using the following LUA class names:<BR>
 * <CODE>Profile<double> -> dProfile</CODE><BR>
 * <CODE>Profile<float> -> fProfile</CODE><BR>
 * <CODE>Profile<int> -> iProfile</CODE><BR>
 * <CODE>Profile<bool> -> bProfile</CODE><BR>
 * <CODE>Profile<Matrix> -> mProfile</CODE><BR>
 * <CODE>Profile<Quat> -> qProfile</CODE><BR>
 *
 * The Profile classes have the following constructors exposed to LUA:<BR>
 * <CODE>Profile()</CODE><BR>
 * <CODE>Profile(Profile prof)</CODE><BR>
 * <CODE>Profile(Pair pair)</CODE><BR>
 * <CODE>Profile(double time, T val)</CODE><BR>
 *
 * The Profile classes have the following methods exposed to LUA(accessible using the ':' operator in LUA):<BR>
 * <CODE>front()</CODE><BR>
 * <CODE>back()</CODE><BR>
 * <CODE>size()</CODE><BR>
 * <CODE>empty()</CODE><BR>
 * <CODE>addData(Pair data)</CODE><BR>
 * <CODE>addData(double time, T val)</CODE><BR>
 * <CODE>addData(Profile data)</CODE><BR>
 * <CODE>getLastValue()</CODE><BR>
 * <CODE>getValueAtTime(double time)</CODE><BR>
 * <CODE>removeDuplicates()</CODE><BR>
 * <CODE>getLastPair()</CODE><BR>
 * <CODE>getPairAtTime(double time)</CODE><BR>
 * <CODE>getBeginTime()</CODE><BR>
 *
 * The Profile classes have the following operators exposed to LUA:<BR>
 * <CODE>operator (size_t time)</CODE><BR>
 *
 * In addition to the methods and operators above, the profile types <CODE>dProfile</CODE>
 * <CODE>fProfile</CODE>, and <CODE>iProfile</CODE> have the following additional methods and operators:<BR>
 * <CODE>pmin()</CODE><BR>
 * <CODE>pmax()</CODE><BR>
 * <CODE>sum()</CODE><BR>
 *
 * <CODE>integrate(double start, double end, T initalValue)</CODE><BR>
 * <CODE>integrateToProf(double start, double end, double saveFreq, T initialValue, T initialCondition)</CODE><BR>
 *
 * The next two functions must make use of multiple return values in LUA. The C++ functions write return values to the bool value passed in as a reference. LUA cannot do this,
 * instead the luabind policy <CODE>out_value</CODE> is used to add that return value as an additional output.<BR>
 * Ex:<BR>
 * <CODE>exceeded = true</CODE><BR>
 * <CODE>prof, exceeded = lowerLimitIntegrateToProf(start, end, saveFreq, lowerBound, exceeded, initialValue, initialCondition)</CODE><BR>
 * <CODE>--exceeded will now contain the value that would be written to the bool value reference passed in if the function was called using C++</CODE><BR>
 *
 * <CODE>lowerLimitIntegrateToProf(double start, double end, double saveFreq, T lowerBound, bool exceeded, T initialValue, T initialCondition)</CODE><BR>
 * <CODE>upperLimitIntegrateToProf(double start, double end, double saveFreq, T upperBound, bool exceeded, T initialValue, T initialCondition)</CODE><BR>
 *
 * <CODE>operator Profile + Profile</CODE><BR>
 * <CODE>operator Profile + double</CODE><BR>
 * <CODE>operator double + Profile</CODE><BR>
 * <CODE>operator - Profile</CODE><BR>
 * <CODE>operator Profile - Profile</CODE><BR>
 * <CODE>operator Profile - double</CODE><BR>
 * <CODE>operator double - Profile</CODE><BR>
 * <CODE>operator Profile * double</CODE><BR>
 * <CODE>operator double * Profile</CODE><BR>
 * <CODE>operator Profile / double</CODE><BR>
 * <CODE>operator double / Profile</CODE><BR>
 *
 * @param L A pointer to the LUA state.
 * @author Brian Butler
 */
void register_allProfiles(lua_State* L);

/**
 * Performs all Luabind bindings for StateVarKey classes.
 * Since Luabind cannot create bindings to templated classes, only specific instances,
 * only the types of StateVarKey that will be used in simulations are binded.
 * The following instances of the StateVarKey class are available using the following LUA class names:<BR>
 * <CODE>StateVarKey<double> -> dStateVarKey</CODE><BR>
 * <CODE>StateVarKey<float> -> fStateVarKey</CODE><BR>
 * <CODE>StateVarKey<int> -> iStateVarKey</CODE><BR>
 * <CODE>StateVarKey<bool> -> bStateVarKey</CODE><BR>
 * <CODE>StateVarKey<Matrix> -> mStateVarKey</CODE><BR>
 * <CODE>StateVarKey<Quat> -> qStateVarKey</CODE><BR>
 *
 * The StateVarKey classes have the following constructors exposed to LUA:<BR>
 * <CODE>StateVarKey(string name)</CODE><BR>
 * <CODE>StateVarKey(StateVarKey key)</CODE><BR>
 *
 * The StateVarKey classes have the following methods exposed to LUA(accessible using the ':' operator in LUA):<BR>
 * <CODE>getNameString()</CODE><BR>
 *
 * @param L A pointer to the LUA state.
 * @author Brian Butler
 */
void register_allStateVarKeys(lua_State* L);

/**
 * Performs all Luabind bindings for the Subsystem class.
 * The C++ Subsystem class is made available using the following LUA class name:<BR>
 * <CODE>Subsystem -> Subsystem</CODE><BR>
 *
 * The Subsystem class has the following constructors exposed to LUA:<BR>
 * <CODE>Environment()</CODE><BR>
 *
 * The Subsystem class has the following methods exposed to LUA(accessible using the ':' operator in LUA):<BR>
 * <CODE>getName()</CODE><BR>
 * <CODE>addKey(dStateVarKey key)</CODE><BR>
 * <CODE>addKey(fStateVarKey key)</CODE><BR>
 * <CODE>addKey(iStateVarKey key)</CODE><BR>
 * <CODE>addKey(bStateVarKey key)</CODE><BR>
 * <CODE>addKey(mStateVarKey key)</CODE><BR>
 * <CODE>addKey(qStateVarKey key)</CODE><BR>
 *
 * @param L A pointer to the LUA state.
 * @author Brian Butler
 */
void register_Subsystem(lua_State* L);

/**
 * Performs all Luabind bindings for the SubsystemParameters class.
 * The C++ SubsystemParameters class is made available using the following LUA class name:<BR>
 * <CODE>SubsystemParameters -> SubsystemParameters</CODE><BR>
 *
 * The SubsystemParameters class has the following constructors exposed to LUA:<BR>
 * <CODE>SubsystemParameters()</CODE><BR>
 *
 * The SubsystemParameters class has the following methods exposed to LUA(accessible using the ':' operator in LUA):<BR>
 * <CODE>addDoubleParameter(string key, double value)</CODE><BR>
 * <CODE>addFloatParameter(string key, float value)</CODE><BR>
 * <CODE>addIntParameter(string key, int value)</CODE><BR>
 * <CODE>addBoolParameter(string key, bool value)</CODE><BR>
 * <CODE>addMatrixParameter(string key, Matrix value)</CODE><BR>
 * <CODE>addQuatParameter(string key, Quat value)</CODE><BR>
 * <CODE>getDoubleParameter(string key)</CODE><BR>
 * <CODE>getFloatParameter(string key)</CODE><BR>
 * <CODE>getIntParameter(string key)</CODE><BR>
 * <CODE>getBoolParameter(string key)</CODE><BR>
 * <CODE>getMatrixParameter(string key)</CODE><BR>
 * <CODE>getQuatParameter(string key)</CODE><BR>
 *
 * @param L A pointer to the LUA state.
 * @author Brian Butler
 */
void register_SubsystemParameters(lua_State* L);

/**
 * Performs all Luabind bindings for the NodeDependencies class.
 * The C++ NodeDependencies class is made available using the following LUA class name:<BR>
 * <CODE>NodeDependencies -> NodeDependencies</CODE><BR>
 *
 * The NodeDependencies class has the following constructors exposed to LUA:<BR>
 * <CODE>NodeDependencies()</CODE><BR>
 *
 * The NodeDependencies class has the following methods exposed to LUA(accessible using the ':' operator in LUA):<BR>
 * <CODE>callDoubleDependency(string key)</CODE><BR>
 * <CODE>callFloatDependency(string key)</CODE><BR>
 * <CODE>callIntDependency(string key)</CODE><BR>
 * <CODE>callBoolDependency(string key)</CODE><BR>
 * <CODE>callMatrixDependency(string key)</CODE><BR>
 * <CODE>callQuatDependency(string key)</CODE><BR>
 * <CODE>enableScriptingSupport()</CODE><BR>
 * <CODE>disableScriptingSupport()</CODE><BR>
 *
 * @param L A pointer to the LUA state.
 * @author Brian Butler
 */
void register_NodeDependencies(lua_State* L);

/**
 * Performs all Luabind bindings for the Environment class.
 * The C++ Environment class is made available using the following LUA class name:<BR>
 * <CODE>Environment -> Environment</CODE><BR>
 *
 * The Environment class has the following constructors exposed to LUA:<BR>
 * <CODE>Environment()</CODE><BR>
 * <CODE>Environment(Sun sun)</CODE><BR>
 *
 * The Environment class has the following methods exposed to LUA(accessible using the ':' operator in LUA):<BR>
 * <CODE>getSun()</CODE><BR>
 *
 * @param L A pointer to the LUA state.
 * @author Brian Butler
 */
void register_Environment(lua_State* L);

/**
 * Performs all Luabind bindings for the Sun class.
 * The C++ Sun class is made available using the following LUA class name:<BR>
 * <CODE>Sun -> Sun</CODE><BR>
 *
 * The Sun class has the following constructors exposed to LUA:<BR>
 * <CODE>Sun()</CODE><BR>
 * <CODE>Sun(bool isEarSunVecConstant)</CODE><BR>
 *
 * The Sun class has the following methods exposed to LUA(accessible using the ':' operator in LUA):<BR>
 * <CODE>castShadowOnPos()</CODE><BR>
 * <CODE>getSolarConstant()</CODE><BR>
 * <CODE>getEarSunVec()</CODE><BR>
 *
 * The enumeration <CODE>shadow_state</CODE> is also made accessible to LUA and has the following possible values:
 * <CODE>Sun.NO_SHADOW</CODE><BR>
 * <CODE>Sun.UMBRA</CODE><BR>
 * <CODE>Sun.PENUMBRA</CODE><BR>
 *
 * Ex:<BR>
 * <CODE>sun = Sun()</CODE><BR>
 * <CODE>--assume a Position with identifier pos exists</CODE><BR>
 * <CODE>if sun:castShadowOnPos(pos, 0) == Sun.UMBRA then</CODE><BR>
 * <CODE>   ...</CODE><BR>
 * <CODE>end</CODE><BR>
 *
 * @param L A pointer to the LUA state.
 * @author Brian Butler
 */
void register_Sun(lua_State* L);

/**
 * Performs Luabind bindings for functions in the namespace <CODE>horizon::geom</CODE>.
 * The C++ functions are made available using the following LUA function names:<BR>
 * <CODE>hasLOS -> hasLOS</CODE><BR>
 * <CODE>LLA2ECI -> LLA2ECI</CODE><BR>
 * <CODE>ECI2LLA -> ECI2LLA</CODE><BR>
 * <CODE>HMS2UT -> HMS2UT</CODE><BR>
 * <CODE>YMDUT2JD -> YMDUT2JD</CODE><BR>
 * <CODE>CT2LST -> CT2LST</CODE><BR>
 *
 * @param L A pointer to the LUA state.
 * @author Brian Butler
 */
void register_geomUtil(lua_State* L);

/**
 * Performs all Luabind bindings for the Position class.
 * The C++ Position class is made available using the following LUA class name:<BR>
 * <CODE>Position -> Position</CODE><BR>
 *
 * The Position class has the following constructors exposed to LUA:<BR>
 * <CODE>Position(XMLNode targetNode)</CODE><BR>
 * <CODE>Position(string type, double data1, double data2, double data3)</CODE><BR>
 * <CODE>Position(string type, Matrix data)</CODE><BR>
 * <CODE>Position(string type, EOMS eoms, double data1, double data2, double data3, double data4, double data5, double data6)</CODE><BR>
 * <CODE>Position(string type, EOMS eoms, Matrix data)</CODE><BR>
 *
 * The Position class has the following methods exposed to LUA(accessible using the ':' operator in LUA):<BR>
 * <CODE>setPosDataTimeStep(double step)</CODE><BR>
 * <CODE>getType()</CODE><BR>
 * <CODE>getEOMS()</CODE><BR>
 * <CODE>getIC()</CODE><BR>
 * <CODE>getStateECI(double time)</CODE><BR>
 * <CODE>getStateAtTime(double time)</CODE><BR>
 * <CODE>getPosECI(double time)</CODE><BR>
 * <CODE>getVelECI(double time)</CODE><BR>
 * <CODE>getPosLLA(double time)</CODE><BR>
 * <CODE>hasLOSTo(Position pos, double time)</CODE><BR>
 * <CODE>hasLOSTo(Matrix pos, double time)</CODE><BR>
 *
 * @param L A pointer to the LUA state.
 * @author Brian Butler
 */
void register_Position(lua_State* L);

/**
 * Performs all Luabind bindings for the EOMS class.
 * The C++ EOMS class is made available using the following LUA class name:<BR>
 * <CODE>EOMS -> EOMS</CODE><BR>
 *
 * The EOMS class has the following operators exposed to LUA:<BR>
 * <CODE>operator (double, Matrix)</CODE><BR>
 *
 * @param L A pointer to the LUA state.
 * @author Brian Butler
 */
void register_EOMS(lua_State* L);

/**
 * Performs all Luabind bindings for the Task class.
 * The C++ Task class is made available using the following LUA class name:<BR>
 * <CODE>Task -> Task</CODE><BR>
 *
 * The Task class has the following constructors exposed to LUA:<BR>
 * <CODE>Task()</CODE><BR>
 * <CODE>Task(string name, Target target, size_t maxTimesPerformable)</CODE><BR>
 *
 * The Task class has the following methods exposed to LUA(accessible using the ':' operator in LUA):<BR>
 * <CODE>getType()</CODE><BR>
 * <CODE>getTarget()</CODE><BR>
 * <CODE>getMaxTimesPerformable()</CODE><BR>
 *
 * @param L A pointer to the LUA state.
 * @author Brian Butler
 */
void register_Task(lua_State* L);

/**
 * Performs all Luabind bindings for the Target class.
 * The C++ Target class is made available using the following LUA class name:<BR>
 * <CODE>Target -> Target</CODE><BR>
 *
 * The Target class has the following constructors exposed to LUA:<BR>
 * <CODE>Task(XMLNode targetNode)</CODE><BR>
 * <CODE>Task(string name, Position position)</CODE><BR>
 * <CODE>Task(string name, Position position, int value)</CODE><BR>
 *
 * The Target class has the following methods exposed to LUA(accessible using the ':' operator in LUA):<BR>
 * <CODE>getName()</CODE><BR>
 * <CODE>getType()</CODE><BR>
 * <CODE>getValue()</CODE><BR>
 * <CODE>getPos()</CODE><BR>
 *
 * The Target class has the following operators exposed to LUA:<BR>
 * <CODE>operator ==</CODE><BR>
 *
 * @param L A pointer to the LUA state.
 * @author Brian Butler
 */
void register_Target(lua_State* L);

/**
 * Performs Luabind bindings for functions in the namespace <CODE>horizon::util::umath</CODE>.
 * The C++ functions are made available using the following LUA function names:<BR>
 * <CODE>cross -> cross</CODE><BR>
 * <CODE>dot -> dot</CODE><BR>
 *
 * @param L A pointer to the LUA state.
 * @author Brian Butler
 */
void register_umath(lua_State* L);

/**
 * Performs all Luabind bindings for the Event class.
 * The C++ Event class is made available using the following LUA class name:<BR>
 * <CODE>Event -> Event</CODE><BR>
 *
 * The Event class has the following methods exposed to LUA(accessible using the ':' operator in LUA):<BR>
 * <CODE>getTask()</CODE><BR>
 * <CODE>getState()</CODE><BR>
 *
 * @param L A pointer to the LUA state.
 * @author Brian Butler
 */
void register_Event(lua_State* L);

/**
 * Performs all Luabind bindings for the NetworkDataClient class.
 * The C++ NetworkDataClient class is made available using the following LUA class name:<BR>
 * <CODE>NetworkDataClient -> NetworkDataClient</CODE><BR>
 *
 * The host and port that a connection is desired to must be set before it is accessed using the LUA functions:<BR>
 * <CODE>setHost(host)</CODE><BR>
 * <CODE>setPort(port)</CODE><BR>
 *
 * The connection can be made by calling one of the following:<BR>
 * <CODE>Connect(host, port)</CODE><BR>
 * <CODE>Connect()</CODE><BR>
 *
 * The client will be disconnected from the server when the following is called:<BR>
 * <CODE>Disconnect(host, por)</CODE><BR>
 *
 * Since the NetworkDataClient is a Singleton object, it is accessed with following LUA function:<BR>
 * <CODE>getNetworkDataClient()</CODE><BR>
 *
 * The connection can be tested using the following LUA function:<BR>
 * <CODE>isOpen()</CODE><BR>
 *
 * The NetworkDataClient class has the following methods exposed to LUA(accessible using the ':' operator in LUA):<BR>
 * <CODE>requestData_Int(string var, double time, HandlerType type)</CODE><BR>
 * <CODE>requestData_IntProfile(string var, double start, double end, HandlerType type)</CODE><BR>
 * <CODE>requestData_Double(string var, double time, HandlerType type)</CODE><BR>
 * <CODE>requestData_DoubleProfile(string var, double start, double end, HandlerType type)</CODE><BR>
 * <CODE>requestData_Float(string var, double time, HandlerType type)</CODE><BR> 
 * <CODE>requestData_FloatProfile(string var, double start, double end, HandlerType type)</CODE><BR>
 * <CODE>requestData_Bool(string var, double time, HandlerType type)</CODE><BR>
 * <CODE>requestData_BoolProfile(string var, double start, double end, HandlerType type)</CODE><BR>
 * <CODE>requestData_Matrix(string var, double time, HandlerType type)</CODE><BR>
 * <CODE>requestData_MatrixProfile(string var, double start, double end, HandlerType type)</CODE><BR>
 * <CODE>requestData_Quat(string var, double time, HandlerType type)</CODE><BR>
 * <CODE>requestData_QuatProfile(string var, double start, double end, HandlerType type)</CODE><BR>
 *
 * The HandlerType enum is exposed to lua as a class, with the following possible values:<BR>
 * <CODE>HandlerType.ANY_DATA_HANDLER</CODE><BR>
 * <CODE>HandlerType.FLAT_FILE_DATA_HANDLER</CODE><BR>
 * <CODE>HandlerType.XML_FILE_DATA_HANDLER</CODE><BR>
 *
 * @param L A pointer to the LUA state.
 * @author Brian Butler
 */
void register_NetworkDataClient(lua_State* L);

//void register_SystemScheduleWrapper(lua_State* L);

/**
 * Load LUA functions from the specified file.
 * Any LUA funcitions found in the file specified by <CODE>filename</CODE> will
 * be parsed and available to LUA.
 *
 * @param L A pointer to the LUA state.
 * @param filename The file to parse for LUA functions.
 * @return true if all functions were parsed properly, false otherwise.
 * @author Brian Butler
 */
bool loadLuaFunctionsFromFile(lua_State* L, string filename);

/**
 * Load LUA functions from the specified string.
 * Any LUA funcitions found in the string <CODE>fcns</CODE> will
 * be parsed and available to LUA.
 *
 * @param L A pointer to the LUA state.
 * @param fcns The string to parse for LUA functions.
 * @return true if all functions were parsed properly, false otherwise.
 * @author Brian Butler
 */
bool loadLuaFunctionsFromString(lua_State* L, string fcns);

/**
 * Creates bindings for Pair classes.
 * This function is called by register_allPairs(lua_State* L) for each template
 * instantiation of the Pair class to be binded.
 *
 * @param pa The Luabind class to add Pair definitions to.
 * @see register_allPairs(lua_State* L)
 * @author Brian Butler
 */
template <typename T>
void register_pair(class_< pair<double, T> >& pa) {
	pa.def(constructor<double, T>());
	pa.def(constructor<pair<double, T>&>());
	pa.def_readwrite("first", &pair<double, T>::first);
	pa.def_readwrite("second", &pair<double, T>::second);
	//pa.def("swap", &pair<double, T>::swap);
}

/**
 * Creates bindings for Profile classes.
 * This function is called by register_allProfiles(lua_State* L) for each template
 * instantiation of the Profile class to be binded.
 *
 * @param prof The Luabind class to add Profile definitions to.
 * @see register_allProfiles(lua_State* L)
 * @author Brian Butler
 */
template <typename T>
void register_Profile(class_< Profile<T> >& prof) {
	prof.def(constructor<>());
	prof.def(constructor<const Profile<T>&>());
	prof.def(constructor<const pair<double, T>&>());
	prof.def(constructor<const double, const T&>());
	prof.def("front", &Profile<T>::front);
	prof.def("back", &Profile<T>::back);
	prof.def("size", &Profile<T>::size);
	prof.def("empty", &Profile<T>::empty);
	prof.def("addData", (void(Profile<T>::*)(const pair<double, T>&))&Profile<T>::addData);
	prof.def("addData", (void(Profile<T>::*)(const double, const T&))&Profile<T>::addData);
	prof.def("addData", (void(Profile<T>::*)(const Profile<T>&))&Profile<T>::addData);
	prof.def("getLastValue", &Profile<T>::getLastValue);
	prof.def("getValueAtTime", &Profile<T>::getValueAtTime);
	prof.def("removeDuplicates", &Profile<T>::removeDuplicates);
	prof.def("getLastPair", &Profile<T>::getLastPair);
	prof.def("getPairAtTime", &Profile<T>::getPairAtTime);
	prof.def(self(size_t()));

	prof.def("getBeginTime", &Profile<T>::getBeginTime);
}

/**
 * Creates bindings for Profile classes.
 * This function is called by register_allProfiles(lua_State* L) for each template
 * instantiation of the Profile class to be binded.
 *
 * @param prof The Luabind class to add Profile definitions to.
 * @see register_allProfiles(lua_State* L)
 * @author Brian Butler
 */
template <typename T>
void register_Profile_additional(class_< Profile<T> >& prof) {
	prof.def("pmin", &Profile<T>::pmin);
	prof.def("pmax", &Profile<T>::pmax);
	prof.def("sum", &Profile<T>::sum);
	prof.def("integrate", &Profile<T>::integrate);
	prof.def("integrateToProf", &Profile<T>::integrateToProf);
	prof.def("lowerLimitIntegrateToProf", &Profile<T>::lowerLimitIntegrateToProf, out_value(_6));
	prof.def("upperLimitIntegrateToProf", &Profile<T>::upperLimitIntegrateToProf, out_value(_6));

	prof.def(const_self + other<const Profile<T>&>());
	prof.def(const_self + other<const double&>());
	prof.def(other<const double&>() + const_self);
	prof.def(- const_self);
	prof.def(const_self - other<const Profile<T>&>());
	prof.def(const_self - other<const double&>());
	prof.def(other<const double&>() - const_self);
	prof.def(const_self * other<const double&>());
	prof.def(other<const double&>() * const_self);
	prof.def(const_self / other<const double&>());
	prof.def(other<const double&>() / const_self);
}

/**
 * Creates bindings for StateVarKey classes.
 * This function is called by register_allStateVarKeys(lua_State* L) for each template
 * instantiation of the StateVarKey class to be binded.
 *
 * @param svk The Luabind class to add StateVarKey definitions to.
 * @see register_allStateVarKeys(lua_State* L)
 * @author Brian Butler
 */
template <typename T>
void register_StateVarKey(class_< StateVarKey<T> >& svk) {
	svk.def(constructor<string>());
	svk.def(constructor<StateVarKey<T>&>());
	svk.def("getNameString", &StateVarKey<T>::getNameString);
}

}}
#endif
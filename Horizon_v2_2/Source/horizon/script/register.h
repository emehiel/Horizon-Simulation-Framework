#ifndef REGISTER_H
#define REGISTER_H

#include "horizon/util/Profile.h"
#include "horizon/util/Matrix.h"
#include "horizon/util/Quat.h"
#include "horizon/State.h"
#include "horizon/sub/dep/Dependencies.h"
#include "simParams.h"
#include <iostream>
#include <fstream>
#include <sstream>
// Lua / Luabind Includes
#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include <luabind/out_value_policy.hpp>
#include <luabind/adopt_policy.hpp>

using namespace horizon::util;
using namespace horizon::sub::dep;
using namespace std;
using namespace luabind;

namespace horizon {
	namespace script {

void register_dependencyLuaState(lua_State* L);

void register_State(lua_State* L);

void register_Matrix(lua_State* L);

void register_matrixIndex(lua_State* L);

void register_Quat(lua_State* L);

void register_simParams(lua_State* L);

void register_allPairs(lua_State* L);

void register_allProfiles(lua_State* L);

void register_allStateVarKeys(lua_State* L);

bool loadLuaFunctionsFromFile(lua_State* L, string filename);

bool loadLuaFunctionsFromString(lua_State* L, string fcns);

template <typename T>
void register_pair(class_< pair<double, T> >& pa) {
	pa.def(constructor<double, T>());
	pa.def(constructor<pair<double, T>&>());
	pa.def_readwrite("first", &pair<double, T>::first);
	pa.def_readwrite("second", &pair<double, T>::second);
	pa.def("swap", &pair<double, T>::swap);
}

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

template <typename T>
void register_Profile_additional(class_< Profile<T> >& prof) {
	prof.def("pmin", &Profile<T>::pmin);
	prof.def("pmax", &Profile<T>::pmax);
	prof.def("sum", &Profile<T>::sum);
	prof.def("integrate", &Profile<T>::integrate);
	prof.def("integrateToProf", &Profile<T>::integrateToProf);
	prof.def("lowerLimitIntegrateToProf", &Profile<T>::lowerLimitIntegrateToProf);
	prof.def("upperLimitIntegrateToProf", &Profile<T>::upperLimitIntegrateToProf);

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

template <typename T>
void register_StateVarKey(class_< StateVarKey<T> >& svk) {
	svk.def(constructor<string>());
	svk.def(constructor<StateVarKey<T>&>());
	svk.def("getNameString", &StateVarKey<T>::getNameString);
}

}}
#endif
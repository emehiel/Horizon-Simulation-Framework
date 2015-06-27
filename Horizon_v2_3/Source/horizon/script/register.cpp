#include "register.h"

namespace luabind {
int luaErrFcn(lua_State* L) {
	lua_Debug d;
	lua_getstack(L, 1, &d);
	lua_getinfo(L, "Sln", &d);
	std::string err = lua_tostring(L, -1);
	lua_pop(L, 1);
	std::stringstream msg;
	msg << d.short_src << ":" << d.what;
	if(d.name != NULL) {
		msg << "(" << d.namewhat << " " << d.name << ")";
	}
	msg << " " << err;
	lua_pushstring(L, msg.str().c_str());
	return 1;
}
}

namespace horizon {
	namespace script {

void register_LUAState(lua_State* L) {
	register_simParams(L);
	register_allPairs(L);
	register_allProfiles(L);
	register_State(L);
	register_Matrix(L);
	register_Quat(L);
	register_matrixIndex(L);
	register_allStateVarKeys(L);
	register_Subsystem(L);
	register_SubsystemParameters(L);
	register_NodeDependencies(L);
	register_Environment(L);
	register_Sun(L);
	register_geomUtil(L);
	register_Position(L);
	register_EOMS(L);
	register_Task(L);
	register_Target(L);
	register_umath(L);
	register_Event(L);
	register_NetworkDataClient(L);
	register_Dependencies(L);
	//register_SystemScheduleWrapper(L);

	module(L) [
		def("getState", &Dependencies::getState)
	];
}

void register_Dependencies(lua_State* L) {
	module(L) [
		class_<Dependencies>("Dependencies")
			.def("getAssetState", &Dependencies::getAssetState)
	];
}

void register_State(lua_State* L) {
	module(L) [
		class_<State>("State")
			.def(constructor<>())
			.def(constructor<State*>())
			.def(constructor<State*, const double>())
			.def("getPrevious", &State::getPrevious)
			.def("setEventStart", &State::setEventStart)
			.def("setTaskStart", &State::setTaskStart)
			.def("setTaskEnd", &State::setTaskEnd)
			.def("setEventEnd", &State::setEventEnd)
			.def("getEventStart",&State::getEventStart)
			.def("getTaskStart",&State::getTaskStart)
			.def("getTaskEnd",&State::getTaskEnd)
			.def("getEventEnd",&State::getEventEnd)

			.def("getLastValue", (pair<double, int> (State::*)(const StateVarKey<int>&) const)&State::getLastValue)
			.def("getLastValue", (pair<double, double> (State::*)(const StateVarKey<double>&) const)&State::getLastValue)
			.def("getLastValue", (pair<double, float> (State::*)(const StateVarKey<float>&) const)&State::getLastValue)
			.def("getLastValue", (pair<double, bool> (State::*)(const StateVarKey<bool>&) const)&State::getLastValue)
			.def("getLastValue", (pair<double, Matrix> (State::*)(const StateVarKey<Matrix>&) const)&State::getLastValue)
			.def("getLastValue", (pair<double, Quat> (State::*)(const StateVarKey<Quat>&) const)&State::getLastValue)

			.def("getValueAtTime", (pair<double, int> (State::*)(const StateVarKey<int>&, const double) const)&State::getValueAtTime)
			.def("getValueAtTime", (pair<double, double> (State::*)(const StateVarKey<double>&, const double) const)&State::getValueAtTime)
			.def("getValueAtTime", (pair<double, float> (State::*)(const StateVarKey<float>&, const double) const)&State::getValueAtTime)
			.def("getValueAtTime", (pair<double, bool> (State::*)(const StateVarKey<bool>&, const double) const)&State::getValueAtTime)
			.def("getValueAtTime", (pair<double, Matrix> (State::*)(const StateVarKey<Matrix>&, const double) const)&State::getValueAtTime)
			.def("getValueAtTime", (pair<double, Quat> (State::*)(const StateVarKey<Quat>&, const double) const)&State::getValueAtTime)

			.def("getProfile", (Profile<int> (State::*)(const StateVarKey<int>&) const)&State::getProfile)
			.def("getProfile", (Profile<double> (State::*)(const StateVarKey<double>&) const)&State::getProfile)
			.def("getProfile", (Profile<float> (State::*)(const StateVarKey<float>&) const)&State::getProfile)
			.def("getProfile", (Profile<bool> (State::*)(const StateVarKey<bool>&) const)&State::getProfile)
			.def("getProfile", (Profile<Matrix> (State::*)(const StateVarKey<Matrix>&) const)&State::getProfile)
			.def("getProfile", (Profile<Quat> (State::*)(const StateVarKey<Quat>&) const)&State::getProfile)

			.def("getFullProfile", (Profile<int> (State::*)(const StateVarKey<int>&) const)&State::getFullProfile)
			.def("getFullProfile", (Profile<double> (State::*)(const StateVarKey<double>&) const)&State::getFullProfile)
			.def("getFullProfile", (Profile<float> (State::*)(const StateVarKey<float>&) const)&State::getFullProfile)
			.def("getFullProfile", (Profile<bool> (State::*)(const StateVarKey<bool>&) const)&State::getFullProfile)
			.def("getFullProfile", (Profile<Matrix> (State::*)(const StateVarKey<Matrix>&) const)&State::getFullProfile)
			.def("getFullProfile", (Profile<Quat> (State::*)(const StateVarKey<Quat>&) const)&State::getFullProfile)

			.def("setProfile", (void (State::*)(const StateVarKey<int>&, const Profile<int>&))&State::setProfile)
			.def("setProfile", (void (State::*)(const StateVarKey<double>&, const Profile<double>&))&State::setProfile)
			.def("setProfile", (void (State::*)(const StateVarKey<float>&, const Profile<float>&))&State::setProfile)
			.def("setProfile", (void (State::*)(const StateVarKey<bool>&, const Profile<bool>&))&State::setProfile)
			.def("setProfile", (void (State::*)(const StateVarKey<Matrix>&, const Profile<Matrix>&))&State::setProfile)
			.def("setProfile", (void (State::*)(const StateVarKey<Quat>&, const Profile<Quat>&))&State::setProfile)
			
			.def("addValue", (void (State::*)(const StateVarKey<int>&, const pair<double, int>&))&State::addValue)
			.def("addValue", (void (State::*)(const StateVarKey<double>&, const pair<double, double>&))&State::addValue)
			.def("addValue", (void (State::*)(const StateVarKey<float>&, const pair<double, float>&))&State::addValue)
			.def("addValue", (void (State::*)(const StateVarKey<bool>&, const pair<double, bool>&))&State::addValue)
			.def("addValue", (void (State::*)(const StateVarKey<Matrix>&, const pair<double, Matrix>&))&State::addValue)
			.def("addValue", (void (State::*)(const StateVarKey<Quat>&, const pair<double, Quat>&))&State::addValue)
			
			.def("addValue", (void (State::*)(const StateVarKey<int>&, const Profile<int>&))&State::addValue)
			.def("addValue", (void (State::*)(const StateVarKey<double>&, const Profile<double>&))&State::addValue)
			.def("addValue", (void (State::*)(const StateVarKey<float>&, const Profile<float>&))&State::addValue)
			.def("addValue", (void (State::*)(const StateVarKey<bool>&, const Profile<bool>&))&State::addValue)
			.def("addValue", (void (State::*)(const StateVarKey<Matrix>&, const Profile<Matrix>&))&State::addValue)
			.def("addValue", (void (State::*)(const StateVarKey<Quat>&, const Profile<Quat>&))&State::addValue)
	];
}

void register_Matrix(lua_State* L) {
	module(L) [
		class_<Matrix>("Matrix")
			.def(constructor<>())
			.def(constructor<const unsigned>()) // nxn, all elements 0
			.def(constructor<const unsigned, const unsigned>()) // nxm, all elements 0
			.def(constructor<const unsigned, const unsigned, const double>()) // nxm, all elements same
			.def(constructor<const Matrix&>())
			.def(constructor<const Matrix&, unsigned, unsigned>())
			.def(constructor<const Matrix&, unsigned, unsigned, unsigned, unsigned>())
			.def("getValue", &Matrix::getValue)
			.def("setValue", &Matrix::setValue)
			.def("getNumRows", &Matrix::getNumRows)
			.def("getNumCols", &Matrix::getNumCols)
			.def("setNumRows", &Matrix::setNumRows)
			.def("setNumCols", &Matrix::setNumCols)
			.def("getNumElems", &Matrix::getNumElems)
			.def("isSquare", &Matrix::isSquare)
			.def("getSize", &Matrix::getSize, out_value(_2) + out_value(_3))
			.def("getColumn", &Matrix::getColumn)
			.def("setColumn", &Matrix::setColumn)
			.def("getRow", &Matrix::getRow)
			.def("setRow", &Matrix::setRow)
			.def("getSubMatrix", &Matrix::getSubMatrix)
			.def("setSubMatrix", &Matrix::setSubMatrix)

			.def(self(unsigned()))
			.def(self(unsigned(), unsigned()))

			.def(self(other<matrixIndex>()))
			.def(self(other<matrixIndex>(), other<matrixIndex>()))

			.def(tostring(self))

			.def(const_self + other<const Matrix&>())
			.def(const_self + other<const double&>())
			.def(other<const double&>() + const_self)

			.def(const_self - other<const Matrix&>())
			.def(const_self - other<const double&>())
			.def(other<const double&>() - const_self)

			.def(const_self * other<const Matrix&>())
			.def(const_self * other<const double&>())
			.def(other<const double&>() * const_self)

			.def(const_self / other<const Matrix&>())
			.def(const_self / other<const double&>())

			.def(const_self == other<const Matrix&>()),

			def("trace", &trace),
			def("transpose", &transpose),
			def("meye", &meye),
			def("vertcat", &vertcat),
			def("vertcat2", &vertcat2),
			def("horzcat", &horzcat),
			def("horzcat2", &horzcat2),
			def("norm", &norm),
			def("mmax", (Matrix (*)(const Matrix&))&mmax),
			def("mmax", (Matrix (*)(const Matrix&, const Matrix&))&mmax),
			def("mmax", (Matrix (*)(const Matrix&, const double))&mmax),
			def("mmax", (Matrix (*)(const double, const Matrix&))&mmax),
			def("mmin", (Matrix (*)(const Matrix&))&mmin),
			def("mmin", (Matrix (*)(const Matrix&, const Matrix&))&mmin),
			def("mmin", (Matrix (*)(const Matrix&, const double))&mmin),
			def("mmin", (Matrix (*)(const double, const Matrix&))&mmin),
			def("mabs", &mabs),
			def("inv", &inv),
			def("atom", &atom)
	];
}

void register_matrixIndex(lua_State* L) {
	module(L) [
		class_<matrixIndex>("matrixIndex")
			.def(constructor<unsigned>())
			.def(constructor<unsigned, unsigned>())
			.def("STARTIDX", &matrixIndex::STARTIDX)
			.def("ENDIDX", &matrixIndex::ENDIDX),
		def("colon", (matrixIndex(*)(const unsigned)) &colon),
		def("colon", (matrixIndex(*)(const unsigned, const unsigned)) &colon)
	];
}

void register_Quat(lua_State* L) {
	module(L) [
		class_<Quat>("Quat")
			.def(constructor<>())
			.def(constructor<const double>())
			.def(constructor<const double, const double, const double>())
			.def(constructor<const double, const double, const double, const double>())
			.def(constructor<const Matrix&, const double>())
			.def(constructor<const Matrix&, const Matrix&>())
			.def("real", &Quat::real)
			.def("comp_q0", &Quat::comp_q0)
			.def("comp_q1", &Quat::comp_q1)
			.def("comp_q2", &Quat::comp_q2)
			.def("comp_q3", &Quat::comp_q3)
			.def("print", &Quat::print)
			.def("set_q0", &Quat::set_q0)
			.def("set_q1", &Quat::set_q1)
			.def("set_q2", &Quat::set_q2)
			.def("set_q3", &Quat::set_q3)
			.def("conjugate", &Quat::conjugate)
			.def("norm", &Quat::norm)
			.def("normalize", &Quat::normalize)

			.def(const_self + other<const Quat&>())
			.def(const_self + other<const double&>())
			.def(other<const double&>() + const_self)
			.def(const_self - other<const Quat&>())
			.def(const_self - other<const double&>())
			.def(other<const double&>() - const_self)
			.def(other<const double&>() * const_self)
			.def(const_self * other<const double&>())
			.def(const_self * other<const Quat&>())
			.def(const_self == other<const Quat&>())

			.def(tostring(self))
	];
}

void register_simParams(lua_State* L) {
	module(L) [
		def("SIMSTART_JD", &simParams::SIMSTART_JD),
		def("SIMSTART_SECONDS", &simParams::SIMSTART_SECONDS),
		def("SIMEND_SECONDS", &simParams::SIMEND_SECONDS)
	];
}

void register_allPairs(lua_State* L) {
	class_< pair<double, double> > dPair("dPair");
	register_pair<double>(dPair);
	module(L) [ dPair ];

	class_< pair<double, float> > fPair("fPair");
	register_pair<float>(fPair);
	module(L) [ fPair ];

	class_< pair<double, int> > iPair("iPair");
	register_pair<int>(iPair);
	module(L) [ iPair ];

	class_< pair<double, bool> > bPair("bPair");
	register_pair<bool>(bPair);
	module(L) [ bPair ];

	class_< pair<double, Matrix> > mPair("mPair");
	register_pair<Matrix>(mPair);
	module(L) [ mPair ];

	class_< pair<double, Quat> > qPair("qPair");
	register_pair<Quat>(qPair);
	module(L) [ qPair ];
}

void register_allProfiles(lua_State* L) {
	class_< Profile<double> > dProf("dProfile");
	register_Profile<double>(dProf);
	register_Profile_additional<double>(dProf);
	module(L) [ dProf ];

	class_< Profile<float> > fProf("fProfile");
	register_Profile<float>(fProf);
	register_Profile_additional<float>(fProf);
	module(L) [ fProf ];

	class_< Profile<int> > iProf("iProfile");
	register_Profile<int>(iProf);
	register_Profile_additional<int>(iProf);
	module(L) [ iProf ];

	class_< Profile<bool> > bProf("bProfile");
	register_Profile<bool>(bProf);
	register_Profile_additional<bool>(bProf);
	module(L) [ bProf ];

	class_< Profile<Matrix> > mProf("mProfile");
	register_Profile<Matrix>(mProf);
	module(L) [ mProf ];

	class_< Profile<Quat> > qProf("qProfile");
	register_Profile<Quat>(qProf);
	module(L) [ qProf ];
}

void register_allStateVarKeys(lua_State* L) {
	class_< StateVarKey<double> > dSVK("dStateVarKey");
	register_StateVarKey<double>(dSVK);
	module(L) [ dSVK ];

	class_< StateVarKey<float> > fSVK("fStateVarKey");
	register_StateVarKey<float>(fSVK);
	module(L) [ fSVK ];

	class_< StateVarKey<int> > iSVK("iStateVarKey");
	register_StateVarKey<int>(iSVK);
	module(L) [ iSVK ];

	class_< StateVarKey<bool> > bSVK("bStateVarKey");
	register_StateVarKey<bool>(bSVK);
	module(L) [ bSVK ];

	class_< StateVarKey<Matrix> > mSVK("mStateVarKey");
	register_StateVarKey<Matrix>(mSVK);
	module(L) [ mSVK ];

	class_< StateVarKey<Quat> > qSVK("qStateVarKey");
	register_StateVarKey<Quat>(qSVK);
	module(L) [ qSVK ];
}

void register_Subsystem(lua_State* L) {
	module(L) [
		class_<Subsystem>("Subsystem")
			.def("getName", &Subsystem::getName)
			.def("addKey", (void (Subsystem::*)(const StateVarKey<double>))&Subsystem::addKey)
			.def("addKey", (void (Subsystem::*)(const StateVarKey<float>))&Subsystem::addKey)
			.def("addKey", (void (Subsystem::*)(const StateVarKey<int>))&Subsystem::addKey)
			.def("addKey", (void (Subsystem::*)(const StateVarKey<bool>))&Subsystem::addKey)
			.def("addKey", (void (Subsystem::*)(const StateVarKey<Matrix>))&Subsystem::addKey)
			.def("addKey", (void (Subsystem::*)(const StateVarKey<Quat>))&Subsystem::addKey)
	];
}

void register_SubsystemParameters(lua_State* L) {
	module(L) [
		class_<SubsystemParameters>("SubsystemParameters")
			.def(constructor<>())
			.def("addDoubleParameter", &SubsystemParameters::addDoubleParameter)
			.def("addFloatParameter", &SubsystemParameters::addFloatParameter)
			.def("addIntParameter", &SubsystemParameters::addIntParameter)
			.def("addBoolParameter", &SubsystemParameters::addBoolParameter)
			.def("addMatrixParameter", &SubsystemParameters::addMatrixParameter)
			.def("addQuatParameter", &SubsystemParameters::addQuatParameter)
			.def("getDoubleParameter", &SubsystemParameters::getDoubleParameter)
			.def("getFloatParameter", &SubsystemParameters::getFloatParameter)
			.def("getIntParameter", &SubsystemParameters::getIntParameter)
			.def("getBoolParameter", &SubsystemParameters::getBoolParameter)
			.def("getMatrixParameter", &SubsystemParameters::getMatrixParameter)
			.def("getQuatParameter", &SubsystemParameters::getQuatParameter)
	];
}

void register_NodeDependencies(lua_State* L) {
	// Only the methods to call dependencies are binded
	module(L) [
		class_<NodeDependencies>("NodeDependencies")
			.def(constructor<>())
			.def("callDoubleDependency", &NodeDependencies::callDoubleDependency)
			.def("callFloatDependency", &NodeDependencies::callFloatDependency)
			.def("callIntDependency", &NodeDependencies::callIntDependency)
			.def("callBoolDependency", &NodeDependencies::callBoolDependency)
			.def("callMatrixDependency", &NodeDependencies::callMatrixDependency)
			.def("callQuatDependency", &NodeDependencies::callQuatDependency)
			.def("enableScriptingSupport", &NodeDependencies::enableScriptingSupport)
			.def("disableScriptingSupport", &NodeDependencies::disableScriptingSupport)
	];
}

void register_Environment(lua_State* L) {
	module(L) [
		class_<Environment>("Environment")
			.def(constructor<>())
			.def(constructor<Sun*>())
			.def("getSun", &Environment::getSun)
	];
}

void register_Sun(lua_State* L) {
	module(L) [
		class_<Sun>("Sun")
			.def(constructor<>())
			.def(constructor<bool>())
			.def("castShadowOnPos", &Sun::castShadowOnPos)
			.def("getSolarConstant", &Sun::getSolarConstant)
			.def("getEarSunVec", &Sun::getEarSunVec)
			.enum_("shadow_state") [
				value("NO_SHADOW", 0),
				value("UMBRA", 1),
				value("PENUMBRA", 2)
			]	
	];
}

void register_geomUtil(lua_State* L) {
	module(L) [
		def("hasLOS", &hasLOS),
		def("LLA2ECI", &LLA2ECI),
		def("ECI2LLA", &ECI2LLA),
		def("HMS2UT", &HMS2UT),
		def("YMDUT2JD", &YMDUT2JD),
		def("CT2LST", &CT2LST)
	];
}

void register_Position(lua_State* L) {
	module(L) [
		class_<Position>("Position")
			.def(constructor<XMLNode&>())
			.def(constructor<const string&, const double&, const double&, const double&>())
			.def(constructor<const string&, const Matrix&>())
			.def(constructor<const string&, EOMS*, const double&, const double&, const double&, const double&, const double&, const double&>())
			.def(constructor<const string&, EOMS*, const Matrix&>())
			.def("setPosDataTimeStep", &Position::setPosDataTimeStep)
			.def("getType", &Position::getType)
			.def("getEOMS", &Position::getEOMS)
			.def("getIC", &Position::getIC)
			.def("getStateECI", &Position::getStateECI)
			.def("getStateAtTime", &Position::getStateAtTime)
			.def("getPosECI", &Position::getPosECI)
			.def("getVelECI", &Position::getVelECI)
			.def("getPosLLA", &Position::getPosLLA)
			.def("hasLOSTo", (bool (Position::*)(Position*, double))&Position::hasLOSTo)
			.def("hasLOSTo", (bool (Position::*)(Matrix, double))&Position::hasLOSTo)
	];
}

void register_EOMS(lua_State* L) {
module(L) [
		class_<EOMS>("EOMS")
			.def(self(other<const double&>(), other<const Matrix&>()))
	];
}

void register_Task(lua_State* L) {
	module(L) [
		class_<Task>("Task")
			.def(constructor<>())
			.def(constructor<const string&, Target*, size_t>())
			.def("getType", &Task::getType)
			.def("getTarget", &Task::getTarget)
			.def("getMaxTimesPerformable", &Task::getMaxTimesPerformable)
	];
}

void register_Target(lua_State* L) {
	module(L) [
		class_<Target>("Target")
			.def(constructor<XMLNode>())
			.def(constructor<const string&, Position*>())
			.def(constructor<const string&, Position*, int>())
			.def("getName", &Target::getName)
			.def("getType", &Target::getType)
			.def("getValue", &Target::getValue)
			.def("getPos", &Target::getPos)
			.def(const_self == other<const Target&>())
	];
}

void register_umath(lua_State* L) {
	module(L) [
		def("cross", (Matrix (*)(const Matrix&, const Matrix&))&cross),
		def("dot", (double (*)(const Matrix&, const Matrix&))&dot)
	];
}

void register_Event(lua_State* L) {
	module(L) [
		class_<Event> ("Event")
			.def("getTask", &Event::getTask)
			.def("getState", &Event::getState)
	];
}

void register_NetworkDataClient(lua_State* L) {
	module(L) [
		class_<NetworkDataClient> ("NetworkDataClient")
			.def("isOpen", &NetworkDataClient::isOpen)
			.def("requestData_Int", &NetworkDataClient::requestData_Int)
			.def("requestData_IntProfile", &NetworkDataClient::requestData_IntProfile)
			.def("requestData_Double", &NetworkDataClient::requestData_Double)
			.def("requestData_DoubleProfile", &NetworkDataClient::requestData_DoubleProfile)
			.def("requestData_Float", &NetworkDataClient::requestData_Float)
			.def("requestData_FloatProfile", &NetworkDataClient::requestData_FloatProfile)
			.def("requestData_Bool", &NetworkDataClient::requestData_Bool)
			.def("requestData_BoolProfile", &NetworkDataClient::requestData_BoolProfile)
			.def("requestData_Matrix", &NetworkDataClient::requestData_Matrix)
			.def("requestData_MatrixProfile", &NetworkDataClient::requestData_MatrixProfile)
			.def("requestData_Quat", &NetworkDataClient::requestData_Quat)
			.def("requestData_QuatProfile", &NetworkDataClient::requestData_QuatProfile), 
		def("Connect", (bool (*)(void))&NetworkDataClient::Connect),
		def("Connect", (bool (*)(string, int))&NetworkDataClient::Connect),
		def("Disconnect", &NetworkDataClient::Disconnect),
		def("getNetworkDataClient", &NetworkDataClient::Instance),
		def("setHost", &NetworkDataClient::setHost),
		def("setPort", &NetworkDataClient::setPort)
	];

	module(L) [
		class_<HandlerType> ("HandlerType")
			.enum_("HandlerType") [
				value("ANY_DATA_HANDLER", 0),
				value("FLAT_FILE_DATA_HANDLER", 1),
				value("XML_FILE_DATA_HANDLER", 2)
			]
	];
}

/*void register_SystemScheduleWrapper(lua_State* L) {
	module(L) [
		class_<SystemScheduleWrapper> ("SystemScheduleWrapper")
			.def(constructor<systemSchedule*> ())
			.def("GetNumberAssets", &SystemScheduleWrapper::GetNumberAssets)
			.def("GetNumberEvents", &SystemScheduleWrapper::GetNumberEvents)
			.def("GetEvent", &SystemScheduleWrapper::GetEvent)
	];
}*/


bool loadLuaFunctionsFromFile(lua_State* L, string filename) {
	bool success = true;
	stringstream temp;
	ifstream fin;
	fin.open(filename.c_str(), ios_base::in);
	if(fin.is_open()) {
		while(!fin.eof()) {
			string line;
			getline(fin,line);
			temp << line << endl;
		}
	}
	else
		success = false;
	fin.close();
	// luaL_dostring returns 0 in case of no errors, 1 if errors
	if(luaL_dostring(L, temp.str().c_str()) != 0) {
		// An error has occured, print the error message
		cout << lua_tostring(L, lua_gettop(L)) << endl;
		// Pop the error message off the stack
		lua_pop(L, 1);
		success = false;
	}
	return success;
}

bool loadLuaFunctionsFromString(lua_State* L, string fcns) {
	bool success = true;
	// luaL_dostring returns 0 in case of no errors, 1 if errors
	if(luaL_dostring(L, fcns.c_str()) != 0) {
		// An error has occured, print the error message
		cout << lua_tostring(L, lua_gettop(L)) << endl;
		// Pop the error message off the stack
		lua_pop(L, 1);
		success = false;
	}
	return success;
}

}}
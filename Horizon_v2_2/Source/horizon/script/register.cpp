#include "register.h"

namespace horizon {
	namespace script {

void register_dependencyLuaState(lua_State* L) {
	register_simParams(L);
	register_allPairs(L);
	register_allProfiles(L);
	register_State(L);
	register_Matrix(L);
	register_Quat(L);
	register_matrixIndex(L);
	register_allStateVarKeys(L);

	module(L) [
		def("getState", &Dependencies::getState)
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
	if(luaL_dostring(L, temp.str().c_str()) != 0)
		success = false;
	return success;
}

bool loadLuaFunctionsFromString(lua_State* L, string fcns) {
	bool success = true;
	// luaL_dostring returns 0 in case of no errors, 1 if errors
	if(luaL_dostring(L, fcns.c_str()) != 0)
		success = false;
	return success;
}

}}
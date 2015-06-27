#include <string>

#include "horizon/script/luaDefs.h"
#include "horizon/script/ScriptingUtil.h"
using namespace luabind;
using namespace horizon::script;

using namespace std;

namespace lua {
	lua_State* L;
}

int n_getFullProfile = 0;

State* createTestState(double t);

double ProfileOperations1(State* state);
double ProfileOperations2(State* state);
double ProfileOperations3(State* state);

double intDProf(Profile<double> prof, double start, double end);

double runLuaFunction(lua_State* L, string luaFunc, State* testState);

int main( int argc, char *argv[ ], char *envp[ ] ) {
	// Args
	string logFileName = argv[1];
	double endT = atof(argv[2]);
	int testNum = atoi(argv[3]);

	string luaFunc = "ProfileOperations3";

	// Lua
	lua::L = lua_open();
	luaL_openlibs(lua::L);
	open(lua::L);
	register_LUAState(lua::L);
	module(lua::L) [
		def("intDProf", (double (*)(Profile<double> prof, double start, double end))&intDProf)
	];
	set_pcall_callback(&luabind::luaErrFcn);

	// Load lua files
	loadLuaFunctionsFromFile(lua::L, "io/scripts/Profile Test/Profile Test.txt");

	State* testState = createTestState(endT);
	DWORD start, diff;
	double res;
	switch(testNum) {
		case 0: // compiled
			start = GetTickCount();
			res = ProfileOperations3(testState);
			diff = GetTickCount() - start;
			break;
		case 1: // scripted
			start = GetTickCount();
			res = runLuaFunction(lua::L, luaFunc, testState);
			diff = GetTickCount() - start;
			break;
		default:
			res = 0;
			diff = 0;
			break;
	}


	cout << "res: " << res << endl;
	cout << "diff: " << diff << endl;

	// Log results
	ofstream fout;
	fout.open(logFileName.c_str(), ios::app);
	if(fout.is_open()) {
		// Time, test type #, runtime
		fout << endT << ", " << testNum << ", ";
		fout << diff << endl;
	}
	fout.close();

	//getchar();
	return 0;

}

State* createTestState(double t) {
	
	Profile<double> dP1;
	Profile<double> dP2;
	Profile<double> dP3;
	double dt = 1;
	double phaseMult = 0.1;
	double amplitude = 10;
	double currT = 0.0;
	int maxDP3 = 10;
	while(currT < t) {
		dP1[currT] = sin(phaseMult*currT);
		dP2[currT] = cos(phaseMult*currT);
		dP3[currT] = (int)currT % maxDP3;
		currT += dt;
	}
	// Create state
	State* state = new State();
	state->setProfile(StateVarKey<double>("dP1"), dP1);
	state->setProfile(StateVarKey<double>("dP2"), dP2);
	state->setProfile(StateVarKey<double>("dP3"), dP3);

	return state;
}

double runLuaFunction(lua_State* L, string luaFunc, State* testState) {
	if(functionExists(L, luaFunc)) {
		return call_function<double>(L, luaFunc.c_str(), testState);
	} else {
		return 0.0;
	}
}

double ProfileOperations1(State* state) {
	// Get profiles
	Profile<double> dP1 = state->getFullProfile(StateVarKey<double>("dP1"));
	Profile<double> dP2 = state->getFullProfile(StateVarKey<double>("dP2"));
	Profile<double> dP3 = state->getFullProfile(StateVarKey<double>("dP3"));
	// Add
	Profile<double> dP = 0.5*dP1 + 1.2*dP2 + 0.3*dP3;
	// Multiply
	dP = dP*0.01;
	// Subtract
	dP = dP - 0.8*dP1;
	// Integrate
	//double res = dP.integrate(0, dP.getLastPair().first);
	//return res;
	//return intDProf(dP, 0, dP.getLastPair().first);
	return 1;
}

double ProfileOperations2(State* state) {
	// Get profiles
	Profile<double> dP1 = state->getFullProfile(StateVarKey<double>("dP1"));
	double res = dP1.integrate(0, dP1.getLastPair().first);
	return res;
}

double ProfileOperations3(State* state) {
	// Get profiles
	Profile<double> dP1 = state->getFullProfile(StateVarKey<double>("dP1"));
	Profile<double> dP2 = state->getFullProfile(StateVarKey<double>("dP2"));
	Profile<double> dP3 = state->getFullProfile(StateVarKey<double>("dP3"));
	// Add
	Profile<double> dP = 0.5*dP1;
	dP += 1.2*dP2;
	dP += 0.3*dP3;
	// Multiply
	dP *= 0.01;
	// Subtract
	dP = dP - 0.8*dP1;
	return dP.integrate(0, dP.getLastPair().first);
	//return 1;
}

double intDProf(Profile<double> prof, double start, double end) {
	return prof.integrate(start, end);
}
#include "horizon/script/luaDefs.h"
using namespace luabind;

namespace lua {
	lua_State* L;
}

int n_getFullProfile = 0;

State* createTestState(double t);

double ProfileOperations1(State* state);
double ProfileOperations2(State* state);
double ProfileOperations3(State* state);

double intDProf(Profile<double>& prof, double start, double end);

int main( int argc, char *argv[ ], char *envp[ ] ) {
	// Args
	//string logFileName = argv[1];
	//double endT = atof(argv[2]);
	//int test = atof(argv[3]);
	double endT = 5000;
	string luaFunc = "ProfileOperations3";

	// Lua
	lua::L = lua_open();
	luaL_openlibs(lua::L);
	open(lua::L);
	register_LUAState(lua::L);
	module(lua::L) [
		def("intDProf", (double (*)(Profile<double>& prof, double start, double end))&intDProf)
	];
	set_pcall_callback(&luabind::luaErrFcn);

	// Load lua files
	loadLuaFunctionsFromFile(lua::L, "io/scripts/Profile Test/Profile Test.txt");

	// Get functions
	/*double (*cFunc)(State*);
	string luaFunc;
	switch(test) {
		case 1:
			cFunc = &ProfileOperations1;
			luaFunc = "ProfileOperations1";
			break;
		case 2:
			cFunc = &ProfileOperations2;
			luaFunc = "ProfileOperations2";
			break;
		default:
			break;
	}*/

	__int64 freq;
	__int64 start1, end1, diff1;
	__int64 start2, end2, diff2;
	// start
QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
/*	

	// code to measure
	QueryPerformanceCounter((LARGE_INTEGER*)&start1);
	Sleep(1234);

	// end
	QueryPerformanceCounter((LARGE_INTEGER*)&end1);
	diff1 = ((end1 - start1) * 1000) / freq;

	unsigned int milliseconds = (unsigned int)(diff1 & 0xffffffff);
	printf("It took %u ms\n", milliseconds);
*/

	// Do test
	State* testState = createTestState(endT);


	__int64 start, end, diff;
	QueryPerformanceCounter((LARGE_INTEGER*)&start);
	double res = ProfileOperations3(testState);
	//double res = call_function<double>(lua::L, luaFunc.c_str(), testState);
	QueryPerformanceCounter((LARGE_INTEGER*)&end);
	diff = ((end - start) * 1000) / freq;

	cout << "---" << diff << "---" << endl;

	getchar();
	return 0;
	



	cout << "********" << endl;

	//DWORD start1 = GetTickCount();
	QueryPerformanceCounter((LARGE_INTEGER*)&start1);
	double res1 = ProfileOperations3(testState);
	QueryPerformanceCounter((LARGE_INTEGER*)&end1);
	diff1 = ((end1 - start1) * 1000) / freq;
	//DWORD diff1 = GetTickCount() - start1;

	cout << "********" << endl;
	

	//DWORD start2 = GetTickCount();
	QueryPerformanceCounter((LARGE_INTEGER*)&start2);
	double res2 = call_function<double>(lua::L, luaFunc.c_str(), testState);
	QueryPerformanceCounter((LARGE_INTEGER*)&end2);
	diff2 = ((end2 - start2) * 1000) / freq;
	//DWORD diff2 = GetTickCount() - start2;

	unsigned int milliseconds1 = (unsigned int)(diff1 & 0xffffffff);
	unsigned int milliseconds2 = (unsigned int)(diff2 & 0xffffffff);
	cout << "check: res1=" << res1 << " , res2=" << res2 << endl; 
	cout << diff1 << ", " << diff2 << endl;

	// Log results
	/*ofstream fout;
	fout.open(logFileName.c_str(), ios::app);
	if(fout.is_open()) {
		// Time, test type #, c++ time, lua time
		fout << endT << ", " << test << ", ";
		fout << diff1 << ", " << diff2 << ", " << endl;
	}
	fout.close();*/

	getchar();
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
	dP = dP + 1.2*dP2;
	dP = dP + 0.3*dP3;
	// Multiply
	dP = dP*0.01;
	// Subtract
	dP = dP - 0.8*dP1;
	return 1;
}

double intDProf(Profile<double>& prof, double start, double end) {
	return prof.integrate(start, end);
}
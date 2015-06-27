#include "horizon/script/luaDefs.h"
using namespace luabind;

namespace lua {
	lua_State* L;
}

double fibonacciRecursive(int n);
double fibonacciLoop(int n);
double factorialRecursive(int n);
double factorialLoop(int n);

int i_ = 0;

int main( int argc, char *argv[ ], char *envp[ ] ) {
	// Args
	int n = atoi(argv[1]);
	int testNum = atoi(argv[2]); // 1 -> 4
	int testType = atoi(argv[3]); // 1 for scripted
	int nLoop = atoi(argv[4]);

	// Lua
	lua::L = lua_open();
	luaL_openlibs(lua::L);
	open(lua::L);
	register_LUAState(lua::L);
	set_pcall_callback(&luabind::luaErrFcn);

	// Load lua files
	loadLuaFunctionsFromFile(lua::L, "io/scripts/Misc Algo Test/fibonacci.txt");
	loadLuaFunctionsFromFile(lua::L, "io/scripts/Misc Algo Test/factorial.txt");

	// Cases:
	// 1 - fibonacci recursive
	// 2 - fibonacci loop
	// 3 - factorial recursive
	// 4 - factorial loop
	DWORD start, diff;
	double res;
	string luaFunc;
	switch(testNum) {
		case 1: // fibonacci recursive
			if(testType) {
				// scripted
				luaFunc = "fibonacciRecursive";
				start = GetTickCount();
				for(int i=1; i<=nLoop; ++i) {
					res = call_function<double>(lua::L, luaFunc.c_str(), n);
				}
				diff = GetTickCount() - start;
			} else {
				// compiled
				start = GetTickCount();
				for(int i=1; i<=nLoop; ++i) {
					res = fibonacciRecursive(n);
				}
				diff = GetTickCount() - start;
			}
			break;
		case 2: // fibonacci loop
			if(testType) {
				// scripted
				luaFunc = "fibonacciLoop";
				start = GetTickCount();
				for(int i=1; i<=nLoop; ++i)
					res = call_function<double>(lua::L, luaFunc.c_str(), n);
				diff = GetTickCount() - start;
			} else {
				// compiled
				start = GetTickCount();
				for(int i=1; i<=nLoop; ++i) {
					res = fibonacciLoop(n);
				}
				diff = GetTickCount() - start;
			}
			break;
		case 3: // factorial recursive
			if(testType) {
				// scripted
				luaFunc = "factorialRecursive";
				start = GetTickCount();
				for(int i=1; i<=nLoop; ++i) {
					res = call_function<double>(lua::L, luaFunc.c_str(), n);
				}
				diff = GetTickCount() - start;
			} else {
				// compiled
				start = GetTickCount();
				for(int i=1; i<=nLoop; ++i) {
					res = factorialRecursive(n);
				}
				diff = GetTickCount() - start;
			}
			break;
		case 4: // factorial loop
			if(testType) {
				// scripted
				luaFunc = "factorialLoop";
				start = GetTickCount();
				for(int i=1; i<=nLoop; ++i) {
					res = call_function<double>(lua::L, luaFunc.c_str(), n);
				}
				diff = GetTickCount() - start;
			} else {
				// compiled
				start = GetTickCount();
				for(int i=1; i<=nLoop; ++i) {
					res = factorialLoop(n);
				}
				diff = GetTickCount() - start;
			}
			break;
		default:
			res = 0;
			diff = 0;
			break;
	}

	cout << "res: " << res << endl;
	cout << "diff: " << diff << endl;

	return diff;

}

double fibonacciRecursive(int n) {
	extern int i_;
	if(n==0) {
		i_++;
		return 0.0;
	} else if(n==1) {
		return 1.0;
	} else {
		return (double)fibonacciRecursive(n-1) + (double)fibonacciRecursive(n-2);
	}
}

double fibonacciLoop(int n) {
	double *arr = new double[n+1];
	arr[0] = 0;
	arr[1] = 1;
	for(int i=2; i<=n; ++i) {
		arr[i] = arr[i-1] + arr[i-2];
	}
	double res = arr[n];
	delete[] arr;
	return res;
}

double factorialRecursive(int n) {
	extern int i_;
	if(n==0) {
		i_++;
		return 1;
	} else {
		return n*factorialRecursive(n-1);
	}
}

double factorialLoop(int n) {
	double *arr = new double[n+1];
	arr[0] = 1;
	for(int i=1; i<=n; ++i) {
		arr[i] = i*arr[i-1];
	}
	double res = arr[n];
	delete[] arr;
	return res;
}
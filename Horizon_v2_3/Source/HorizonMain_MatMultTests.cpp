#include <iostream> 
#include <fstream>
#include <deque>
using namespace std; 

#include "horizon/script/luaDefs.h"
using namespace luabind;

namespace lua {
	lua_State* L;
}

void registerMathH(lua_State* L);
int add100();

Matrix matMult1(Matrix m1, Matrix m2);
Matrix matMult2(Matrix m1, Matrix m2);
Matrix matMult3(Matrix m1, Matrix m2);

Matrix genMatrix(int n);

int fcnCount = 0;

int main( int argc, char *argv[ ], char *envp[ ] ) {
	// Args
	int n = atoi(argv[1]); // size of matrix
	int test = atof(argv[2]); // 1,2, or 3
	int scripted = atoi(argv[3]); // 0 for C++, 1 for Lua

	lua::L = lua_open();
	luaL_openlibs(lua::L);
	open(lua::L);
	register_LUAState(lua::L);
	set_pcall_callback(&luabind::luaErrFcn);

	// Load lua files
	loadLuaFunctionsFromFile(lua::L, "io/scripts/MatMult Test/MatMult.txt");

	Matrix mat = genMatrix(n);

	DWORD startTime, stopTime, runTime;
	Matrix res;
	switch(test) {
		case 1:
			if(scripted) {
				startTime = GetTickCount();
				res = call_function<Matrix> (lua::L, "matMult1", mat, mat);
				stopTime = GetTickCount();
			} else {
				startTime = GetTickCount();
				res = matMult1(mat, mat);
				stopTime = GetTickCount();
			}
			runTime = stopTime - startTime;
			break;
		case 2:
			if(scripted) {
				startTime = GetTickCount();
				res = call_function<Matrix> (lua::L, "matMult2", mat, mat);
				stopTime = GetTickCount();
			} else {
				startTime = GetTickCount();
				res = matMult2(mat, mat);
				stopTime = GetTickCount();
			}
			runTime = stopTime - startTime;
			break;
		case 3:
			if(scripted) {
				startTime = GetTickCount();
				res = call_function<Matrix> (lua::L, "matMult3", mat, mat);
				stopTime = GetTickCount();
			} else {
				startTime = GetTickCount();
				res = matMult3(mat, mat);
				stopTime = GetTickCount();
			}
			runTime = stopTime - startTime;
			break;
		default:
			runTime = 0;
			break;
	}

	cout << runTime << endl;

	//getchar();
	return runTime;
}

Matrix matMult1(Matrix m1, Matrix m2) {
	return m1*m2;
}

Matrix matMult2(Matrix m1, Matrix m2) {
	int m = m1.getNumRows();
	int n = m2.getNumCols();
	Matrix res(m, n, 0.0);

	for(int r=1; r<= m; ++r) {
		Matrix currRowM1 = m1.getRow(r);
		for(int c=1; c<= n; ++c) {
			res.setValue(r, c, dot(currRowM1, m2.getColumn(c)));
		}
	}
	return res;
}

Matrix matMult3(Matrix m1, Matrix m2) {
	int m = m1.getNumRows();
	int p = m1.getNumCols();
	int n = m2.getNumCols();
	Matrix res(m, n, 0.0);

	for(int r=1; r<= m; ++r) {
		for(int c=1; c<= n; ++c) {
			double val = 0;
			for(int i=1; i<=p; ++i) {
				val += m1.getValue(r, i)*m2.getValue(i, c);
			}
			res.setValue(r, c, val);
		}
	}
	return res;
}

Matrix genMatrix(int n) {
	deque<double> inData;

	for(int i=1; i<=n*n; ++i)
		inData.push_back((double)i);

	Matrix res(n);
	res.setData(inData);

	return res;
}
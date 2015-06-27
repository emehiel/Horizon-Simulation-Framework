#include <iostream> 
#include <fstream> 
using namespace std; 

#include "horizon/script/luaDefs.h"
using namespace luabind;

namespace lua {
	lua_State* L;
}

void registerMathH(lua_State* L);

int main( int argc, char *argv[ ], char *envp[ ] ) {
	// Args
	string logFileName = argv[1];
	double endT = atof(argv[2]);
	int test = atof(argv[3]);

	lua::L = lua_open();
	luaL_openlibs(lua::L);
	open(lua::L);
	register_LUAState(lua::L);
	registerMathH(lua::L);
	set_pcall_callback(&luabind::luaErrFcn);

	// Load lua files
	loadLuaFunctionsFromFile(lua::L, "io/scripts/EOMS test/orbital_EOMS.txt");
	loadLuaFunctionsFromFile(lua::L, "io/scripts/EOMS test/orbital_EOMS_J2.txt");
	loadLuaFunctionsFromFile(lua::L, "io/scripts/EOMS test/orbital_EOMS_J2_DRAG.txt");

	// Setup
	//double endT = 8*60*60;
	simParams::setSimEnd(endT);

	// Outputs
	DWORD runTime, runTimeLua;
	double diffPosNorm, diffVelNorm;

	// XML Nodes
	XMLNode posNode = XMLNode::createXMLTopNode("POSITION");
	XMLNode eomsNode = XMLNode::createXMLTopNode("EOMS");
	XMLNode intNode = XMLNode::createXMLTopNode("INTEGRATOR");
	XMLNode posNodeLua = XMLNode::createXMLTopNode("POSITION");
	XMLNode eomsNodeLua = XMLNode::createXMLTopNode("EOMS");
	XMLNode intNodeLua = XMLNode::createXMLTopNode("INTEGRATOR");

	switch(test) {
		case 1:
			// XML Nodes Normal
			eomsNode.addAttribute("EOMSType", "orbital_EOMS");
			eomsNodeLua.addAttribute("EOMSType", "scripted");
			eomsNodeLua.addAttribute("initFcn", "orbital_EOMS_init");
			eomsNodeLua.addAttribute("evalFcn", "orbital_EOMS_eval");
			break;
		case 2:
			// XML Nodes J2
			eomsNode.addAttribute("EOMSType", "orbital_EOMS_J2");
			eomsNodeLua.addAttribute("EOMSType", "scripted");
			eomsNodeLua.addAttribute("initFcn", "orbital_EOMS_J2_init");
			eomsNodeLua.addAttribute("evalFcn", "orbital_EOMS_J2_eval");
			break;
		case 3:
			// XML Nodes J2
			eomsNode.addAttribute("EOMSType", "orbital_EOMS_TEST");
			eomsNodeLua.addAttribute("EOMSType", "scripted");
			eomsNodeLua.addAttribute("initFcn", "orbital_EOMS_J2_DRAG_init");
			eomsNodeLua.addAttribute("evalFcn", "orbital_EOMS_J2_DRAG_eval");
			break;
		default:
			break;
	}

	// XML Attributes and Structure
	posNode.addAttribute("PositionType", "PredeterminedECI");
	posNode.addAttribute("ICs", "Matrix(6,1,{7378.137, 0.0, 0.0, 0.0, 5.1973, 5.1973})");
	intNode.addAttribute("rtol", "1e-6");
	intNode.addAttribute("atol", "1e-6");
	posNode.addChild(eomsNode);
	posNode.addChild(intNode);
	posNodeLua.addAttribute("PositionType", "PredeterminedECI");
	posNodeLua.addAttribute("ICs", "Matrix(6,1,{7378.137, 0.0, 0.0, 0.0, 5.1973, 5.1973})");
	intNodeLua.addAttribute("rtol", "1e-6");
	intNodeLua.addAttribute("atol", "1e-6");
	posNodeLua.addChild(eomsNodeLua);
	posNodeLua.addChild(intNodeLua);

	// Structure
	Position* position = new Position(posNode);
	Position* positionLua = new Position(posNodeLua);
	// C++
	DWORD startTime = GetTickCount();
	Matrix endState =  position->getStateECI(endT);
	DWORD stopTime = GetTickCount();
	runTime = stopTime - startTime;
	// Lua
	DWORD startTimeLua = GetTickCount();
	Matrix endStateLua =  positionLua->getStateECI(endT);
	DWORD stopTimeLua = GetTickCount();
	runTimeLua = stopTimeLua - startTimeLua;
	// Check
	Matrix diff = endState - endStateLua;
	Matrix diffPos = diff(colon(1,3));
	Matrix diffVel = diff(colon(4,6));
	diffPosNorm = norm(diffPos);
	diffVelNorm = norm(diffVel);

	cout << runTime << ", " << runTimeLua << " : " << diffPosNorm << ", " << diffVelNorm << endl;

	// Log results
	ofstream fout;
	fout.open(logFileName.c_str(), ios::app);
	if(fout.is_open()) {
		// Time, test type #, c++ time, lua time, pos err norm, vel error norm
		fout << endT << ", " << test << ", ";
		fout << runTime << ", " << runTimeLua << ", ";
		fout << diffPosNorm << ", " << diffVelNorm << endl;
	}
	fout.close();

	//getchar();
	return 0;
}

void registerMathH(lua_State* L) {
	module(L) [
		def("cPow", (double (*)(double, double))&pow)
	];
}
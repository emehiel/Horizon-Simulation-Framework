#include "ScriptedSubsystemTests.h"

namespace horizon {
	namespace test {

void testSubsystemParameters(lua_State* L) {
	SubsystemParameters* subParams = new SubsystemParameters();
	subParams->addDoubleParameter("double1", 1.00);
	subParams->addFloatParameter("float1", 1.00);
	subParams->addIntParameter("int1", 1);
	subParams->addBoolParameter("bool1", true);
	subParams->addMatrixParameter("matrix1", Matrix(2,2,1.00));
	subParams->addQuatParameter("quat1", Quat(1.00, 2.00, 3.00, 4.00));

	// double1 should hold 1.00
	double double1 = subParams->getDoubleParameter("double1");
	// double2 should hold 0
	double double2 = subParams->getDoubleParameter("double2");

	// float1 should hold 1.00
	float float1 = subParams->getFloatParameter("float1");
	// float2 should hold 0
	float float2 = subParams->getFloatParameter("float2");

	// int1 should hold 1.00
	int int1 = subParams->getIntParameter("int1");
	// int2 should hold 0
	int int2 = subParams->getIntParameter("int2");

	// bool1 should hold true
	bool bool1 = subParams->getBoolParameter("bool1");
	// bool2 should hold bool()
	bool bool2 = subParams->getBoolParameter("bool2");
	bool bool3 = bool();

	// matrix1 should hold Matrix(2,2,1.00)
	Matrix matrix1 = subParams->getMatrixParameter("matrix1");
	// matrix2 should hold Matrix()
	Matrix matrix2 = subParams->getMatrixParameter("matrix2");
	Matrix matrix3 = Matrix();

	// quat1 should hold Quat(1.00, 2.00, 3.00, 4.00)
	Quat quat1 = subParams->getQuatParameter("quat1");
	// quat2 should hold Quat()
	Quat quat2 = subParams->getQuatParameter("quat2");
	Quat quat3 = Quat();

	// REDO WITH LUA SCRIPT
	delete subParams;
	subParams = new SubsystemParameters();
	subParams->addDoubleParameter("double2",2.00);
	subParams = call_function<SubsystemParameters*> (L, "test_SubsystemParameter", subParams);
}

void testScriptedSubsystem(lua_State* L) {
	string initFcn = "test_ScriptedSubsystem_init";
	string canPerformFcn = "test_ScriptedSubsystem_canPerform";
	string canExtendFcn = "test_ScriptedSubsystem_canExtend";
	string subName = "Scripted Subsystem Test";

	ScriptedSubsystem* subsystem_ = new ScriptedSubsystem(L, subName, initFcn, canPerformFcn, canExtendFcn);
	Subsystem* subsystem = (Subsystem*)subsystem_;
	// Verify init code
}

}} // horizon::test 
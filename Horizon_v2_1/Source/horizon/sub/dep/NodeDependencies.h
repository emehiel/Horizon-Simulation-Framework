#ifndef NODEDEPENDENCIES_H
#define NODEDEPENDENCIES_H

#include "Dependencies.h"
#include <string>
#include <map>

namespace horizon {
	namespace sub {
		namespace dep {

	class Dependencies;
	typedef Profile<int>	(Dependencies::*IntDependency) ();
	typedef Profile<float>	(Dependencies::*FloatDependency) ();
	typedef Profile<double> (Dependencies::*DoubleDependency) ();
	typedef Profile<bool>	(Dependencies::*BoolDependency) ();
	typedef Profile<Matrix>	(Dependencies::*MatrixDependency) ();
	typedef Profile<Quat>	(Dependencies::*QuatDependency) ();

class NodeDependencies {
	Dependencies* depInstance;

	map<string, IntDependency>		intDependencies;
	map<string, DoubleDependency>	doubleDependencies;
	map<string, FloatDependency>	floatDependencies;
	map<string, BoolDependency>		boolDependencies;
	map<string, MatrixDependency>	matrixDependencies;
	map<string, QuatDependency>		quatDependencies;

public:
	NodeDependencies();
	
	~NodeDependencies();

	Profile<int>	callIntDependency(string key);
	Profile<double> callDoubleDependency(string key);
	Profile<float>	callFloatDependency(string key);
	Profile<bool>	callBoolDependency(string key);
	Profile<Matrix> callMatrixDependency(string key);
	Profile<Quat>	callQuatDependency(string key);

	void addDependcy(string key, IntDependency dep);
	void addDependcy(string key, DoubleDependency dep);
	void addDependcy(string key, FloatDependency dep);
	void addDependcy(string key, BoolDependency dep);
	void addDependcy(string key, MatrixDependency dep);
	void addDependcy(string key, QuatDependency dep);
};

}}} // end namespace horizon::sub::dep
#endif // NODEDEPENDENCIES_H
/**
 * \file DependencyAdapter.h
 * This file contains the DependencyAdapter class.
 * @author Brian Butler
 */

#ifndef DEPENDENCYADAPTER_H
#define DEPENDENCYADAPTER_H

#include "horizon/sub/dep/Dependencies.h"
#include "horizon/sub/dep/NodeDependencies.h"
#include <string>
#include <map>

using namespace horizon::sub::dep;

namespace horizon {
	namespace util {
		namespace adapt {
/**
 * A class for holding static dependencies and retrieving them using strings as a key.
 * @author Brian Butler
 */
class DependencyAdapter {
	map<string, IntDependency>		intDependencies;
	map<string, DoubleDependency>	doubleDependencies;
	map<string, FloatDependency>	floatDependencies;
	map<string, BoolDependency>		boolDependencies;
	map<string, MatrixDependency>	matrixDependencies;
	map<string, QuatDependency>		quatDependencies;

public:
	/** The constructor for the DependencyAdapter class. */
	DependencyAdapter();

	/**
	 * Adds an IntDependency.
	 * @param key The key that must be used to look up the IntDependency.
	 * @param dep The IntDependency to add.
	 */
	void addIntDependency(string key, IntDependency dep);

	/**
	 * Adds a DoubleDependency.
	 * @param key The key that must be used to look up the DoubleDependency.
	 * @param dep The DoubleDependency to add.
	 */
	void addDoubleDependency(string key, DoubleDependency dep);

	/**
	 * Adds a FloatDependency.
	 * @param key The key that must be used to look up the FloatDependency.
	 * @param dep The FloatDependency to add.
	 */
	void addFloatDependency(string key, FloatDependency dep);

	/**
	 * Adds a BoolDependency.
	 * @param key The key that must be used to look up the BoolDependency.
	 * @param dep The BoolDependency to add.
	 */
	void addBoolDependency(string key, BoolDependency dep);

	/**
	 * Adds a MatrixDependency.
	 * @param key The key that must be used to look up the MatrixDependency.
	 * @param dep The MatrixDependency to add.
	 */
	void addMatrixDependency(string key, MatrixDependency dep);

	/**
	 * Adds a QuatDependency.
	 * @param key The key that must be used to look up the QuatDependency.
	 * @param dep The QuatDependency to add.
	 */
	void addQuatDependency(string key, QuatDependency dep);

	/**
	 * Gets an IntDependnecy.
	 * @param key The string to lookup the IntDependency with.
	 * @return The IntDependency that was stored using <CODE>key</CODE>
	 */
	IntDependency getIntDependency(string key);

	/**
	 * Gets a DoubleDependency.
	 * @param key The string to lookup the DoubleDependency with.
	 * @return The DoubleDependency that was stored using <CODE>key</CODE>
	 */
	DoubleDependency getDoubleDependency(string key);

	/**
	 * Gets a FloatDependency.
	 * @param key The string to lookup the FloatDependency with.
	 * @return The FloatDependency that was stored using <CODE>key</CODE>
	 */
	FloatDependency getFloatDependency(string key);

	/**
	 * Gets a BoolDependency.
	 * @param key The string to lookup the BoolDependency with.
	 * @return The BoolDependency that was stored using <CODE>key</CODE>
	 */
	BoolDependency getBoolDependency(string key);

	/**
	 * Gets a MatrixDependency.
	 * @param key The string to lookup the MatrixDependency with.
	 * @return The MatrixDependency that was stored using <CODE>key</CODE>
	 */
	MatrixDependency getMatrixDependency(string key);

	/**
	 * Gets a QuatDependency.
	 * @param key The string to lookup the QuatDependency with.
	 * @return The QuatDependency that was stored using <CODE>key</CODE>
	 */
	QuatDependency getQuatDependency(string key);
};

}}}
#endif
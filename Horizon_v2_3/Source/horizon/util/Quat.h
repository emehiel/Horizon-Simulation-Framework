#ifndef QUAT_H
#define QUAT_H

#include <iostream>
#include <valarray>
#include "Matrix.h"

namespace horizon {
namespace util {

using std::valarray;
using std::cout;
using std::ostream;
using horizon::util::Matrix;

/**
 * Defines the quaternion and its operations, a hyper-complex number of rank 4 with
 * properties that make it useful for rotation calculations.
 * q = q0 + (q1)i + (q2)j + (q3)k  where  i^2 = j^2 = k^2 = ijk = -1
 * @author Travis Lockyer
 */
class Quat
{
public:
	/**
	 * Creates a null quaternion.
	 */
	Quat();
	
	/**
	 * Creates a quaternion without its imaginary part.
	 * @param q0 the real component of the quaternion
	 */
	Quat(const double& q0);
	
	/**
	 * Creates a pure quaternion.
	 * @param q1 the first element of the imaginary vector
	 * @param q2 the second element of the imaginary vector
	 * @param q3 the third element of the imaginary vector
	 */
	Quat(const double& q1, const double& q2, const double& q3);
	
	/**
	 * Creates a fully defined quaternion.
	 * @param q0 the first element of the quaternion
	 * @param q1 the first element of the quaternion
	 * @param q2 the first element of the quaternion
	 * @param q3 the first element of the quaternion
	 */
	Quat(const double& q0, const double& q1, const double& q2, const double& q3);
	
	/**
	 * Creates a fully defined quaternion from a valarray of 4 doubles.
	 * @param vals a valarray containing the 4 elements of the quaternion
	 */
	Quat(const valarray<double>& vals);

	/**
	 * Creates a quaternion from a rotation axis and angle of rotation.
	 * @param v the rotation axis
	 * @param theta the angle of rotation
	 */
	Quat(const Matrix& v, const double& theta);

	/**
	 * Creates a quaternion from two matrices
	 * @param c the first matrix
	 * @param d the second matrix
	 */
	Quat(const Matrix& c, const Matrix& d);
	
	/** Destroys the quaternion */
	~Quat();
	
	/**
	 * Returns the real part of the quaternion.
	 * @return the real part of the quaternion
	 */
	double real() const;
	
	/**
	 * Returns the imaginary part of the quaternion as a valarray.
	 * @return a valarray of the quaternion's imag. vector
	 */
	std::valarray<double> imag() const;
	
	/**
	 * Returns the first element of the quaternion.
	 * @return component q0
	 */
	double comp_q0() const;
	
	/**
	 * Returns the second element of the quaternion.
	 * @return component q1
	 */
	double comp_q1() const;
	
	/**
	 * Returns the third element of the quaternion.
	 * @return component q2
	 */
	double comp_q2() const;
	
	/**
	 * Returns the fourth element of the quaternion.
	 * @return component q3
	 */
	double comp_q3() const;
	
	/**
	 * Returns the quaternion as a valarray.
	 * @return a valarray containing the quaternion components
	 */
	std::valarray<double> comp_array() const;
	
	/**
	 * Prints the quaternion in the form:
	 *   q0 + q1i + q2j + q3k
	 */
	void print() const;
	
	/**
	 * Sets the first element of the quaternion.
	 * @param q0 the desired value of the first element
	 */
	void set_q0(const double& q0);
	
	/**
	 * Sets the second element of the quaternion.
	 * @param q1 the desired value of the second element
	 */
	void set_q1(const double& q1);
	
	/**
	 * Sets the third element of the quaternion.
	 * @param q2 the desired value of the third element
	 */
	void set_q2(const double& q2);
	
	/**
	 * Sets the fourth element of the quaternion.
	 * @param q3 the desired value of the fourth element
	 */
	void set_q3(const double& q3);
	
	/**
	 * Returns the conjugate of the quaternion.
	 * @return the quaternion's conjugate
	 */
	Quat conjugate() const;
	
	/**
	 * Returns the norm of the quaternion.
	 * @return the quaternion's norm
	 */
	double norm() const;
	
	/**
	 * Normalizes the quaternion to unit length.
	 * @return the normalized quaternion
	 */
	Quat & normalize();

	/*
	// converts a string to a quaternion.  The string must look like the quaternion constructor
	// e.g. Quat(1, {1.0})
	// e.g. Quat(3, {1.0, 1.0, 1.0})
	// e.g. Quat(4, {1.0, 1.0, 1.0, 1.0})
	*/
	friend Quat atoq (const string &inQuat);
	
	Quat & operator= (double *vals);
	Quat & operator-= (const Quat& rhs);
	Quat & operator+= (const Quat& rhs);
	Quat & operator*= (const Quat& rhs);
		
	friend std::ostream & operator<< (std::ostream& ostrm, const Quat& q);

	friend Quat operator+ (const Quat& q);
	friend Quat operator+ (const Quat& p, const Quat& q);
	friend Quat operator+ (const double& k, const Quat& q);
	friend Quat operator+ (const Quat& q, const double& k);

	friend Quat operator- (const Quat& q);
	friend Quat operator- (const Quat& p, const Quat& q);
	friend Quat operator- (const double& k, const Quat& q);
	friend Quat operator- (const Quat& q, const double& k);
	
	friend Quat operator* (const double& k, const Quat& q);
	friend Quat operator* (const Quat& q, const double& k);
	friend Quat operator* (const Quat& p, const Quat& q );

	friend bool operator== (const Quat& p, const Quat& q);
	friend bool operator!= (const Quat& p, const Quat& q);

private:

	/** A valarray containing the 4 elements of the quaternion */
	std::valarray<double> q_;
};

}} // end namespace horizon::util

#endif /*QUAT_H*/
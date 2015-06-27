#include "umath.h"

namespace horizon {
namespace util {
namespace umath {
	
valarray<double> cross(const valarray<double>& u, const valarray<double>& v) {
	valarray<double> w(3);		// the cross product of u and v
	// calculate each element of the cross product
	// assumes u and v are vectors in 3-space
	w[0] = u[1] * v[2] - u[2] * v[1];
	w[1] = u[2] * v[0] - u[0] * v[2];
	w[2] = u[0] * v[1] - u[1] * v[0];
	return w;
}

double dot(const valarray<double>& u, const valarray<double>& v) {
	// the dot product the sum of the elements after element by element multiplication
	// assumes u and v are the same length
	return (u*v).sum();
}

Matrix cross(const Matrix& u, const Matrix& v) {
	deque<double> uvals = u.getData();	// deque of values for the u vector
	deque<double> vvals = v.getData();	// deque of values for the v vector
	deque<double> wvals(3,0.0);			// initialize deque for w vector values

	// check whether u and v are vectors in 3-space
	if (uvals.size() == 3 && vvals.size() == 3) {
		// calculate each element of the cross product
		wvals[0] = uvals[1] * vvals[2] - uvals[2] * vvals[1];
		wvals[1] = uvals[2] * vvals[0] - uvals[0] * vvals[2];
		wvals[2] = uvals[0] * vvals[1] - uvals[1] * vvals[0];
	}
	Matrix w(3, 1, 0.0);				// initialize matrix for w vector
	w.setData(wvals);					// set w matrix vector to deque of w values
	return w;
}

double dot(const Matrix& u, const Matrix& v) {
	deque<double> uvals = u.getData();
	deque<double> vvals = v.getData();
	double dotp = 0;
	if (uvals.size() == vvals.size())
		dotp = inner_product(uvals.begin(), uvals.end(), vvals.begin(), 0.0);
	return dotp;
}

Matrix quat2matrix_v(const Quat& q) {
	Matrix Q(3);	// initialize rotation matrix
	// perform algorithm for conversion from quaternion to matrix
	// from "The Matrix and Quaternions FAQ" at <http://vamos.sourceforge.net/matrixfaq.htm>
	Q.setValue( 1, 1, 2 * q.comp_q0() * q.comp_q0() - 1 +
					  2 * q.comp_q1() * q.comp_q1());
	Q.setValue( 1, 2, 2 * q.comp_q1() * q.comp_q2() -
					  2 * q.comp_q0() * q.comp_q3());
	Q.setValue( 1, 3, 2 * q.comp_q1() * q.comp_q3() +
					  2 * q.comp_q0() * q.comp_q2());
	Q.setValue( 2, 1, 2 * q.comp_q1() * q.comp_q2() +
					  2 * q.comp_q0() * q.comp_q3());
	Q.setValue( 2, 2, 2 * q.comp_q0() * q.comp_q0() - 1 +
					  2 * q.comp_q2() * q.comp_q2());
	Q.setValue( 2, 3, 2 * q.comp_q2() * q.comp_q3() -
					  2 * q.comp_q0() * q.comp_q1());
	Q.setValue( 3, 1, 2 * q.comp_q1() * q.comp_q3() -
					  2 * q.comp_q0() * q.comp_q2());
	Q.setValue( 3, 2, 2 * q.comp_q2() * q.comp_q3() +
					  2 * q.comp_q0() * q.comp_q1());
	Q.setValue( 3, 3, 2 * q.comp_q0() * q.comp_q0() - 1 +
					  2 * q.comp_q3() * q.comp_q3());
	return Q;
}

Matrix quat2matrix_cf(const Quat& q) {
	Matrix Q(3);	// initialize rotation matrix
	// perform algorithm for conversion from quaternion to matrix
	// from "The Matrix and Quaternions FAQ" at <http://vamos.sourceforge.net/matrixfaq.htm>
	Q.setValue( 1, 1, 2 * q.comp_q0() * q.comp_q0() - 1 +
					  2 * q.comp_q1() * q.comp_q1());
	Q.setValue( 1, 2, 2 * q.comp_q1() * q.comp_q2() -
					  2 * q.comp_q0() * q.comp_q3());
	Q.setValue( 1, 3, 2 * q.comp_q1() * q.comp_q3() +
					  2 * q.comp_q0() * q.comp_q2());
	Q.setValue( 2, 1, 2 * q.comp_q1() * q.comp_q2() +
					  2 * q.comp_q0() * q.comp_q3());
	Q.setValue( 2, 2, 2 * q.comp_q0() * q.comp_q0() - 1 +
					  2 * q.comp_q2() * q.comp_q2());
	Q.setValue( 2, 3, 2 * q.comp_q2() * q.comp_q3() -
					  2 * q.comp_q0() * q.comp_q1());
	Q.setValue( 3, 1, 2 * q.comp_q1() * q.comp_q3() -
					  2 * q.comp_q0() * q.comp_q2());
	Q.setValue( 3, 2, 2 * q.comp_q2() * q.comp_q3() +
					  2 * q.comp_q0() * q.comp_q1());
	Q.setValue( 3, 3, 2 * q.comp_q0() * q.comp_q0() - 1 +
					  2 * q.comp_q3() * q.comp_q3());
	return transpose(Q);
}

Quat matrix2quat_v(const Matrix& Q) {
	// perform algorithm to convert the rotation matrix to a quaternion
	// from "The Matrix and Quaternions FAQ" at <http://vamos.sourceforge.net/matrixfaq.htm>
	double T = Q.getValue(1,1) + Q.getValue(2,2) + Q.getValue(3,3) + 1;
	double a, b, c, d;
	if (T > 0) {
		double S = 0.5 / sqrt(T);
		a = 0.25 / S;
		b = (Q.getValue(3,2) - Q.getValue(2,3)) * S;
		c = (Q.getValue(1,3) - Q.getValue(3,1)) * S;
		d = (Q.getValue(2,1) - Q.getValue(1,2)) * S;		   
	} else if (Q.getValue(1,1) > Q.getValue(2,2) &&
			   Q.getValue(1,1) > Q.getValue(3,3)) {
		double S = sqrt(1.0 + Q.getValue(1,1) - 
			Q.getValue(2,2) - Q.getValue(3,3)) * 2;
		a = (Q.getValue(2,3) + Q.getValue(3,2)) / S;
		b = 0.5 / S;
		c = (Q.getValue(1,2) + Q.getValue(2,1)) / S;
		d = (Q.getValue(1,3) + Q.getValue(3,1)) / S;
	} else if (Q.getValue(2,2) > Q.getValue(3,3)) {
		double S = sqrt(1.0 + Q.getValue(2,2) - 
			Q.getValue(1,1) - Q.getValue(3,3)) * 2;
		a = (Q.getValue(1,3) + Q.getValue(3,1)) / S;
		b = (Q.getValue(1,2) + Q.getValue(2,1)) / S;
		c = 0.5 / S;
		d = (Q.getValue(2,3) + Q.getValue(3,2)) / S;
	} else {
		double S = sqrt(1.0 + Q.getValue(3,3) - 
			Q.getValue(1,1) - Q.getValue(2,2)) * 2;
		a = (Q.getValue(1,2) + Q.getValue(2,1)) / S;
		b = (Q.getValue(1,3) + Q.getValue(3,1)) / S;
		c = (Q.getValue(2,3) + Q.getValue(3,2)) / S;
		d = 0.5 / S;
	}
	Quat q(a, b, c, d);
	return q;
}

Quat matrix2quat_cf(const Matrix& Q) {
	// perform algorithm to convert the rotation matrix to a quaternion
	// from "The Matrix and Quaternions FAQ" at <http://vamos.sourceforge.net/matrixfaq.htm>
	double T = Q.getValue(1,1) + Q.getValue(2,2) + Q.getValue(3,3) + 1;
	double a, b, c, d;
	if (T > 0) {
		double S = 0.5 / sqrt(T);
		a = 0.25 / S;
		b = (Q.getValue(3,2) - Q.getValue(2,3)) * S;
		c = (Q.getValue(1,3) - Q.getValue(3,1)) * S;
		d = (Q.getValue(2,1) - Q.getValue(1,2)) * S;		   
	} else if (Q.getValue(1,1) > Q.getValue(2,2) &&
			   Q.getValue(1,1) > Q.getValue(3,3)) {
		double S = sqrt(1.0 + Q.getValue(1,1) - Q.getValue(2,2) - Q.getValue(3,3)) * 2;
		a = (Q.getValue(2,3) + Q.getValue(3,2)) / S;
		b = 0.5 / S;
		c = (Q.getValue(1,2) + Q.getValue(2,1)) / S;
		d = (Q.getValue(1,3) + Q.getValue(3,1)) / S;
	} else if (Q.getValue(2,2) > Q.getValue(3,3)) {
		double S = sqrt(1.0 + Q.getValue(2,2) - Q.getValue(1,1) - Q.getValue(3,3)) * 2;
		a = (Q.getValue(1,3) + Q.getValue(3,1)) / S;
		b = (Q.getValue(1,2) + Q.getValue(2,1)) / S;
		c = 0.5 / S;
		d = (Q.getValue(2,3) + Q.getValue(3,2)) / S;
	} else {
		double S = sqrt(1.0 + Q.getValue(3,3) - Q.getValue(1,1) - Q.getValue(2,2)) * 2;
		a = (Q.getValue(1,2) + Q.getValue(2,1)) / S;
		b = (Q.getValue(1,3) + Q.getValue(3,1)) / S;
		c = (Q.getValue(2,3) + Q.getValue(3,2)) / S;
		d = 0.5 / S;
	}
	Quat q(a, -b, -c, -d);
	return q;
}

Quat mv2pq(const Matrix& v) {
	Quat q;					// initialize quaternion
	deque<double> vals = v.getData();
	// set q1, q2, and q3 to the matrix values, leaving q0 zero
	q.set_q1(vals[0]);
	q.set_q2(vals[1]);
	q.set_q3(vals[2]);
	return q;
}

Matrix pq2mv(const Quat& q) {
	Matrix v(3, 1, 0.0);			// initialize matrix vector
	// set v equal to the matrix part of the quaternion
	v.setValue(1,1,q.comp_q1());
	v.setValue(2,1,q.comp_q2());
	v.setValue(3,1,q.comp_q3());
	return v;
}

Matrix cf_rotate(const Matrix& v, const Quat& q) {
	Matrix vp(3, 1, 0.0);			// initialize v', the vector in the rotated coord. frame
	unsigned int rows, cols;
	v.getSize(rows, cols);
	// check whether the matrix is the correct dimensions
	if ((rows == 3 && cols == 1) || (rows == 1 && cols == 3))
		// from Kuipers, Pp. 127
		vp = pq2mv(q.conjugate() * mv2pq(v) * q);
	return vp;
}

Matrix v_rotate(const Matrix& v, const Quat& q) {
	Matrix vp(3, 1, 0.0);			// initialize v', the vector in the rotated coord. frame
	// initialize the number of rows and columns
	unsigned int rows, cols;
	v.getSize(rows, cols);
	if ((rows == 3 && cols == 1) || (rows == 1 && cols == 3))
		// from Kuipers, Pp. 127
		vp = pq2mv(q * mv2pq(v) * q.conjugate());
	return vp;
}

}}} // end namespace horizon::util::umath
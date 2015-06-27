#ifndef UMATH_H
#define UMATH_H

#include <valarray>
#include <cmath>
#include <numeric>
#include <map>
#include "Quat.h"
#include "Matrix.h"
#include "../util/spline/Spline.h"

namespace horizon {
namespace util {
namespace umath {

	/// \todo TODO: should we include matrix operations dot and cross inside matrix file?

using std::valarray;
using std::inner_product;
using std::map;
using namespace horizon::util::umath::spline;

/**
 * Finds the cross product of two vectors represented as valarrays.
 * @param u first vector
 * @param v second vector
 * @return the cross product
 */
valarray<double> cross(const valarray<double>& u, const valarray<double>& v);

/**
 * Finds the cross product of two vectors represented as column vectors
 * or row vectors.
 * @param u first vector
 * @param v second vector
 * @return the cross product
 */
Matrix cross(const Matrix& u, const Matrix& v);

/**
 * Finds the dot product of two vectors represented as valarrays.
 * @param u first vector
 * @param v second vector
 * @return the dot product
 */
double dot(const valarray<double>& u, const valarray<double>& v);

/**
 * Finds the dot product of two vectors represented as column vectors.
 * @param u first vector
 * @param v second vector
 * @return the dot product
 */
double dot(const Matrix& u, const Matrix& v);

/**
 * Converts a quaternion to a rotation matrix for a
 * vector rotation.
 * @param q the quaternion
 * @return the rotation matrix
 */
Matrix quat2matrix_v(const Quat& q);

/**
 * Converts a quaternion to a rotation matrix for a
 * coordinate frame rotation.
 * @param q the quaternion
 * @return the rotation matrix
 */
Matrix quat2matrix_cf(const Quat& q);

/**
 * Converts a rotation matrix to a quaternion for a
 * vector rotation.  The algorithm was obtained from
 * "The Matrix and Quaternions FAQ" at
 * <http://vamos.sourceforge.net/matrixfaq.htm>.
 * @param Q the rotation matrix
 * @return the quaternion
 */
Quat matrix2quat_v(const Matrix& Q);

/**
 * Converts a rotation matrix to a quaternion for a
 * vector rotation.  The algorithm was obtained from
 * "The Matrix and Quaternions FAQ" at
 * <http://vamos.sourceforge.net/matrixfaq.htm>.
 * @param Q the rotation matrix
 * @return the quaternion
 */
Quat matrix2quat_cf(const Matrix& Q);

/**
 * Transforms a column vector or row vector into a
 * pure quaternion.
 * @param v the column or row vector
 * @return the pure quaternion
 */
Quat mv2pq(const Matrix& v);

/**
 * Transforms a pure quaternion to a column vector.
 * @param q the quaternion
 * @return the  column vector
 */
Matrix pq2mv(const Quat& q);

/**
 * Rotates the coordinate frame of a vector represented by
 * a pure quaternion about a rotation quaternion.
 * @param v the column or row vector
 * @param q the quaternion that specifies the axis
 * and angle of rotation
 * @return the column vector in the new coord. frame
 */
Matrix cf_rotate(const Matrix& v, const Quat& q);

/**
 * Rotates a vector, represented by a pure quaternion, in its
 * coordinate frame about a rotation quaternion.
 * @param v the column or row vector
 * @param q the quaternion that specifies the axis
 * and angle of rotation
 * @return the rotated column vector
 */
Matrix v_rotate(const Matrix& v, const Quat& q);

inline double interp_spline(int n, double xi[], double yi[], double xinterp)
{
	int i;
	vector<double> xiV, yiV;
	for(i = 0; i<=n-1; i++) {
		xiV.push_back(xi[i]);
		yiV.push_back(yi[i]);
	}
	vector<double> ypp = findSpline2ndDeriv(xiV, yiV, 2, 0.0, 2, 0.0);
	double yinterp = evalCubicSpline(xiV, xinterp, yiV, ypp);
	return yinterp;
}

inline double interp_spline(vector<double> xi, vector<double> yi, double xinterp)
{
	vector<double> ypp = findSpline2ndDeriv(xi, yi, 2, 0.0, 2, 0.0);
	double yinterp = evalCubicSpline(xi, xinterp, yi, ypp);
	return yinterp;
}

inline double interp_spline(Matrix xiM, Matrix yiM, double xinterp)
{
	unsigned int i;
	vector<double> xi(xiM.getNumElems()), yi(xiM.getNumElems());
	for(i = 0; i< xiM.getNumElems(); i++) {
		xi[i] = xiM.getData().at(i);
		yi[i] = yiM.getData().at(i);
	}
	vector<double> ypp = findSpline2ndDeriv(xi, yi, 2, 0.0, 2, 0.0);
	double yinterp = evalCubicSpline(xi, xinterp, yi, ypp);
	return yinterp;
}

inline Matrix interp_spline(map<double, Matrix> data, double xinterp)
{
	unsigned int i, j;
	unsigned int nrows = (*data.begin()).second.getNumRows();
	unsigned int ncols = (*data.begin()).second.getNumCols();
	Matrix result(nrows, ncols, 0.0);
	vector<double> xi(data.size()), yi(data.size());

	for(i = 1; i <= nrows; i++) {
		for( j = 1; j <= ncols; j++) {
			unsigned int k = 0;
			map<double, Matrix>::iterator mIt = data.begin();
			for(mIt; mIt!=data.end(); mIt++) {
				xi[k] = (*mIt).first;
				yi[k] = (*mIt).second.getValue(i, j);
				k++;
			}
			result.setValue(i, j, interp_spline(xi, yi, xinterp));
		}
	}
	return result;
}

inline vector<double> resample_spline(int n, double xi[], double yi[], double xresample[], int nresample)
{
	int i = 0;
	vector<double> result(nresample);
	for(i; i<=nresample-1; i++) {
		result[i] = interp_spline(n, xi, yi, xresample[i]);
	}
	return result;
}
inline vector<double> resample_spline(vector<double> xi, vector<double> yi, vector<double> xresample)
{
	int i = 0;
	int nresample = int(xresample.size());
	vector<double> result(nresample);
	for(i; i<=nresample-1; i++) {
		result[i] = interp_spline(xi, yi, xresample[i]);
		printf("Resample State:  %4.2f%% done\r", 100*i/nresample);
	}
	printf("\n");
	return result;
}
inline Matrix resample_spline(Matrix xiM, Matrix yiM, Matrix xresample)
{
	int i = 0;
	int nresample = int(xresample.getNumElems());
	Matrix result(nresample, 1, 0.0);
	for(i; i<=nresample-1; i++) {
		result.setValue(i+1, 1, interp_spline(xiM, yiM, xresample.getData().at(i)));
	}
	return result;
}

inline map<double, Matrix> resample_spline(map<double, Matrix> data, vector<double> xresample)
{
	int i = 0;
	int nresample = int(xresample.size());
	map<double, Matrix> result;
	for(i; i<=nresample-1; i++) {
		result[xresample[i]] = interp_spline(data, xresample[i]);
		printf("Resample State:  %4.2f%% done\r", 100*(double)i/(double)nresample);
	}
	printf("\n");
	return result;
}


}}} // end namespace horizon::util::umath

#endif // UMATH_H
#ifndef SPLINE_H
#define SPLINE_H

#include <iostream>
#include <vector>

namespace horizon {
namespace util {
namespace umath {
namespace spline {

		using std::cout;
		using std::vector;

		//    This function factors and solves a D3 system.
		//
		//    The D3 storage format is used for a tridiagonal matrix.
		//    The superdiagonal is stored in entries (1,2:N), the diagonal in
		//    entries (2,1:N), and the subdiagonal in (3,1:N-1).  Thus, the
		//    original matrix is "collapsed" vertically into the array.
		//
		//    This algorithm requires that each diagonal entry be nonzero.
		//    It does not use pivoting, and so can fail on systems that
		//    are actually nonsingular.
		//
		//    Input/output, vector A[3*N].
		//    On input, the nonzero diagonals of the linear system.
		//    On output, the data in these vectors has been overwritten
		//    by factorization information.
		//
		//    Input, vector B[N], the right hand side.
		//
		//    Output, vector[N], the solution of the linear system.
		//    This is NULL if there was an error because one of the diagonal
		//    entries was zero.
		//
		vector<double> solveD3(vector<double> a, vector<double> b);

		//    This function computes the second derivatives of a piecewise cubic spline.
		//
		//    The cubic spline is a piecewise cubic polynomial.  The intervals
		//    are determined by the "knots" or abscissas of the data to be
		//    interpolated.  The cubic spline has continous first and second
		//    derivatives over the entire interval of interpolation.
		//
		//    Input, vector x[N], the knot values, that is, the points were data is
		//    specified.  The knot values should be distinct, and increasing.
		//
		//    Input, vector y[N], the data values to be interpolated.
		//
		//    Input, int IBCBEG, left boundary condition flag:
		//      0: the cubic spline should be a quadratic over the first interval;
		//      1: the first derivative at the left endpoint should be YBCBEG;
		//      2: the second derivative at the left endpoint should be YBCBEG.
		//
		//    Input, double YBCBEG, the values to be used in the boundary
		//    conditions if IBCBEG is equal to 1 or 2.
		//
		//    Input, int IBCEND, right boundary condition flag:
		//      0: the cubic spline should be a quadratic over the last interval;
		//      1: the first derivative at the right endpoint should be YBCEND;
		//      2: the second derivative at the right endpoint should be YBCEND.
		//
		//    Input, double YBCEND, the values to be used in the boundary
		//    conditions if IBCEND is equal to 1 or 2.
		//
		//    Output, vector[N], the second derivatives of the cubic spline.
		//
		vector<double> findSpline2ndDeriv(vector<double> x, vector<double> y, int ibcbeg, 
			double ybcbeg, int ibcend, double ybcend);

		//    This function evaluates a piecewise cubic spline at a point.
		//
		//    Input, vector x[N], the knot values.
		//
		//    Input, double xVAL, a point, typically between x[0] and x[N-1], at
		//    which the spline is to be evalulated.  If xVAL lies outside
		//    this range, extrapolation is used.
		//
		//    Input, vector y[N], the data values at the knots.
		//
		//    Input, vector YPP[N], the second derivatives of the spline at
		//    the knots.
		//
		//    Output, double, the value of the spline at xVAL.
		//
		double evalCubicSpline(vector<double> x, double xval, vector<double> y, 
			vector<double> ypp);


		inline vector<double> solveD3(vector<double> a, vector<double> b)
		{
			int n = int(b.size());
			int i;
			vector<double> x(n);
			double xmult;
			//
			//  Check.
			//
			for ( i = 0; i < n; i++ ) {
				if ( a[1+i*3] == 0.0 ) {
					return x;
				}
			}
			
			for ( i = 0; i < n; i++ ) {
				x[i] = b[i];
			}

			for ( i = 1; i < n; i++ ) {
				xmult = a[2+(i-1)*3] / a[1+(i-1)*3];
				a[1+i*3] = a[1+i*3] - xmult * a[0+i*3];
				x[i] = x[i] - xmult * x[i-1];
			}

			x[n-1] = x[n-1] / a[1+(n-1)*3];
			for ( i = n-2; 0 <= i; i-- ) {
				x[i] = ( x[i] - a[0+(i+1)*3] * x[i+1] ) / a[1+i*3];
			}
			return x;
		}
		

		inline vector<double> findSpline2ndDeriv(vector<double> x, vector<double> y, int ibcbeg, 
			double ybcbeg, int ibcend, double ybcend)
		{
			int n = int(x.size());
			vector<double> a(3*n);
			vector<double> b(n);
			int i;
			vector<double> ypp;

			//
			//  Check.
			//
			if ( n <= 1 ) {
				cout << "\n";
				cout << "findSpline2ndDeriv - Fatal error!\n";
				cout << "  The number of data points N must be at least 2.\n";
				cout << "  The input value is " << n << ".\n";
				return ypp;
			}

			for ( i = 0; i < n - 1; i++ ) {
				if ( x[i+1] <= x[i] ) {
					cout << "\n";
					cout << "findSpline2ndDeriv - Fatal error!\n";
					cout << "  The knots must be strictly increasing, but\n";
					cout << "  x(" << i   << ") = " << x[i]   << "\n";
					cout << "  x(" << i+1 << ") = " << x[i+1] << "\n";
					return ypp;
				}
			}

			//
			//  Set up the first equation.
			//
			if ( ibcbeg == 0 ) {
				b[0] = 0.0;
				a[1+0*3] = 1.0;
				a[0+1*3] = -1.0;
			}

			else if ( ibcbeg == 1 ) {
				b[0] = ( y[1] - y[0] ) / ( x[1] - x[0] ) - ybcbeg;
				a[1+0*3] = ( x[1] - x[0] ) / 3.0;
				a[0+1*3] = ( x[1] - x[0] ) / 6.0;
			}

			else if ( ibcbeg == 2 ) {
				b[0] = ybcbeg;
				a[1+0*3] = 1.0;
				a[0+1*3] = 0.0;
			}

			else {
				cout << "\n";
				cout << "findSpline2ndDeriv - Fatal error!\n";
				cout << "  IBCBEG must be 0, 1 or 2.\n";
				cout << "  The input value is " << ibcbeg << ".\n";
				return ypp;
			}

			//
			//  Set up the intermediate equations.
			//
			for ( i = 1; i < n-1; i++ ) {
				b[i] = ( y[i+1] - y[i] ) / ( x[i+1] - x[i] )
				- ( y[i] - y[i-1] ) / ( x[i] - x[i-1] );
				a[2+(i-1)*3] = ( x[i] - x[i-1] ) / 6.0;
				a[1+ i   *3] = ( x[i+1] - x[i-1] ) / 3.0;
				a[0+(i+1)*3] = ( x[i+1] - x[i] ) / 6.0;
			}

			//
			//  Set up the last equation.
			//
			if ( ibcend == 0 ) {
				b[n-1] = 0.0;
				a[2+(n-2)*3] = -1.0;
				a[1+(n-1)*3] = 1.0;
			}
			else if ( ibcend == 1 ) {
				b[n-1] = ybcend - ( y[n-1] - y[n-2] ) / ( x[n-1] - x[n-2] );
				a[2+(n-2)*3] = ( x[n-1] - x[n-2] ) / 6.0;
				a[1+(n-1)*3] = ( x[n-1] - x[n-2] ) / 3.0;
			}
			else if ( ibcend == 2 ) {
				b[n-1] = ybcend;
				a[2+(n-2)*3] = 0.0;
				a[1+(n-1)*3] = 1.0;
			}
			else {
				cout << "\n";
				cout << "findSpline2ndDeriv - Fatal error!\n";
				cout << "  IBCEND must be 0, 1 or 2.\n";
				cout << "  The input value is " << ibcend << ".\n";
				return ypp;
			}
			//
			//  Solve the linear system.
			//
			if ( n == 2 && ibcbeg == 0 && ibcend == 0 ) {
				ypp[0] = 0.0;
				ypp[1] = 0.0;
			}
			else {
				ypp = solveD3(a, b);
				if ((ypp.size() == 0)) {
					cout << "\n";
					cout << "findSpline2ndDeriv - Fatal error!\n";
					cout << "  The linear system could not be solved.\n";
					return ypp;
				}
			}

			return ypp;
		}

		

		inline double evalCubicSpline(vector<double> x, double xval, vector<double> y, 
			vector<double> ypp)
		{
			int n = int(x.size());
			double dt;
			double h;
			int i;
			int ival;
			double yval;
			//
			//  Determine the interval [ x(I), x(I+1) ] that contains xval.
			//  Values below x[0] or above x[N-1] use extrapolation.
			//
			ival = n - 2;

			for ( i = 0; i < n-1; i++ )
			{
				if ( xval < x[i+1] )
				{
					ival = i;
					break;
				}
			}
			//
			//  In the interval I, the polynomial is in terms of a normalized
			//  coordinate between 0 and 1.
			//
			dt = xval - x[ival];
			h = x[ival+1] - x[ival];

			yval = y[ival]
			+ dt * ( ( y[ival+1] - y[ival] ) / h
				   - ( ypp[ival+1] / 6.0 + ypp[ival] / 3.0 ) * h
			+ dt * ( 0.5 * ypp[ival]
			+ dt * ( ( ypp[ival+1] - ypp[ival] ) / ( 6.0 * h ) ) ) );

			/*
			double ypval, yppval;
			ypval = ( y[ival+1] - y[ival] ) / h
			- ( ypp[ival+1] / 6.0 + ypp[ival] / 3.0 ) * h
			+ dt * ( ypp[ival]
			+ dt * ( 0.5 * ( ypp[ival+1] - ypp[ival] ) / h ) );

			*yppval = ypp[ival] + dt * ( ypp[ival+1] - ypp[ival] ) / h;
			*/

			return yval;
		}

}}}}

#endif
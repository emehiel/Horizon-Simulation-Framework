#ifndef MATRIX_H
#define MATRIX_H

#include <deque>
#include "matrixIndex.h"
#include <algorithm>
#include <numeric>
#include <limits>
#include <cmath>
#include <iostream>
#include <crtdbg.h>
#include <string>

namespace horizon {
namespace util {

	using std::deque;
	using std::ostream;
	using std::endl;
	using std::cout;
	using std::max;
	using std::min;
	using std::string;

	using std::numeric_limits;
/**
 * Defines the Matrix and its operators.  In general Matrices are n by m arrays of doubles.
 // All elements of the matrix start indexing from 1, just like Matlab and standard matrix notation
 * @author Eric Mehiel
 */
class Matrix
{
	/// \todo TODO: Templatize the Matrix, default Matrix() should make a Matrix<double>
	/// \todo TODO: Find all implementations of Matrix.getValue(i,j) and replace with Matrix(i,j) or Matrix(i)
	// Construction
public:
	/*
	// creates a new Matrix of zero size
	*/
	Matrix(void);

	/*
	// creates a new square Matrix with an optimal initializer
	// @param size the size of the square matrix, n by n
	// @param val the value to set all elements of the matrix, zero by deafult
	*/
	Matrix(const unsigned size, const double& val = 0.0);

	/*
	// creates a new Matrix of size 'nrows x ncols' with optional initializer
	// @param nrows the number of rows in the matrix
	// @param ncols the number of columns in the matrix
	// @param val the value of each element of the matrix, zero by deafult
	*/
	Matrix(const unsigned nrows, const unsigned ncols, const double& val = 0.0);

	/*
	// creates a new Matrix of size 'nrows x ncols' from a pointer to an array
	// @param nrows the number of rows in the matrix
	// @param ncols the number of columns in the matrix
	// @param val the pointed to a one dimensional arry of doubles.  Used to populate the matrix.
	//			  The matrix data in the array should be stored in rows
	*/
	Matrix(const unsigned nrows, const unsigned ncols, double *vals);

	/*
	// copy constructor
	// @param inMatrix the reference to the matrix to be copied
	*/
	Matrix(const Matrix & inMatrix);

	/*
	// used by submatrix, colon operator, and () operator - 1D vector
	// @param inMatrix the reference to the matrix to be copied
	// @param start the first element that goes into the submatrix
	// @param end the last element that goes into the submatrix
	*/
	Matrix(const Matrix & inMatrix, unsigned start, unsigned end);

	/*
	// used by submatrix, colon operator, and () operator - 2D matrix
	// @param inMatrix the reference to the matrix to be copied
	// @param is the initial row of the new submatrix
	// @param ie the final row of the new submatrix
	// @param js the initial column of the new submatrix
	// @param je the final column of the new submatrix
	*/
	Matrix(const Matrix & inMatrix, unsigned is, unsigned ie, unsigned js, unsigned je);


	/*
	// Default Destructor
	*/
	~Matrix(void);

	// Variables
private:
	// A deque containing the elements of the matrix stored in rows
	deque<double> m_Data;

	// The number of rows in the matrix
	unsigned m_nRows;

	// The number of colums in the matrix
	unsigned m_nCols;

	// functions
public:

	/*
	// Defines the equal operator.  Sets one matrix equal to other
	// @param other the matrix used to set the LHS
	*/
	const Matrix & operator= ( const Matrix & other );

	/*
	// Gets the value of an element in the matrix
	// @param i the row number of the element to get
	// @param j the column number of the element to get
	// @return the i, j element
	*/
    double getValue ( const unsigned i, const unsigned j ) const;

	/*
	// Sets the value of an element in the matrix
	// @param i the row number of the element to get
	// @param j the column number of the element to get
	// @param value the new value
	*/
	void setValue ( const unsigned i, const unsigned j, const double value );
	
	/*
	// Gets all the data contained in the matrix as a deque
	// @return a deque containing the matrix data
	*/
	deque<double> getData ( void ) const;

	/*
	// Sets all the data in the Matrix to a new deque
	// @param inData the new set of matrix data
	*/
	void setData (const deque<double> inData);

	/*
	// Gets the number of rows in the matrix
	// @retrun the number of rows in the matrix
	*/
	unsigned getNumRows ( void ) const;

	/*
	// Gets the number of colums in the matrix
	// @retrun the number of columns in the matrix
	*/
	unsigned getNumCols ( void ) const;

	/*
	// Sets the number of rows in the matrix
	// @param r the new number of rows
	*/
	void setNumRows ( const unsigned r );

	/*
	// Sets the number of columns in the matrix
	// @param c the new number of columns
	*/
	void setNumCols ( const unsigned c );

	/*
	// Gets the number of elements in the matrix
	// @retrun the number of elements in the matrix
	*/
	unsigned getNumElems( void ) const;

	/*
	// Tests if the matrix is square
	// @return a boolean indicating whether on not the matrix is square
	*/
	bool isSquare( void ) const;

	/*
	// Gets the numbre of rows and colums in the matrix by passing references
	// @param nrows a reference to the number of rows in the matrix
	// @param ncols a reference to the number of columns in the matrix
	*/
	void getSize ( unsigned &nrows, unsigned &ncols ) const;

	/* Gets a column from a matrix
	// @param col the number of the column to return
	// @return the column as a new matrix of size n by 1
	*/
	Matrix getColumn(unsigned col) const;

	/* Sets a column of a matrix
	// @param col the number of the column to change
	// @param the new column as a new matrix of size n by 1
	*/
	void setColumn ( unsigned col, Matrix colData );

	/* Gets a row from a matrix
	// @param row the number of the row to return
	// @return the row as a new matrix of size 1 by n
	*/
	Matrix getRow( unsigned row) const;
	
	/* Sets a row of a matrix
	// @param row the number of the row to change
	// @param the new row as a new matrix of size 1 by n
	*/
	void setRow( unsigned row, const Matrix & rowData);

	/*
	// Gets a submatrix of a matrix using the matrixIndex object
	// @param ridx the row matrix index object used to store which rows to take
	// @param cidx the column matrix index object used to store which columns to take
	// @return the submatrix indicated in ridx and cidx as a new matrix
	*/
	Matrix getSubMatrix( const matrixIndex &ridx, const matrixIndex &cidx ) const;

	/*
	// Sets a submatrix of a matrix using the matrixIndex object
	// @param ridx the row matrix index object used to store which rows to take
	// @param cidx the column matrix index object used to store which columns to take
	// @param the submatrix used to set the new data in the matrix
	*/
	void setSubMatrix( const matrixIndex &ridx, const matrixIndex &cidx, Matrix subMatrix );

	/*
	// the () operator for getting a submatrix of a matrix when the matrix is one dimensional
	// @param idx the matrixIndex object containing the indices to get
	// @return a submatrix of the matrix
	*/
	Matrix operator() (const matrixIndex &idx) const {
		return (Matrix(*this, idx.STARTIDX(), idx.ENDIDX()));
	};

	/*
	// the () operator for getting a submatrix of a matrix when the matrix is two dimensional
	// @param ridx the matrixIndex object containing the row indices to get
	// @param cidx the matrixIndex object containing the column indices to get
	// @return a submatrix of the matrix
	*/
	Matrix operator() (const matrixIndex &ridx, const matrixIndex &cidx) const{
		return (Matrix(*this, ridx.STARTIDX(), ridx.ENDIDX(), cidx.STARTIDX(), cidx.ENDIDX()));
	};

	double Matrix::operator () (const unsigned i) const;

	double Matrix::operator() ( const unsigned i, const unsigned j ) const;

	// Friend Functions
public:

	/*
	// the output standard stream operator
	// @param ostrm the LHS output stream
	// @param A the matrix to send to the stream
	// @return the new stream
	*/
	friend ostream & operator<< ( ostream& ostrm, const Matrix & A);

	/*
	// Standard Matrix addition
	*/
	friend Matrix operator+ ( const Matrix &A, const Matrix &B );
	/*
	// Addition of a Matrix and a scalar.  Add the scalar to each element in the matrix
	*/
	friend Matrix operator+ ( const Matrix &A, const double &a );
	/*
	// Addition of a Matrix and a scalar.  Add the scalar to each element in the matrix
	*/
	friend Matrix operator+ ( const double &a, const Matrix &A );

	/*
	// Standard Matrix subtraction
	*/
	friend Matrix operator- ( const Matrix &A, const Matrix &B );
	/*
	// subtraction of a Matrix and a scalar.  Subtract the scalar from each element in the matrix
	*/
	friend Matrix operator- ( const Matrix &A, const double &a );
	/*
	// subtraction of a Matrix and a scalar.  Subtract the scalar from each element in the matrix
	*/
	friend Matrix operator- ( const double &a, const Matrix &A );
	/*
	// The unary subtraction operator
	*/
	friend Matrix operator- ( const Matrix & A );

	/*
	// Standard Matrix multiplication
	*/
	friend Matrix operator* ( const Matrix &A, const Matrix &B );
	/*
	// Multiplication of a matrix and a scalar
	*/
	friend Matrix operator* ( const Matrix &A, const double &a );
	/*
	// Multiplication of a matrix and a scalar
	*/
	friend Matrix operator* ( const double &a, const Matrix &A );

	/*
	// Standard Matrix division or matrix inversion
	*/
	friend Matrix operator/ ( const Matrix &A, const Matrix &B );
	/*
	// Divide each element of a matrix by a scalra
	*/
	friend Matrix operator/ ( const Matrix &A, const double &a );
//	friend Matrix operator/ ( const double &a, const Matrix &A );

	/*
	// Tests if two matrices are equal element by element
	*/
	friend bool operator== ( const Matrix &A, const Matrix &B );
	/*
	// Tests if each element in an matrix is equal to a scalar
	// @return a matrix containing a 1 if the element is equal to the scalar
	//           and a 0 if the elements are not equal
	*/
	friend Matrix operator== ( const Matrix &A, const double a );

	/*
	// Tests if two matrices are not equal element by element
	*/
	friend bool operator!= ( const Matrix &A, const Matrix &B );
	/*
	// Tests if each element in an matrix is not equal to a scalar
	// @return a matrix containing a 1 if the element is not equal to the scalar
	//           and a 0 if the elements are equal
	*/
	friend Matrix operator!= ( const Matrix &A, const double a );

	/*
	// Finds the trace, or sum of the diagonal elements of a matrix
	*/
    friend double trace( const Matrix &A );

	/*
	// Returns the transpose of a matrix - The matrix itself is NOT trasposed
	*/
    friend Matrix transpose( const Matrix &A );

	/*
	// Returns an identity matrix that's size by size
	*/
	friend Matrix meye(const unsigned size );
	/*
	// vertically concatinates two matrices and returns the result as a new matrix
	*/
	friend Matrix vertcat (const Matrix &A, const Matrix &B);
	/*
	// vertically concatinates the matrix B to the Matrix A inplace
	*/
	friend void vertcat2 (Matrix &A, const Matrix &B);
	/*
	// hoizontally concatinates two matrices and returns the result as a new matrix
	*/
	friend Matrix horzcat(const Matrix &A, const Matrix &B);
	/*
	// hoizontally concatinates the matrix B to the Matrix A inplace
	*/
	friend void horzcat2 (Matrix &A, const Matrix &B);

	/*
	//calculates the norm of a matrix - only works for 1D matrices
	*/
	friend double norm (const Matrix &A);

	/*
	// Returns a 1xn matrix with the maximum element of each column of the input matrix
	*/
	friend Matrix mmax (const Matrix &A);
	/*
	// returns a matrix containing the maximum element of A and B element by element
	*/
	friend Matrix mmax (const Matrix &A, const Matrix &B);
	/*
	// Returns a matrix containing the maximum of each element and a
	*/
	friend Matrix mmax (const Matrix &A, const double a);
	/*
	// Returns a matrix containing the maximum of each element and a
	*/
	friend Matrix mmax (const double a, const Matrix &A);

	/*
	// Returns a 1xn matrix with the minimum element of each column of the input matrix
	*/
	friend Matrix mmin (const Matrix &A);
	/*
	// returns a matrix containing the minimum element of A and B element by element
	*/
	friend Matrix mmin (const Matrix &A, const Matrix &B);
	/*
	// Returns a matrix containing the minimum of each element and a
	*/
	friend Matrix mmin (const Matrix &A, const double a);
	/*
	// Returns a matrix containing the minimum of each element and a
	*/
	friend Matrix mmin (const double a, const Matrix &A);

	/*
	// Retruns the absolute value of all the elements of the matrix as a new matrix
	*/
	friend Matrix mabs (const Matrix &A);
	/*
	// Returns the inverse of a matrix - Uses the Gauss-Jordan method
	*/
	friend Matrix inv (const Matrix &A);
	/*
	// converts a string to a matrix.  The string must look like the matrix constructor
	// e.g. Matrix(1, 3, {2.1, 4.3, -2.3})
	*/
	friend Matrix atom (const string &inMatrix);
};

}} // end namespace horizon::util


#endif //_MATRIX_H_
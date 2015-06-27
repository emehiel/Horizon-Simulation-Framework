#include "Matrix.h"

namespace horizon {
namespace util {

// creates a new Matrix of zero size
Matrix::Matrix(void)
{
	m_nRows = 0;
	m_nCols = 0;
}

// creates a new Matrix of size 'size' with optional initializer
Matrix::Matrix(const unsigned size, const double& val):
m_nRows(size),
m_nCols(size)
{
	unsigned i;

	for (i = 0; i < size*size; i++)
		this->m_Data.push_back(val);

}

// creates a new Matrix of size 'nrows x ncols' with optional initializer
Matrix::Matrix(const unsigned nrows, const unsigned ncols, const double& val):
m_nRows(nrows),
m_nCols(ncols)
{
	unsigned i;

	for (i = 0; i < nrows*ncols; i++)
		this->m_Data.push_back(val);
}

// creates a new Matrix of size 'nrows x ncols' from a pointer to an array
Matrix::Matrix(const unsigned nrows, const unsigned ncols, double *vals):
m_nRows(nrows),
m_nCols(ncols)
{
	unsigned i;

	for (i = 0; i < nrows*ncols; i++)
		this->m_Data.push_back(vals[i]);
}

// copy constructor
Matrix::Matrix(const Matrix & inMatrix)
{
//	cout << "copy called" << endl;
	this->m_nRows = inMatrix.m_nRows;
	this->m_nCols = inMatrix.m_nCols;
	this->m_Data = inMatrix.m_Data;
}

Matrix::Matrix(const Matrix & inMatrix, unsigned start, unsigned end)
{
	unsigned length = end - start + 1;

	if (inMatrix.m_nRows == 1)
	{
		this->m_nRows = 1;
		this->m_nCols = length;
	}
	else
	{
		this->m_nRows = length;
		this->m_nCols = 1;
	}

	deque<double> indata = inMatrix.getData();
	deque<double> Cdata = this->getData();

	Cdata.assign(indata.begin()+start-1, indata.begin()+end);
	
	this->setData(Cdata);
}

Matrix::Matrix(const Matrix & inMatrix, unsigned rs, unsigned re, unsigned cs, unsigned ce)
{
	unsigned r, c;

	this->m_nRows = re-rs+1;
	this->m_nCols = ce-cs+1;

	for (r = rs; r <= re; ++r){
		for (c = cs; c <= ce; ++c){
			this->m_Data.push_back(inMatrix(r,c));
		}
	}
}

// default destructor
Matrix::~Matrix(void)
{
};

const Matrix & Matrix::operator= ( const Matrix & other )
{
	if (this == &other)
		return *this;

	this->m_Data = other.getData();
	this->m_nCols = other.getNumCols();
	this->m_nRows = other.getNumRows();

	return *this;
}

double Matrix::getValue ( const unsigned r, const unsigned c ) const
{
	// matricies are indexed starting with one, (1) like matlab
	return this->m_Data[(r-1)*this->m_nCols + (c-1)];
}

void Matrix::setValue ( const unsigned r, const unsigned c, const double value )
{
	// matricies are indexed starting with one, (1) like matlab
	this->m_Data[(r-1)*this->m_nCols + (c-1)] = value;
}

deque<double> Matrix::getData ( void ) const
{
	return this->m_Data;
}

void Matrix::setData (deque<double> inData)
{
	this->m_Data = inData;
}

unsigned Matrix::getNumRows( void ) const
{
	return this->m_nRows;
}

unsigned Matrix::getNumCols( void ) const
{
	return this->m_nCols;
}

unsigned Matrix::getNumElems( void ) const
{
	return m_nRows * m_nCols;
}

void Matrix::setNumRows ( unsigned r )
{
	this->m_nRows = r;
}

void Matrix::setNumCols ( unsigned c )
{
	this->m_nCols = c;
}

bool Matrix::isSquare( void ) const
{
	return (m_nRows == m_nCols);
}

void Matrix::getSize ( unsigned &nrows, unsigned &ncols ) const
{
	nrows = m_nRows;
	ncols = m_nCols;
}

Matrix Matrix::getColumn(unsigned col) const
{
	// matrices are indexed starting with ONE like Matlab

	unsigned nRows = this->m_nRows;
	Matrix C(nRows, 1, 0.0);

	for (unsigned row = 1; row <= nRows; row++)
		C.setValue(row, 1, this->getValue(row, col));

	return C;
}

void Matrix::setColumn( unsigned col, Matrix colData )
{
	// matrices are indexed starting with ONE like Matlab

	for (unsigned row = 1; row <= this->m_nRows; row++)
		this->setValue(row, col, colData.getValue(row, 1));

}

Matrix Matrix::getRow( unsigned row) const
{
	// matrices are indexed starting with ONE like Matlab

	unsigned nCols = this->m_nCols;
	Matrix C(1, nCols);

	for (unsigned col = 1; col <= nCols; col++)
		C.setValue(1, col, this->getValue(row, col));

	return C;
}

void Matrix::setRow( unsigned row, const Matrix & rowData)
{
	// matrices are indexed starting with ONE like Matlab

	for (unsigned col = 1; col <= this->m_nCols; col++)
		this->setValue(row, col, rowData.getValue(1, col));
}

Matrix Matrix::getSubMatrix( const matrixIndex &ridx, const matrixIndex &cidx ) const
{
	return (*this)(ridx, cidx);
}

void Matrix::setSubMatrix( const matrixIndex &ridx, const matrixIndex &cidx, Matrix subMatrix )
{
	// matrices are indexed starting with ONE like Matlab

	int nRows = ridx.ENDIDX() - ridx.STARTIDX() + 1;
	int nCols = cidx.ENDIDX() - cidx.STARTIDX() + 1;

	for (int col = 1; col <= nCols; col++){
		for (int row = 1; row <= nRows; row++)
			this->setValue(ridx.STARTIDX()+row-1, cidx.STARTIDX()+col-1, subMatrix.getValue(row, col));
	}
}

double Matrix::operator() (const unsigned i) const
{
	if (this->m_nCols > this->m_nRows)
		return this->getValue(1, i);
	else
		return this->getValue(i, 1);
}


double Matrix::operator() ( const unsigned i, const unsigned j ) const
{
	// matricies are indexed starting with one, (1) like matlab
	return this->getValue(i, j);
}

/*
Matrix Matrix::operator / ( Matrix rhs )
{
       // i.e., when A = B*C then B = A*inv(C) or B = A/C where C = rhs
       // since these are 3x3's, we'll use the co-factor inverse method

       if (rhs.getDeterminate() == 0.0)
          return rhs;

       Matrix B;
       Matrix invC;
       invC = rhs.inverse();

	   B = this->matrixMultiply(invC);

       return B;
}
*/

/*
int _matherr( struct _exception *except )
{
    // Handle _OVERFLOW errors for log or log10.
	cout << except->type << endl;
    if( except->type == _OVERFLOW )
    {
        except->retval = log( 3.4 );
        printf( "Special: using absolute value: %s: _DOMAIN "
                    "error\n", except->name );
        return 1;
    }
    else
    {
        printf( "Normal: " );
        return 0;    /* Else use the default actions
    }
}
*/

ostream & operator<< ( ostream &os, const Matrix &m )
{
	unsigned r, c;
	unsigned nrows = m.getNumRows();
	unsigned ncols = m.getNumCols();

	deque<double> data = m.getData();

	for (r = 0; r < nrows; ++r) {
		for (c = 0; c < ncols; ++c)
			os << data.at(r*ncols+c) << "\t";

//		os << endl;
	}

	return os;
}


Matrix operator+ ( const Matrix &A, const Matrix &B )
{
	// C = A + B

	_ASSERTE((A.m_nCols == B.m_nCols) && (A.m_nRows == B.m_nRows));

	Matrix C(A.getNumRows(), A.getNumCols(), 0.0);

	deque<double> firstData = A.getData();
	deque<double>::iterator firstIt = firstData.begin();
	deque<double>::iterator lastIt = firstData.end();

	deque<double> secondData = B.getData();
	deque<double>::iterator secondIt = secondData.begin();

	deque<double> destData = C.getData();
	deque<double>::iterator destIt = destData.begin();

	for ( ; firstIt != lastIt; ++firstIt, ++secondIt, ++destIt)
		*destIt = *firstIt + *secondIt;

	C.setData(destData);

	return C;
}


Matrix operator+ ( const Matrix &A, const double &a )
{
	// C = A + a - element wise, i.e. add a to each element of A

	Matrix C(A.getNumRows(), A.getNumCols(), 0.0);

	deque<double> firstData = A.getData();
	deque<double>::iterator firstIt = firstData.begin();
	deque<double>::iterator lastIt = firstData.end();

	deque<double> destData = C.getData();
	deque<double>::iterator destIt = destData.begin();

	for ( ; firstIt != lastIt; ++firstIt, ++destIt)
		*destIt = *firstIt + a;

	C.setData(destData);

	return C;
}


Matrix operator+ ( const double &a, const Matrix &A )
{
	// C = a + A - element wise, i.e. add a to each element of A

	Matrix C(A.getNumRows(), A.getNumCols(), 0.0);

	deque<double> firstData = A.getData();
	deque<double>::iterator firstIt = firstData.begin();
	deque<double>::iterator lastIt = firstData.end();

	deque<double> destData = C.getData();
	deque<double>::iterator destIt = destData.begin();

	for ( ; firstIt != lastIt; ++firstIt, ++destIt)
		*destIt = *firstIt + a;

	C.setData(destData);

	return C;
}


Matrix operator- ( const Matrix &A, const Matrix &B )
{
	// C = A - B

	_ASSERTE((A.m_nCols == B.m_nCols) && (A.m_nRows == B.m_nRows));

	Matrix C(A.getNumRows(), A.getNumCols(), 0.0);

	deque<double> firstData = A.getData();
	deque<double>::iterator firstIt = firstData.begin();
	deque<double>::iterator lastIt = firstData.end();

	deque<double> secondData = B.getData();
	deque<double>::iterator secondIt = secondData.begin();

	deque<double> destData = C.getData();
	deque<double>::iterator destIt = destData.begin();

	for ( ; firstIt != lastIt; ++firstIt, ++secondIt, ++destIt)
		*destIt = *firstIt - *secondIt;

	C.setData(destData);

	return C;
}


Matrix operator- ( const Matrix &A, const double &a )
{
		// C = A - a - element wise, i.e. subtract a from each element of A

	Matrix C(A.getNumRows(), A.getNumCols(), 0.0);

	deque<double> firstData = A.getData();
	deque<double>::iterator firstIt = firstData.begin();
	deque<double>::iterator lastIt = firstData.end();

	deque<double> destData = C.getData();
	deque<double>::iterator destIt = destData.begin();

	for ( ; firstIt != lastIt; ++firstIt, ++destIt)
		*destIt = *firstIt - a;

	C.setData(destData);

	return C;
}


Matrix operator- ( const double &a, const Matrix &A )
{
	// C = a - A - element wise, i.e. subtract a from each element of A

	Matrix C(A.getNumRows(), A.getNumCols(), 0.0);

	deque<double> firstData = A.getData();
	deque<double>::iterator firstIt = firstData.begin();
	deque<double>::iterator lastIt = firstData.end();

	deque<double> destData = C.getData();
	deque<double>::iterator destIt = destData.begin();

	for ( ; firstIt != lastIt; ++firstIt, ++destIt)
		*destIt = a - *firstIt;

	C.setData(destData);

	return C;
}


Matrix operator- ( const Matrix & A )
{
	// C = -A - element wise, i.e. negate each element of A

	Matrix C(A.getNumRows(), A.getNumCols(), 0.0);

	deque<double> firstData = A.getData();
	deque<double>::iterator firstIt = firstData.begin();
	deque<double>::iterator lastIt = firstData.end();

	deque<double> destData = C.getData();
	deque<double>::iterator destIt = destData.begin();

	for ( ; firstIt != lastIt; ++firstIt, ++destIt)
		*destIt = -(*firstIt);

	C.setData(destData);

	return C;
}


Matrix operator* ( const Matrix &A, const Matrix &B )
{
	_ASSERTE((A.m_nCols == B.m_nRows));

	unsigned nrows = A.getNumRows();
	unsigned ncols = B.getNumCols();
	unsigned inDim = B.getNumRows();
	unsigned r, c, k;
	double temp;

	Matrix C(nrows, ncols, 0.0);

	for (r = 1; r <= nrows; ++r){
		for (c = 1; c <= ncols; ++c){
			temp =  0.0;
			for (k = 1; k <= inDim; ++k)
				temp += A.getValue(r, k)*B.getValue(k, c);
			C.setValue(r, c, temp);
		}
	}

	return C;
}


Matrix operator* ( const Matrix &A, const double &a )
{
	// C = A*a - multiply A by a element wise

	Matrix C(A.getNumRows(), A.getNumCols(), 0.0);

	deque<double> firstData = A.getData();
	deque<double>::iterator firstIt = firstData.begin();
	deque<double>::iterator lastIt = firstData.end();

	deque<double> destData = C.getData();
	deque<double>::iterator destIt = destData.begin();

	for ( ; firstIt != lastIt; ++firstIt, ++destIt)
		*destIt = a*(*firstIt);

	C.setData(destData);

	return C;
}


Matrix operator* ( const double &a, const Matrix &A )
{
	// C = a*A - multiply A by a element wise

	Matrix C(A.getNumRows(), A.getNumCols(), 0.0);

	deque<double> firstData = A.getData();
	deque<double>::iterator firstIt = firstData.begin();
	deque<double>::iterator lastIt = firstData.end();

	deque<double> destData = C.getData();
	deque<double>::iterator destIt = destData.begin();

	for ( ; firstIt != lastIt; ++firstIt, ++destIt)
		*destIt = a*(*firstIt);

	C.setData(destData);

	return C;
}


Matrix operator/ ( const Matrix &A, const Matrix &B )
{
	// C = A/B - divide each element of A by B element wise

//	_ASSERTE((A.m_nCols == B.m_nRows));

	Matrix C(A.getNumRows(), A.getNumCols(), 0.0);

	if (B.getNumCols() == 1 && B.getNumRows() == 1)
	{
		_ASSERTE(B.getValue(1,1) != 0);
		C = A/B(1);
	}
	else
	{
		deque<double> firstData = A.getData();
		deque<double>::iterator firstIt = firstData.begin();
		deque<double>::iterator lastIt = firstData.end();

		deque<double> secondData = B.getData();
		deque<double>::iterator secondIt = secondData.begin();

		deque<double> destData = C.getData();
		deque<double>::iterator destIt = destData.begin();

		for ( ; firstIt != lastIt; ++firstIt, ++secondIt, ++destIt){
			_ASSERTE( (*secondIt) != 0.0);
			*destIt = (*firstIt)/(*secondIt);
		}

		C.setData(destData);
	}

	return C;

}


Matrix operator/ ( const Matrix &A, const double &a )
{
	// C = A/a - divide A by a element wise
	
	_ASSERTE(a != 0);

	Matrix C(A.getNumRows(), A.getNumCols(), 0.0);

	deque<double> firstData = A.getData();
	deque<double>::iterator firstIt = firstData.begin();
	deque<double>::iterator lastIt = firstData.end();

	deque<double> destData = C.getData();
	deque<double>::iterator destIt = destData.begin();

	for ( ; firstIt != lastIt; ++firstIt, ++destIt){
		*destIt = (*firstIt)/a;
		if ((*destIt == numeric_limits<double>::infinity( )) || (*destIt == -1*numeric_limits<double>::infinity( )))
			cout << "infinity" << endl;
	}

	C.setData(destData);

	return C;
}

/*

Matrix operator/ ( const double &a, const Matrix &A )
{
	// C = A/a - divide A by a element wise

	Matrix C(A.getNumRows(), A.getNumCols(), 0.0);

	transform (A.m_Data.begin( ), A.m_Data.end( ), C.m_Data.begin( ), multValue<double>( 1.0/a ) );

	return C;
}
*/

bool operator== ( const Matrix &A, const Matrix &B )
{
	// A == B

	bool eq = true;

	// Check that size(A) == size(B) if not, return false
	if (A.m_nCols != B.m_nCols || A.m_nRows != B.m_nRows)
		return false;

	deque<double> firstData = A.getData();
	deque<double>::iterator firstIt = firstData.begin();
	deque<double>::iterator lastIt = firstData.end();

	deque<double> secondData = B.getData();
	deque<double>::iterator secondIt = secondData.begin();

	double eps = numeric_limits<double>::epsilon( );

	// Check that all elements of A and B are equal within the eps limit
	for ( ; firstIt != lastIt; ++firstIt, ++secondIt){
		if (fabs(*firstIt - *secondIt) > 10.0*eps)
			return false;
	}

	return eq;
}



Matrix operator== ( const Matrix &A, const double a )
{
	// A == a - element wise

	const unsigned nrows = A.getNumRows();
	const unsigned ncols = A.getNumCols();

	Matrix C(nrows, ncols, 0.0);

	deque<double> firstData = A.getData();
	deque<double>::iterator firstIt = firstData.begin();
	deque<double>::iterator lastIt = firstData.end();

	deque<double> destData = C.getData();
	deque<double>::iterator destIt = destData.begin();

	double eps = numeric_limits<double>::epsilon( );

	for ( ; firstIt != lastIt; ++firstIt, destIt++){
		if (fabs(*firstIt - a) > eps)
			*destIt = 0;
		else
			*destIt = 1;
	}

	C.setData(destData);

	return C;
}


bool operator!= ( const Matrix &A, const Matrix &B )
{
	// A != B

	const unsigned nrows = A.getNumRows();
	const unsigned ncols = A.getNumCols();
	bool eq = false;

	if (ncols != B.getNumCols() || nrows != B.getNumRows())
		return true;

	deque<double> firstData = A.getData();
	deque<double>::iterator firstIt = firstData.begin();
	deque<double>::iterator lastIt = firstData.end();

	deque<double> secondData = B.getData();
	deque<double>::iterator secondIt = secondData.begin();

	double eps = numeric_limits<double>::epsilon( );

	for ( ; firstIt != lastIt; ++firstIt, ++secondIt)
	{
		if (fabs(*firstIt - *secondIt) > eps)
			return true;
	}

	return eq;
}


Matrix operator!= ( const Matrix &A, const double a )
{
	// A != B - element wise

	const unsigned nrows = A.getNumRows();
	const unsigned ncols = A.getNumCols();

	Matrix C(nrows, ncols, 0.0);

	deque<double> firstData = A.getData();
	deque<double>::iterator firstIt = firstData.begin();
	deque<double>::iterator lastIt = firstData.end();

	deque<double> destData = C.getData();
	deque<double>::iterator destIt = destData.begin();

	double eps = numeric_limits<double>::epsilon( );

	for ( ; firstIt != lastIt; ++firstIt, destIt++){
		if (fabs(*firstIt - a) < eps)
			*destIt = 0;
		else
			*destIt = 1;
	}

	C.setData(destData);

	return C;
}


double trace( const Matrix &A )
{
	_ASSERTE(A.m_nCols == A.m_nRows);

	double t = 0.0;
	unsigned int i;
	unsigned ncols = A.getNumCols();

	for (i = 1; i <= ncols; i++)
		t = t + A.getValue(i,i);

	return t;
}


Matrix transpose( const Matrix &A )
{
	const unsigned nrows = A.getNumRows();
	const unsigned ncols = A.getNumCols();
	unsigned r, c;

	Matrix C(ncols, nrows, 0.0);

	for (r = 1; r <= ncols; ++r){
		for (c = 1; c <= nrows; ++c)
			C.setValue(r, c, A.getValue(c,r));
	}

	return C;
}


Matrix meye(const unsigned size )
{
	Matrix C(size);
	unsigned i;

	for (i = 1; i <= size; i++)
		C.setValue(i,i,1.0);

	return C;
}

Matrix vertcat (const Matrix &A, const Matrix &B)
{

	// vertical concatination "C = [A; B]"
	_ASSERTE(A.m_nCols == B.m_nCols);

	Matrix C(A.getNumRows()+B.getNumRows(), A.getNumCols(),  0.0);
	
	deque<double> Cdata = A.getData();
	deque<double> Bdata = B.getData();

	Cdata.insert(Cdata.end(), Bdata.begin(), Bdata.end());

	C.setData(Cdata);

	return C;
}


void vertcat2 (Matrix &A, const Matrix &B)
{
	// vertical concatination "A = [A; B]" i.e., A is OVERWRITTEN!!
	_ASSERTE(A.m_nCols == B.m_nCols);

	A.setNumRows(A.getNumRows() + B.getNumRows());

	deque<double> Adata = A.getData();
	deque<double> Bdata = B.getData();

	Adata.insert(Adata.end(), Bdata.begin(), Bdata.end());

	A.setData(Adata);
}


Matrix horzcat(const Matrix &A, const Matrix &B)
{
	// horizontal concatination "C = [A B]"
	_ASSERTE(A.m_nRows == B.m_nRows);

	return transpose(vertcat(transpose(A),transpose(B)));
}


void horzcat2 (Matrix &A, const Matrix &B)
{
	// horizontal concatination "C = [A; B]"
	_ASSERTE(A.m_nRows == B.m_nRows);

	A = transpose(vertcat(transpose(A), transpose(B)));
}


double norm (const Matrix &A)
{
	// FOR VECTORS ONLY

	_ASSERTE( (A.m_nCols == 1) || (A.m_nRows ==1) );

	deque<double> firstData = A.getData();
	deque<double>::iterator firstIt = firstData.begin();
	deque<double>::iterator lastIt = firstData.end();

	double temp = inner_product(firstIt, lastIt, firstIt, 0.0);

	return sqrt(temp);
}

Matrix mmax (const Matrix &A)
{
	Matrix C(1, A.m_nCols, 0.0);

	if (A.m_nCols == 1 || A.m_nRows == 1) {
		// find the max of a VECTOR and return as type double

		deque<double> firstData = A.getData();
		deque<double>::iterator firstIt = firstData.begin();
		deque<double>::iterator lastIt = firstData.end();

		C.setValue(1 ,1 , *(max_element(firstIt, lastIt)));
	}
	else {
		// find the max of each COLUMN of A and return a VECTOR of the MAX elements
		for (unsigned i = 1; i <= A.m_nCols; i++) {
			C.setValue(1,i, mmax(A.getColumn(i))(1)); 
		}
	}
	return C;
}


Matrix mmax (const Matrix  &A, const Matrix &B)
{
	_ASSERTE( (A.m_nRows == B.m_nRows) && (A.m_nCols == B.m_nCols) );

	Matrix C(A.getNumRows(), A.getNumCols(), 0.0);

	deque<double> firstData = A.getData();
	deque<double>::iterator firstIt = firstData.begin();
	deque<double>::iterator lastIt = firstData.end();

	deque<double> secondData = B.getData();
	deque<double>::iterator secondIt = secondData.begin();

	deque<double> destData = C.getData();
	deque<double>::iterator destIt = destData.begin();

	for ( ; firstIt != lastIt; ++firstIt, ++secondIt, ++destIt)
		*destIt = max((*firstIt), (*secondIt));

	C.setData(destData);

	return C;
}


Matrix mmax (const Matrix &A, const double a)
{
	// find the max of A and a and return a MATRIX the same size as A
	
	Matrix C(A.getNumRows(), A.getNumCols(), 0.0);

	deque<double> firstData = A.getData();
	deque<double>::iterator firstIt = firstData.begin();
	deque<double>::iterator lastIt = firstData.end();

	deque<double> destData = C.getData();
	deque<double>::iterator destIt = destData.begin();

	for ( ; firstIt != lastIt; ++firstIt, ++destIt)
		*destIt = max((*firstIt), a);

	C.setData(destData);

	return C;

}

Matrix mmax (const double a, const Matrix &A)
{
	// find the max of A and a and return a MATRIX the same size as A
	
	Matrix C(A.getNumRows(), A.getNumCols(), 0.0);

	deque<double> firstData = A.getData();
	deque<double>::iterator firstIt = firstData.begin();
	deque<double>::iterator lastIt = firstData.end();

	deque<double> destData = C.getData();
	deque<double>::iterator destIt = destData.begin();

	for ( ; firstIt != lastIt; ++firstIt, ++destIt)
		*destIt = max((*firstIt), a);

	C.setData(destData);

	return C;

}

Matrix mmin (const Matrix &A)
{
	Matrix C(1, A.m_nCols, 0.0);

	if (A.m_nCols == 1 || A.m_nRows == 1) {
		// find the min of a VECTOR and return as type double

		deque<double> firstData = A.getData();
		deque<double>::iterator firstIt = firstData.begin();
		deque<double>::iterator lastIt = firstData.end();

		C.setValue(1 ,1 , *(min_element(firstIt, lastIt)));
	}
	else {
		// find the min of each COLUMN of A and return a VECTOR of the MAX elements
		for (unsigned i = 1; i <= A.m_nCols; i++) {
			C.setValue(1,i, mmin(A.getColumn(i))(1)); 
		}
	}
		return C;
}


Matrix mmin (const Matrix  &A, const Matrix &B)
{
	_ASSERTE( (A.m_nRows == B.m_nRows) && (A.m_nCols == B.m_nCols) );

	Matrix C(A.getNumRows(), A.getNumCols(), 0.0);

	deque<double> firstData = A.getData();
	deque<double>::iterator firstIt = firstData.begin();
	deque<double>::iterator lastIt = firstData.end();

	deque<double> secondData = B.getData();
	deque<double>::iterator secondIt = secondData.begin();

	deque<double> destData = C.getData();
	deque<double>::iterator destIt = destData.begin();

	for ( ; firstIt != lastIt; ++firstIt, ++secondIt, ++destIt)
		*destIt = min((*firstIt), (*secondIt));

	C.setData(destData);

	return C;
}


Matrix mmin (const Matrix &A, const double a)
{
	// find the min of A and a and return a MATRIX the same size as A
	
	Matrix C(A.getNumRows(), A.getNumCols(), 0.0);

	deque<double> firstData = A.getData();
	deque<double>::iterator firstIt = firstData.begin();
	deque<double>::iterator lastIt = firstData.end();

	deque<double> destData = C.getData();
	deque<double>::iterator destIt = destData.begin();

	for ( ; firstIt != lastIt; ++firstIt, ++destIt)
		*destIt = min((*firstIt), a);

	C.setData(destData);

	return C;

}

Matrix mmin (const double a, const Matrix &A)
{
	// find the min of A and a and return a MATRIX the same size as A
	
	Matrix C(A.getNumRows(), A.getNumCols(), 0.0);

	deque<double> firstData = A.getData();
	deque<double>::iterator firstIt = firstData.begin();
	deque<double>::iterator lastIt = firstData.end();

	deque<double> destData = C.getData();
	deque<double>::iterator destIt = destData.begin();

	for ( ; firstIt != lastIt; ++firstIt, ++destIt)
		*destIt = min((*firstIt), a);

	C.setData(destData);

	return C;

}

Matrix mabs (const Matrix &A)
{
	// returns the absolute value of each element of the Matrix A
	// as a Matrix of the same size
	
	Matrix C(A.getNumRows(), A.getNumCols(), 0.0);

	deque<double> firstData = A.getData();
	deque<double>::iterator firstIt = firstData.begin();
	deque<double>::iterator lastIt = firstData.end();

	deque<double> destData = C.getData();
	deque<double>::iterator destIt = destData.begin();

	for ( ; firstIt != lastIt; ++firstIt, ++destIt)
		*destIt = fabs(*firstIt);

	C.setData(destData);

	return C;

}



Matrix inv (const Matrix &A)
{
	// same as inv(A) - solves for inverse via the Gauss-Jordan method

	_ASSERT(A.isSquare());

	//  Do we want to use Dolittle's method?
	unsigned nrows = A.getNumRows();
	unsigned ncols = A.getNumCols();
	unsigned r = 1;
	unsigned c = 1;

	Matrix invMatrix = meye(nrows);

	if ( nrows != ncols ){
		cout << "Matrix must be square!" << endl;
		return invMatrix;
	}

	unsigned n = ncols;

	// concatinate A and invMatrix
	Matrix Ab = horzcat(A, invMatrix);
	Matrix L = meye(n);
	Matrix U(n);

	deque<double> wmData = Ab.getData();
	deque<double>::iterator wmfirstIt = wmData.begin();
	deque<double>::iterator wmlastIt = wmData.end();

	double eps = numeric_limits<double>::epsilon( );

	// if A(1,1) ~= 0, swap rows
	while (( fabs(Ab(r,1)) < eps ) && (r < n)){
		r++;
	}

	if ( r != 1 ){ // swap rows
		Matrix rowOne = Ab(1, colon(1,2*n));
		Ab.setRow(1, Ab(r, colon(1,2*n)));
		Ab.setRow(r, rowOne);
	}

	// if A(1,1) < 0, multiply first row by -1
	if ( Ab(1,1) < 0.0 )
		Ab.setRow(1, -Ab(1, colon(1,2*n)));

	for ( c = 1; c <= n-1; c++){
		for ( r = c+1; r <= n; r++){
			_ASSERTE(abs(Ab(c,c)) > eps); 
			L.setValue(r, c, Ab(r,c)/Ab(c,c));
			Ab.setRow(r, Ab(r,colon(1,2*n)) - L(r,c)*Ab(c, colon(1, 2*n)));
		}
	}

	U = Ab(colon(1,n), colon(1,n));

	for ( c = n; c >= 1; c--){
		_ASSERTE(abs(Ab(c,c)) > eps);
		Ab.setRow(c, Ab(c,colon(1,2*n))/Ab(c,c));
		for ( r = c-1; r >= 1; r--)
			Ab.setRow(r, Ab(r, colon(1,2*n)) - Ab(r,c)*Ab(c, colon(1,2*n)));
	}

	invMatrix = Ab(colon(1, n), colon(n+1, 2*n));

	return invMatrix;
}

Matrix atom (const string &inMatrix)
{
	size_t a = inMatrix.find("(")+1;
	size_t b = inMatrix.find(",",a);
	size_t c = inMatrix.find(",",b+1);
	const unsigned int nrows = atoi(inMatrix.substr(a, b-a).c_str());
	const unsigned int ncols = atoi(inMatrix.substr(b+1, c-b-1).c_str());
	Matrix C(nrows, ncols, 0.0);

	size_t d = inMatrix.find("{",c+1)+1;
	for (unsigned r = 1; r <= ncols; r++){
		for (unsigned c = 1; r <= nrows; r++){
			size_t e = inMatrix.find(",", d+1);
			if (e == string::npos)
				e = inMatrix.size()-2;

			double data = atof(inMatrix.substr(d, e-d).c_str());
			C.setValue(r,c, data); 
			d = e+1;
		}
	}

//	cout << C;
	return C;
}
}} // end namespace horizon::util
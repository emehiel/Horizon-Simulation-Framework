#ifndef MATRIXINDEX_H
#define MATRIXINDEX_H

namespace horizon {
namespace util {


class matrixIndex
{
public:

//  matrixIndex(void) : m_StartIdx(0), m_EndIdx(max?) { }
/**
*  Creates a set of indices that contains one element only (equivalent to
* (idx) in Matlab).
* @param idx the index of the desired element.
*/
matrixIndex(unsigned idx):
m_StartIdx(idx),
m_EndIdx(idx)
{
}
/**
* Creates a set of indices in the specified range (equivalent to
* (start:end) in Matlab).
* @param start is the first index in the desired range
* @param end is the last index in the desired range
*/
matrixIndex(unsigned start, unsigned end):
m_StartIdx(start),
m_EndIdx(end)
{
}
//! Destructor; does nothing but free memory
~matrixIndex(void) { /* nothing to do */ }

unsigned STARTIDX() const {return this->m_StartIdx;}; 
unsigned ENDIDX() const {return this->m_EndIdx;}; 

//friend class Matrix<double>;
//friend class Matrix<float>;
//friend class Matrix<unsigned char>;

private:
  unsigned m_StartIdx;
  unsigned m_EndIdx;

};

//CMatrixIndex colon(void);

matrixIndex colon(const unsigned idx);

matrixIndex colon(const unsigned start, const unsigned end);

	}}


#endif // _MATRIXINDEX_H_
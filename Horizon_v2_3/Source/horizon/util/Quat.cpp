#include "Quat.h"
#include "umath.h"
#include <cmath>

namespace horizon {
namespace util {

using std::sin;
using std::cos;
using horizon::util::umath::cross;
using horizon::util::umath::dot;

Quat::Quat() {
	// set the valarray size
	q_.valarray<double>::valarray(4);
	// set values for null quaternion
	q_[0] = 0;
	q_[1] = 0;
	q_[2] = 0;
	q_[3] = 0;
}

Quat::Quat(const double& q0) {
	// set the valarray size
	q_.valarray<double>::valarray(4);
	// set values for quaternion without imaginary component
	q_[0] = q0;
	q_[1] = 0;
	q_[2] = 0;
	q_[3] = 0;
}

Quat::Quat(const double& q1, const double& q2, const double& q3) {
	// set the valarray size
	q_.valarray<double>::valarray(4);
	// set values for pure quaternion
	q_[0] = 0;
	q_[1] = q1;
	q_[2] = q2;
	q_[3] = q3;
}

Quat::Quat(const double& q0, const double& q1,
		const double& q2, const double& q3) {
	// set the valarray size
	q_.valarray<double>::valarray(4);
	// set quaternion values
	q_[0] = q0;
	q_[1] = q1;
	q_[2] = q2;
	q_[3] = q3;
}

Quat::Quat(const valarray<double>& vals) {
	// set the valarray size
	q_.valarray<double>::valarray(4);
	// if size is one then assume no imaginary component
	if (vals.size() == 1) {
		q_[0] = vals[0];
		q_[1] = 0;
		q_[2] = 0;
		q_[3] = 0;
	// if size is three then assume it's a pure quaternion
	} else if (vals.size() == 3) {
		q_[0] = 0;
		q_[1] = vals[0];
		q_[2] = vals[1];
		q_[3] = vals[2];
	} else if (vals.size() == 4)
		q_ = vals;
}

Quat::Quat(const Matrix& v, const double& theta) {
	// set the valarray size
	q_.valarray<double>::valarray(4);
	// find the values of the quaternion that would rotate the
	// vector through angle theta
	// from Kuipers, Pp. 130
	double norm = util::norm(v);
	q_[0] = cos(theta/2);
	q_[1] = sin(theta/2) * v.getValue(1,1) / norm;
	q_[2] = sin(theta/2) * v.getValue(2,1) / norm;
	q_[3] = sin(theta/2) * v.getValue(3,1) / norm;
}

Quat::Quat(const Matrix& c, const Matrix& d) {
	// set the valarray size
	q_.valarray<double>::valarray(4);
	// find the cross product of the two vectors, around which the rotation will occur
	Matrix v = cross(c, d);
	// find the necessary angle to rotate vector c to vector d
	double theta = acos(dot(c, d) / util::norm(c) / util::norm(d));
	// find the values of the quaternion that would rotate the
	// vector through angle theta
	// from Kuipers, Pp. 130
	double norm = util::norm(v);
	q_[0] = cos(theta/2);
	q_[1] = sin(theta/2) * v.getValue(1,1) / norm;
	q_[2] = sin(theta/2) * v.getValue(2,1) / norm;
	q_[3] = sin(theta/2) * v.getValue(3,1) / norm;
}



Quat::~Quat() {}



double Quat::real() const {
	return q_[0];
}

valarray<double> Quat::imag() const {
	valarray<double> temp(3);	// will store the imaginary component of the quaternion
	temp[0] = q_[1];
	temp[1] = q_[2];
	temp[2] = q_[3];
	return temp;
}

double Quat::comp_q0() const {
	return q_[0];
}

double Quat::comp_q1() const {
	return q_[1];
}

double Quat::comp_q2() const {
	return q_[2];
}

double Quat::comp_q3() const {
	return q_[3];
}

valarray<double> Quat::comp_array() const {
	return q_;
}

void Quat::print() const {
	cout << q_[0] << " + "
		 << q_[1] << "i + "
		 << q_[2] << "j + "
		 << q_[3] << "k\n";
}

void Quat::set_q0(const double& q0) {
	q_[0] = q0;
}

void Quat::set_q1(const double& q1) {
	q_[1] = q1;
}

void Quat::set_q2(const double& q2) {
	q_[2] = q2;
}

void Quat::set_q3(const double& q3) {
	q_[3] = q3;
}


Quat Quat::conjugate() const {
	return Quat(q_[0], -q_[1], -q_[2], -q_[3]);
}

double Quat::norm() const {
	// the norm is equal to the squareroot of the sum of the resulting
	// valarray after element by element multiplication of the quaternion
	// with itself
	return sqrt((q_ * q_).sum());
}

Quat & Quat::normalize() {
	Quat temp;
	temp.q_[0] = q_[0] / this->norm();
	temp.q_[1] = q_[1] / this->norm();
	temp.q_[2] = q_[2] / this->norm();
	temp.q_[3] = q_[3] / this->norm();
	*this = temp;
	return *this;
}

Quat atoq (const string &inQuat)
{
	size_t a = inQuat.find("(")+1;
	size_t b = inQuat.find(",",a);
	const unsigned int whichCtr = atoi(inQuat.substr(a, b-a).c_str());

	if(whichCtr != 1 && whichCtr != 3 && whichCtr != 4)
		return Quat();

	double data[4] = {0.0, 0.0, 0.0, 0.0};

	size_t offset = b+1;
	offset = inQuat.find("{",offset)+1;
	for (unsigned i = 1; i <= whichCtr; i++){
		size_t dataEnd = inQuat.find(",", offset+1);
		if (dataEnd == string::npos)
			dataEnd = inQuat.size()-2;

		string cData = inQuat.substr(offset, dataEnd-offset);
		data[i-1] = atof(cData.c_str());
		offset = dataEnd+1;
	}
	Quat ret;
	switch(whichCtr) {
		case 1:
			ret = Quat(data[0]);
			break;
		case 3:
			ret = Quat(data[0], data[1], data[2]);
			break;
		case 4:
			ret = Quat(data[0], data[1], data[2], data[3]);
			break;
		default:
			ret = Quat();
			break;
	}
	return ret;
}

Quat & Quat::operator= (double *vals) {
	q_.valarray<double>::valarray(4);
	for (int i = 0; i < 4; i++)
		q_[i] = vals[i];
	return *this;
}

Quat & Quat::operator+= (const Quat& rhs) {
	q_ = q_ + rhs.q_;
	return *this;
}

Quat & Quat::operator-= (const Quat& rhs) {
	q_ = q_ - rhs.q_;
	return *this;
}

Quat & Quat::operator*= (const Quat& rhs) {
	*this = *this * rhs;
	return *this;
}



ostream & operator<< (ostream& ostrm, const Quat& q) {
	ostrm << q.q_[0] << " + "
		  << q.q_[1] << "i + "
		  << q.q_[2] << "j + "
		  << q.q_[3] << "k";
	return ostrm;
}



Quat operator+ (const Quat& q) {
	return q;
}

Quat operator+ (const Quat& p, const Quat& q) {
	return Quat(p.q_+q.q_);
}

Quat operator+ (const double& k, const Quat& q) {
	return Quat(q.q_[0] + k, q.q_[1], q.q_[2], q.q_[3]);
}

Quat operator+ (const Quat& q, const double& k) {
	return Quat(q.q_[0] + k, q.q_[1], q.q_[2], q.q_[3]);
}

Quat operator- (const Quat& q) {
	return Quat(-q.q_);
}

Quat operator- (const Quat& p, const Quat& q) {
	return Quat(p.q_ + q.q_);
}

Quat operator- (const double& k, const Quat& q) {
	return Quat(k - q.q_[0], -q.q_[1], -q.q_[2], -q.q_[3]);
}

Quat operator- (const Quat& q, const double& k) {
	return Quat(q.q_[0] - k, q.q_[1], q.q_[2], q.q_[3]);
}

Quat operator* (const double& k, const Quat& q) {
	return Quat(k * q.q_);
}

Quat operator* (const Quat& q, const double& k) {
	return Quat(k * q.q_);
}

Quat operator* (const Quat& p, const Quat& q) {
	// from Kuipers, Pp. 107
	return Quat(p.q_[0] * q.q_[0] - p.q_[1] * q.q_[1] -
				p.q_[2] * q.q_[2] - p.q_[3] * q.q_[3],
				p.q_[0] * q.q_[1] + p.q_[1] * q.q_[0] + 
				p.q_[2] * q.q_[3] - p.q_[3] * q.q_[2],
				p.q_[0] * q.q_[2] - p.q_[1] * q.q_[3] + 
				p.q_[2] * q.q_[0] + p.q_[3] * q.q_[1],
				p.q_[0] * q.q_[3] + p.q_[1] * q.q_[2] - 
				p.q_[2] * q.q_[1] + p.q_[3] * q.q_[0]);
}



bool operator== (const Quat& p, const Quat& q) {
	return (p.q_[0] == q.q_[0] &&
			p.q_[1] == q.q_[1] &&
			p.q_[2] == q.q_[2] &&
			p.q_[3] == q.q_[3]);
}

bool operator!= (const Quat& p, const Quat& q ) {
	return !(p == q);
}

}} // end namespace horizon::util
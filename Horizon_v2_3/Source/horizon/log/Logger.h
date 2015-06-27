#ifndef LOGGER_H
#define LOGGER_H

#include "horizon/util/Profile.h"
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>

namespace horizon {
	namespace log {

using namespace std;

struct errData {
	int number;
	double val;

	errData() : number(0), val(0) {}
};

class Logger
{

// Do not edit code below this line
public:
	static Logger* Instance();
	
	static void Report(string error, double value = 0);

	static void ReportGenericConstraint(string varType, string failType, int value);
	static void ReportGenericConstraint(string varType, string failType, float value);
	static void ReportGenericConstraint(string varType, string failType, double value);

	static void write(string filename);
	static void writeConstraint(string filename);

protected:
	Logger();
	Logger(const Logger&);
	Logger& operator= (const Logger&);
private:
	static void ReportGenericConstraint(string key);

	static Logger* pinstance;
	static map<string, errData> subFailures;
	static map<string, errData> genericConstraintFailures;
};
}} // end namespace horizon::log

#endif // LOGGER_H
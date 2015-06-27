#include "Logger.h"

namespace horizon {
	namespace log {

// Called by failed subsystem
void Logger::Report(string error, double value) {
	errData* data;
	map<string, errData>::iterator mapPos;
	if(subFailures.find(error) == subFailures.end()) {
		subFailures.insert(make_pair(error, errData()));
	}
	data = &subFailures[error];
	data->number++;
}

void Logger::ReportGenericConstraint(string varType, string failType, int value) {
	stringstream ss;
	ss << varType << "," << failType << "," << value;
	ReportGenericConstraint(ss.str());
}

void Logger::ReportGenericConstraint(string varType, string failType, float value) {
	stringstream ss;
	ss << varType << "," << failType << "," << value;
	ReportGenericConstraint(ss.str());
}

void Logger::ReportGenericConstraint(string varType, string failType, double value) {
	stringstream ss;
	ss << varType << "," << failType << "," << value;
	ReportGenericConstraint(ss.str());
}

void Logger::ReportGenericConstraint(string key) {
	errData* data;
	map<string, errData>::iterator mapPos;
	if(genericConstraintFailures.find(key) == genericConstraintFailures.end()) {
		genericConstraintFailures.insert(make_pair(key, errData()));
	}
	data = &genericConstraintFailures[key];
	data->number++;
}

void Logger::write(string filename) {
	ofstream fout;
	fout.open(filename.c_str());
	fout << "Subsystem Log Failures" << endl;
	map<string, errData>::iterator itr;
	for(itr = subFailures.begin(); itr != subFailures.end(); itr++) {
		fout << itr->first << " " << itr->second.number << endl;
	}
	fout.close();
}

void Logger::writeConstraint(string filename) {
	ofstream fout;
	fout.open(filename.c_str());
	fout << "Constraint Log Failures" << endl;
	map<string, errData>::iterator itr;
	for(itr = genericConstraintFailures.begin(); itr != genericConstraintFailures.end(); itr++) {
		fout << itr->first << endl;
	}
	fout.close();
}

// Do not edit code below here
Logger* Logger::pinstance = 0;// initialize pointer
map<string, errData> Logger::subFailures = map<string, errData>();
map<string, errData> Logger::genericConstraintFailures = map<string, errData>();
Logger* Logger::Instance() {
	if (pinstance == 0) {  // is it the first call?
		pinstance = new Logger; // create sole instance
    }
    return pinstance; // address of sole instance
}

Logger::Logger() {
    //... perform necessary instance initializations 
}

}} // end namespace horizon::log
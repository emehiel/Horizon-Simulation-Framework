#ifndef CLOUDCOVER_H
#define CLOUDCOVER_H

#include "math.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "simParams.h"

#define NUM_REGIONS 14
#define NUM_DAYS 30

using std::vector;
using std::string;

class CloudCover 
{
public:
	static CloudCover* Instance();

	double data[NUM_DAYS][NUM_REGIONS];

	vector<string> regions;

	vector<double> days;

	bool importCloudDataFromTextFile(std::string filename);

protected:
	CloudCover();
	CloudCover(const CloudCover&);

private:
	static CloudCover* pinstance;
};

#endif
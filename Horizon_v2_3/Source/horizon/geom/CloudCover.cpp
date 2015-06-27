#include "CloudCover.h"

CloudCover* CloudCover::pinstance = 0;// initialize pointer

CloudCover* CloudCover::Instance () 
{
	if (pinstance == 0)  // is it the first call?
	{  
		pinstance = new CloudCover; // create sole instance
	}
	return pinstance; // address of sole instance
}

CloudCover::CloudCover() 
{ 
std::cout << "Initializing Cloud Coverage Data... " << std::endl;
	double init = 0.0;

	for(int i = 0; i < NUM_DAYS; i++) {
		for(int j = 0; j < NUM_REGIONS; j++) {			
			data[i][j] = init;
		}
	}

//... perform necessary instance initializations 
}

bool CloudCover::importCloudDataFromTextFile(std::string filename) {
	std::ifstream fin (filename.c_str());
	
	string temps;
	double tempd;
	int i = 0;
	int j = 0;

	if (fin.is_open()) {

		fin >> temps; // read past fisrt string
		for(j = 0; j < NUM_REGIONS; j++){
			fin >> temps;
			regions.push_back(temps);
			}

		for(i = 0; i < NUM_DAYS; i++){
			fin >> tempd;
			days.push_back(tempd);
			for(j = 0; j < NUM_REGIONS; j++) {
				fin >> data[i][j];
			}
		}
		fin.close();
		return true;
	}
	return false;
}
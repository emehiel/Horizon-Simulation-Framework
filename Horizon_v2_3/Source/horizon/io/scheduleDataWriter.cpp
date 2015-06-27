#include "scheduleDataWriter.h"

namespace horizon {
	namespace io {

scheduleDataWriter::scheduleDataWriter(const string& outputDir, bool shouldOutputGoToCommandWindow, size_t numOutput) : 
		dirOut(outputDir), cwOut(shouldOutputGoToCommandWindow), relOut(outputDir), nSched(numOutput) 
{
	// Start timer
	tickerStart = GetTickCount();
	// Finds the string location of the directory to write to
	string outputFolderName;
	char buffer[32];
	struct tm newtime;
	__time32_t aclock;
	errno_t errNum;
	_time32( &aclock );   // Get time in seconds.
	_localtime32_s( &newtime, &aclock );   // Convert time to struct tm form.
	// Print local time as a string.
	errNum = asctime_s(buffer, 32, &newtime);
	outputFolderName = buffer;
	replace(outputFolderName.begin(), outputFolderName.end(), ':', '.');
	outputFolderName.erase(outputFolderName.end()-1);
	char* wd = _getcwd(NULL, 100);
	dirOut += "\\" + outputFolderName;
	cout << endl << "Output directory set to: " << dirOut << endl;
	relOut = dirOut;
	dirOut = string(wd) + "\\" + dirOut;
	simParams::setOutputDir(dirOut);
	_mkdir(dirOut.c_str());
}

scheduleDataWriter::~scheduleDataWriter() {}

bool scheduleDataWriter::writeAll(const list<systemSchedule*> schedules, System* system)
{
	// Timer stops
	tickerEnd = GetTickCount();
	runTime = ((double)(tickerEnd-tickerStart))/1000;
	cout << endl << endl << "Simulation run time: " << runTime << " seconds." << endl;

	// Debug printouts
	cout << endl << "Final number of schedules generated: " << schedules.size();
	
	// If user wants to print more schedules than there are, just print all the schedules
	size_t schedulesToShow = schedules.size() < nSched ? schedules.size() : nSched;

	// Prints the whole final list of schedules to the command window
	if(cwOut) {
		size_t counter = 1;
		for (list<systemSchedule*>::const_iterator sIt = schedules.begin(); sIt != schedules.end(); sIt++, counter++)
			cout << endl << "Schedule " << counter << " - Value: " << (*sIt)->getScheduleValue();
		cout << endl << endl;
	}

	// Creates a time-stamped folder for this simulation to put its output data in
	_mkdir(dirOut.c_str());

	// Iterates through the schedules being outputted
	list<systemSchedule*>::const_iterator sIt = schedules.begin();
	for(size_t i = 1; i <= schedulesToShow; i++, sIt++) {
		stringstream iin;
		iin << i; 
		// Creates a new folder inside the time-stamped simulation folder to store the data from each schedule kept
		string tempDirOut = dirOut + "\\Schedule " + iin.str();
		_mkdir(tempDirOut.c_str());
		// Iterate through assetSchedules and write target data
		const vector<assetSchedule*> assetscheds = (*sIt)->getAssetScheds();
		const vector<const Asset*>& assets = (*system).getAssets();

		vector<assetSchedule*>::const_iterator assSchIt = assetscheds.begin();
		vector<const Asset*>::const_iterator assIt = assets.begin();
		for(size_t j = 1; assSchIt != assetscheds.end(), assIt != assets.end(); j++, assSchIt++, assIt++)
		{			
			stringstream assetname;
			assetname << "Asset" << j;
			writeTargetData((*assSchIt)->getEvents(), tempDirOut.c_str(), assetname.str().c_str());
			// TODO:  Change the 30.0 second interval to the sim step size by default, or allow for input
			writeAssetPositionData((*assIt), 30.0, tempDirOut.c_str(), assetname.str().c_str());
			writeAssetVelocityData((*assIt), 30.0, tempDirOut.c_str(), assetname.str().c_str());
			writeStateData((*assSchIt)->getEvents(), system, tempDirOut.c_str(), assetname.str().c_str());
		}
		// initialize xmlDataWriter and write data
		xmlDataWriter xmlOut;		
		stringstream xmlFilename;
		xmlFilename << tempDirOut.c_str() << " data.xml";
		xmlOut.writeSchedule(*sIt, system, xmlFilename.str());
	}
	return true;
}

void scheduleDataWriter::writeTargetData(const vector<shared_ptr<Event>>& events, string schedulePath, string assetName)
{
		// Output file path
		string outputFileName = schedulePath + "\\" + assetName + "_TargetData.txt";
		fstream fs(outputFileName.c_str(), ios::in | ios::out | ios::trunc);
		if (!fs.bad())
		{
			fs << setiosflags( ios::fixed );
			fs << setprecision( 10 );
			// Iterate from the first to the last event, sends the data 
			vector<shared_ptr<Event>>::const_iterator vIt;
			fs << "TargetName";
			for(vIt = events.begin(); vIt != events.end(); vIt++)
				fs << "\t" << (*vIt)->getTask()->getTarget()->getName();
			fs << endl << "TargetType";
			for(vIt = events.begin(); vIt != events.end(); vIt++)
				fs << "\t" << (*vIt)->getTask()->getTarget()->getType();
			fs << endl << "EventStartTime";
			for(vIt = events.begin(); vIt != events.end(); vIt++)
				fs << "\t" << (*vIt)->getState()->getEventStart();
			fs << endl << "TaskStartTime";
			for(vIt = events.begin(); vIt != events.end(); vIt++)
				fs << "\t" << (*vIt)->getState()->getTaskStart();
			fs << endl << "TaskEndTime";
			for(vIt = events.begin(); vIt != events.end(); vIt++)
				fs << "\t" << (*vIt)->getState()->getTaskEnd();
			fs << endl << "EventEndTime";
			for(vIt = events.begin(); vIt != events.end(); vIt++)
				fs << "\t" << (*vIt)->getState()->getEventEnd();
			fs << endl << "TargetLat";	
			for(vIt = events.begin(); vIt != events.end(); vIt++)
				fs << "\t" << (*vIt)->getTask()->getTarget()->getPos()->getPosLLA((*vIt)->getState()->getTaskStart()).getValue(1,1);
			fs << endl << "TargetLon";
			for(vIt = events.begin(); vIt != events.end(); vIt++)
				fs << "\t" << (*vIt)->getTask()->getTarget()->getPos()->getPosLLA((*vIt)->getState()->getTaskStart()).getValue(2,1);		
		}
		fs.close();
}

void scheduleDataWriter::writeAssetPositionData(const Asset* asset, double stepSize, string schedulePath, string assetName)
{
		// Output file path
		string outputFileName = schedulePath + "\\" + assetName + "_PositionData.txt";
		fstream fs(outputFileName.c_str(), ios::in | ios::out | ios::trunc);
		if (!fs.bad())
		{
			fs << setiosflags( ios::fixed );
			fs << setprecision( 10 );
			double endTime = simParams::SIMEND_SECONDS();
			fs << "Time";
			for(double i = 0; i< endTime; i +=stepSize)
			{
				fs << "\t" << i;
			}
			fs << endl << "AssetPosX";
			for(double i = 0; i< endTime; i +=stepSize)
			{
				fs << "\t" << asset->getPos()->getPosECI(i).getValue(1,1);
			}
			fs << endl << "AssetPosY";
			for(double i = 0; i< endTime; i +=stepSize)
			{
				fs << "\t" << asset->getPos()->getPosECI(i).getValue(2,1);
			}
			fs << endl << "AssetPosZ";
			for(double i = 0; i< endTime; i +=stepSize)
			{
				fs << "\t" << asset->getPos()->getPosECI(i).getValue(3,1);
			}
		}
		fs.close();
}

void scheduleDataWriter::writeAssetVelocityData(const Asset* asset, double stepSize, string schedulePath, string assetName)
{
		// Output file path
		string outputFileName = schedulePath + "\\" + assetName + "_VelocityData.txt";
		fstream fs(outputFileName.c_str(), ios::in | ios::out | ios::trunc);
		if (!fs.bad())
		{
			fs << setiosflags( ios::fixed );
			fs << setprecision( 10 );
			double endTime = simParams::SIMEND_SECONDS();
			fs << "Time";
			for(double i = 0; i< endTime; i +=stepSize)
			{
				fs << "\t" << i;
			}
			fs << endl << "AssetVelX";
			for(double i = 0; i< endTime; i +=stepSize)
			{
				fs << "\t" << asset->getPos()->getVelECI(i).getValue(1,1);
			}
			fs << endl << "AssetVelY";
			for(double i = 0; i< endTime; i +=stepSize)
			{
				fs << "\t" << asset->getPos()->getVelECI(i).getValue(2,1);
			}
			fs << endl << "AssetVelZ";
			for(double i = 0; i< endTime; i +=stepSize)
			{
				fs << "\t" << asset->getPos()->getVelECI(i).getValue(3,1);
			}
		}
		fs.close();
}

void scheduleDataWriter::writeStateData(const vector<shared_ptr<Event>>& events, System* system, string schedulePath, string assetName)
{
	if(!events.empty()) 
	{
		// Get the last state for the list of Events
		const State* lastState = events.back()->getState();
		// Get the list of SubsystemNodes to iterate through
		const vector<SubsystemNode*>& subSystemNodes = system->getSubsystemNodes();
		
		// Iterate through each SubsystemNode and write out its data
		for(vector<SubsystemNode*>::const_iterator subNodeIt = subSystemNodes.begin(); subNodeIt != subSystemNodes.end(); subNodeIt++)
		{
			// Get the Subsystem corresponding to the SubsystemNode
			const Subsystem* subsystem = (*subNodeIt)->getSubsystem();
			// Get all of the StateVarKeys from the subsystem
			vector< StateVarKey<int> >		ikeys = subsystem->getIkeys();
			vector< StateVarKey<double> >	dkeys = subsystem->getDkeys();
			vector< StateVarKey<float> >	fkeys = subsystem->getFkeys();
			vector< StateVarKey<bool> >		bkeys = subsystem->getBkeys();
			vector< StateVarKey<Matrix> >	mkeys = subsystem->getMkeys();
			vector< StateVarKey<Quat> >		qkeys = subsystem->getQkeys();

			string outputFileName = schedulePath + "\\" + assetName + "_" + subsystem->getName() + "StateData.txt";
			fstream fs(outputFileName.c_str(), ios::in | ios::out | ios::trunc);
			if (!fs.bad())
			{
				fs << setiosflags( ios::fixed );
				fs << setprecision( 10 );

				// Iterate through all the vectors of StateVarKeys, get the FullProfile for the corresponding State data and output it
				if(!ikeys.empty())
				{
					for(vector<StateVarKey<int>>::const_iterator keyIt = ikeys.begin(); keyIt != ikeys.end(); keyIt++)
					{
						Profile<int> prof = Profile<int>(lastState->getFullProfile(*keyIt));
						if(prof.size() != 0) {
							fs << "Time";
							for(map<double, int>::const_iterator profIt = prof.begin(); profIt != prof.end(); profIt++)
							{
								fs << "\t" << (*profIt).first;
							}
							fs << endl << keyIt->getNameString();
							for(map<double, int>::const_iterator profIt = prof.begin(); profIt != prof.end(); profIt++)
							{
								fs << "\t" << (*profIt).second;
							}
							fs << endl;
						}
					}
				}
				if(!dkeys.empty())
				{
					for(vector<StateVarKey<double>>::const_iterator keyIt = dkeys.begin(); keyIt != dkeys.end(); keyIt++)
					{				
						Profile<double> prof = Profile<double>(lastState->getFullProfile(*keyIt));
						if(prof.size() != 0) {
							fs << "Time";
							for(map<double, double>::const_iterator profIt = prof.begin(); profIt != prof.end(); profIt++)
							{
								fs << "\t" << (*profIt).first;
							}
							fs << endl << keyIt->getNameString();
							for(map<double, double>::const_iterator profIt = prof.begin(); profIt != prof.end(); profIt++)
							{
								fs << "\t" << (*profIt).second;
							}
							fs << endl;
						}
					}
				}
				if(!fkeys.empty())
				{
					for(vector<StateVarKey<float>>::const_iterator keyIt = fkeys.begin(); keyIt != fkeys.end(); keyIt++)
					{
						// Output Data					
						Profile<float> prof = Profile<float>(lastState->getFullProfile(*keyIt));
						if(prof.size() != 0) {
							fs << "Time";
							for(map<double, float>::const_iterator profIt = prof.begin(); profIt != prof.end(); profIt++)
							{
								fs << "\t" << (*profIt).first;
							}
							fs << endl << keyIt->getNameString();
							for(map<double, float>::const_iterator profIt = prof.begin(); profIt != prof.end(); profIt++)
							{
								fs << "\t" << (*profIt).second;
							}
							fs << endl;
						}
					}
				}
				if(!bkeys.empty())
				{
					for(vector<StateVarKey<bool>>::const_iterator keyIt = bkeys.begin(); keyIt != bkeys.end(); keyIt++)
					{				
						Profile<bool> prof = Profile<bool>(lastState->getFullProfile(*keyIt));
						if(prof.size() != 0) {
							fs << "Time";
							for(map<double, bool>::const_iterator profIt = prof.begin(); profIt != prof.end(); profIt++)
							{
								fs << "\t" << (*profIt).first;
							}
							fs << endl << keyIt->getNameString();
							for(map<double, bool>::const_iterator profIt = prof.begin(); profIt != prof.end(); profIt++)
							{
								fs << "\t" << (*profIt).second;
							}
							fs << endl;
						}
					}
				}
				if(!mkeys.empty())
				{
					for(vector<StateVarKey<Matrix>>::const_iterator keyIt = mkeys.begin(); keyIt != mkeys.end(); keyIt++)
					{				
						Profile<Matrix> prof = Profile<Matrix>(lastState->getFullProfile(*keyIt));
						if(prof.size() != 0) {
							vector< deque<double> > matdata;
							size_t cols;

							fs << "Time";
							for(map<double, Matrix>::const_iterator profIt = prof.begin(); profIt != prof.end(); profIt++)
							{
								fs << "\t" << (*profIt).first;
								matdata.push_back((*profIt).second.getData());
								cols = (*profIt).second.getNumCols();
							}
							size_t length = matdata.at(1).size();

							for(size_t i = 0; i < length; i++)
							{
								fs << endl << keyIt->getNameString() << "(" << i/cols + 1 << "," << i%cols + 1<< ")";
								for(vector< deque<double> >::const_iterator matIt = matdata.begin(); matIt != matdata.end(); matIt++)
								{
									fs << "\t" << (*matIt).at(i);
								}
							}
							fs << endl;
						}
					}
				}
				if(!qkeys.empty())
				{
					for(vector<StateVarKey<Quat>>::const_iterator keyIt = qkeys.begin(); keyIt != qkeys.end(); keyIt++)
					{				
						Profile<Quat> prof = Profile<Quat>(lastState->getFullProfile(*keyIt));
						if(prof.size() != 0) {
							fs << "Time";
							for(map<double, Quat>::const_iterator profIt = prof.begin(); profIt != prof.end(); profIt++)
							{
								fs << "\t" << (*profIt).first;
							}
							fs << endl << keyIt->getNameString() << "_q1";
							for(map<double, Quat>::const_iterator profIt = prof.begin(); profIt != prof.end(); profIt++)
							{
								fs << "\t" << (*profIt).second.comp_q0();
							}
							fs << endl << keyIt->getNameString() << "_q2";
							for(map<double, Quat>::const_iterator profIt = prof.begin(); profIt != prof.end(); profIt++)
							{
								fs << "\t" << (*profIt).second.comp_q1();
							}
							fs << endl << keyIt->getNameString() << "_q3";
							for(map<double, Quat>::const_iterator profIt = prof.begin(); profIt != prof.end(); profIt++)
							{
								fs << "\t" << (*profIt).second.comp_q2();
							}
							fs << endl << keyIt->getNameString() << "_q4";
							for(map<double, Quat>::const_iterator profIt = prof.begin(); profIt != prof.end(); profIt++)
							{
								fs << "\t" << (*profIt).second.comp_q3();
							}
							fs << endl;
						}
					}
				}
			}
			fs.close();
		}
	}
}

}} // end namespace horizon::io
#include "Position.h"

namespace horizon {
namespace geom {

Position::Position(const string& type, const double& data1, const double& data2, const double& data3) : posType(type), eomsObject(0)
{
	_ASSERTE((type == POSITION_TYPE_STATIC_LLA) || (type == POSITION_TYPE_STATIC_ECI));
	double posa[3] = {data1, data2, data3};
	Matrix pos(3,1,posa);
	posData.insert(pair<double, Matrix> (0.0, pos));
}

Position::Position(const string& type, const Matrix& pos) : posType(type), eomsObject(0)
{	
	_ASSERTE((type == POSITION_TYPE_STATIC_LLA) || (type == POSITION_TYPE_STATIC_ECI));
	posData.insert(pair<double, Matrix> (0.0, pos));
}

Position::Position(const string& type, EOMS* equationsOfMotion, const double& ic1, const double& ic2, 
				   const double& ic3, const double& ic4, const double& ic5, 
				   const double& ic6) : posType(type), eomsObject(equationsOfMotion), posDataTimeStep(30.0)
{
	_ASSERTE((type != POSITION_TYPE_STATIC_LLA) && (type != POSITION_TYPE_STATIC_ECI));
	double ics[6] = {ic1, ic2, ic3, ic4, ic5, ic6};
	Matrix initialConditions(6,1,ics);
	posData.insert(pair<double, Matrix> (0.0, initialConditions));
}

Position::Position(const string& type, EOMS* equationsOfMotion, const Matrix& initialConditions) : posType(type), 
eomsObject(equationsOfMotion) , posDataTimeStep(30.0)
{
	_ASSERTE((type != POSITION_TYPE_STATIC_LLA) && (type != POSITION_TYPE_STATIC_ECI));
	posData.insert(pair<double, Matrix> (0.0, initialConditions));
}

Position::~Position() { delete eomsObject; }

void Position::setPosDataTimeStep(double step) { posDataTimeStep = step; }
const string Position::getType() {
	return posType; }

EOMS* Position::getEOMS()
{
	_ASSERTE((posType != POSITION_TYPE_STATIC_LLA) && (posType != POSITION_TYPE_STATIC_ECI));
	return eomsObject;
}

const Matrix Position::getIC()
{
	_ASSERTE((posType == POSITION_TYPE_DYNAMIC_LLA) || (posType == POSITION_TYPE_DYNAMIC_ECI));
	return posData[0];
}

const Matrix Position::getStateECI(const double& simTime)
{
	_ASSERTE((posType != POSITION_TYPE_DYNAMIC_LLA) && (posType != POSITION_TYPE_DYNAMIC_ECI));
	_ASSERTE((simTime >= 0) && (simTime <= simParams::SIMEND_SECONDS()));
	Matrix initState = posData[0];
	double JD = simTime/86400.0 + simParams::SIMSTART_JD();
	bool hasrun = !(posData.size()==1);

	if(posType == POSITION_TYPE_STATIC_LLA)
		return LLA2ECI(initState, JD);
	else if(posType == POSITION_TYPE_STATIC_ECI)
		return initState;
	else if(posType == POSITION_TYPE_PREDETERMINED_LLA)
	{
		if(hasrun)
			return LLA2ECI((posData.lower_bound(simTime))->second, JD);
		else
		{
			cout << "Integrating and resampling position data... " << endl;
			double vals[2] = {0, simParams::SIMEND_SECONDS()};
			Integrator solver;
			vector<double> resampletimes;
			for(double t = vals[0]; t<= vals[1]; t += posDataTimeStep)
				resampletimes.push_back(t);
			// TODO:  Pass resampletimes to solver
			posData = solver.rk45(eomsObject, Matrix(2,1,vals), initState).ODE_RESULT;
			//posData = resample_spline(solver.rk45(eomsObject, Matrix(2,1,vals), initState).ODE_RESULT, resampletimes);
			cout << "DONE!" << endl;
			return LLA2ECI((posData.lower_bound(simTime))->second, JD);
		}
	}
	else if(posType == POSITION_TYPE_PREDETERMINED_ECI)
	{
		if(hasrun) {
			return (posData.lower_bound(simTime))->second;
		}
		else
		{
			cout << "Integrating and resampling position data... " << endl;
			double vals[2] = {0, simParams::SIMEND_SECONDS()};
			Integrator solver;
			vector<double> resampletimes;
			for(double t = vals[0]; t<= vals[1]; t += posDataTimeStep)
				resampletimes.push_back(t);
			// TODO:  Pass resampletimes to solver
			posData = solver.rk45(eomsObject, Matrix(2,1,vals), initState).ODE_RESULT;
			//posData = resample_spline(solver.rk45(eomsObject, Matrix(2,1,vals), initState).ODE_RESULT, resampletimes);
			cout << "DONE!" << endl;
			return (posData.lower_bound(simTime))->second;
		}
	}
	else
		return 0;
}

const Matrix Position::getPosECI(const double& simTime) {
	return getStateECI(simTime).getSubMatrix(colon(1,3), 1);
}

const Matrix Position::getVelECI(const double& simTime) {
	return getStateECI(simTime).getSubMatrix(colon(4,6), 1);
}

const Matrix Position::getPosLLA(const double& simTime)
{
	_ASSERTE((posType != POSITION_TYPE_DYNAMIC_LLA) && (posType != POSITION_TYPE_DYNAMIC_ECI));
	_ASSERTE((simTime >= 0) && (simTime <= simParams::SIMEND_SECONDS()));
	Matrix initState = posData[0];
	double JD = simTime/86400.0 + simParams::SIMSTART_JD();
	bool hasrun = !(posData.size()==1);

	if(posType == POSITION_TYPE_STATIC_LLA)
		return initState;
	else if(posType == POSITION_TYPE_STATIC_ECI)
		return ECI2LLA(initState, JD);
	else if(posType == POSITION_TYPE_PREDETERMINED_LLA)
	{
		if(hasrun)
			return (posData.lower_bound(simTime))->second;
		else
		{
			cout << "Integrating and resampling position data... ";
			double vals[2] = {0, simParams::SIMEND_SECONDS()};
			Integrator solver;
			vector<double> resampletimes;
			for(double t = vals[0]; t<= vals[1]; t += posDataTimeStep)
				resampletimes.push_back(t);
			posData = resample_spline(solver.rk45(eomsObject, Matrix(2,1,vals), initState).ODE_RESULT, resampletimes);
			cout << "DONE!" << endl;
			return (posData.lower_bound(simTime))->second;
		}
	}
	else if(posType == POSITION_TYPE_PREDETERMINED_ECI)
	{
		if(hasrun)
			return ECI2LLA((posData.lower_bound(simTime))->second, JD);
		else
		{
			cout << "Integrating and resampling position data... ";
			double vals[2] = {0, simParams::SIMEND_SECONDS()};
			Integrator solver;
			vector<double> resampletimes;
			for(double t = vals[0]; t<= vals[1]; t += posDataTimeStep)
				resampletimes.push_back(t);
			posData = resample_spline(solver.rk45(eomsObject, Matrix(2,1,vals), initState).ODE_RESULT, resampletimes);
			cout << "DONE!" << endl;
			return ECI2LLA((posData.lower_bound(simTime))->second, JD);
		}
	}
	else
		return 0;
}

bool Position::hasLOSTo(Position* posECI, double simTime) {
	_ASSERTE((posType == POSITION_TYPE_PREDETERMINED_ECI || posType == POSITION_TYPE_STATIC_ECI || posType == POSITION_TYPE_PREDETERMINED_LLA || posType == POSITION_TYPE_STATIC_LLA) && (posECI->getType() == POSITION_TYPE_PREDETERMINED_ECI || posECI->getType() == POSITION_TYPE_STATIC_ECI || posECI->getType() == POSITION_TYPE_PREDETERMINED_LLA || posECI->getType() == POSITION_TYPE_STATIC_LLA));
	if((posType == POSITION_TYPE_PREDETERMINED_ECI || posType == POSITION_TYPE_STATIC_ECI || posType == POSITION_TYPE_PREDETERMINED_LLA || posType == POSITION_TYPE_STATIC_LLA) && (posECI->getType() == POSITION_TYPE_PREDETERMINED_ECI || posECI->getType() == POSITION_TYPE_STATIC_ECI || posECI->getType() == POSITION_TYPE_PREDETERMINED_LLA || posECI->getType() == POSITION_TYPE_STATIC_LLA))
		return hasLOS(getPosECI(simTime), posECI->getPosECI(simTime));
	else {
		cout << "Wrong Position Types for hasLOSTo(), posECI must be Predetermined or Static ECI or LLA types" << endl;
		return false;
	}
}

bool Position::hasLOSTo(Matrix posECI, double simTime) {
	_ASSERTE(posType == POSITION_TYPE_PREDETERMINED_ECI || posType == POSITION_TYPE_STATIC_ECI || posType == POSITION_TYPE_PREDETERMINED_LLA || posType == POSITION_TYPE_STATIC_LLA);
	if(posType == POSITION_TYPE_PREDETERMINED_ECI || posType == POSITION_TYPE_STATIC_ECI || posType == POSITION_TYPE_PREDETERMINED_LLA || posType == POSITION_TYPE_STATIC_LLA)
		return hasLOS(getPosECI(simTime), posECI);
	else {
		cout << "Wrong Position Type for hasLOSTo(), must be Predetermined or Static ECI or LLA types" << endl;
		return false;
	}
}

}} // end namespace horizon::geom

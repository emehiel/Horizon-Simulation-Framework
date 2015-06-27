#include "Position.h"

namespace horizon {
namespace geom {

Position::Position(XMLNode& positionXMLNode) : posType(positionXMLNode.getAttribute("PositionType"))
{
	//_ASSERTE((posType == POSITION_TYPE_STATIC_LLA) || (posType == POSITION_TYPE_STATIC_ECI));
	if((posType == POSITION_TYPE_STATIC_LLA) || (posType == POSITION_TYPE_STATIC_ECI)) {
		Matrix ics = atom(positionXMLNode.getAttribute("ICs"));
		posData.insert(pair<double, Matrix> (0.0, ics));
	}
	else {
		Matrix ics = atom(positionXMLNode.getAttribute("ICs"));
		posData.insert(pair<double, Matrix> (0.0, ics));
		eomsObject = createEOMSObject(positionXMLNode.getChildNode("EOMS"));
		int n = positionXMLNode.nChildNode("INTEGRATOR");
		if(n!=0)
			integratorNode = positionXMLNode.getChildNode("INTEGRATOR");
		else
			integratorNode = XMLNode::emptyXMLNode;
		if(positionXMLNode.isAttributeSet("PosDataStep"))
			posDataTimeStep = atof(positionXMLNode.getAttribute("PosDataStep"));
		else
			posDataTimeStep = 30.0;
	}
	this->propagatorType = RK45;
}

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
	//_ASSERTE((posType == POSITION_TYPE_DYNAMIC_LLA) || (posType == POSITION_TYPE_DYNAMIC_ECI));
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
		if(hasrun) {
			//return LLA2ECI((posData.lower_bound(simTime))->second, JD);
			return LLA2ECI(this->getStateAtTime(simTime), JD);
		}
		else
		{
			cout << "Integrating and resampling position data... " << endl;
			double vals[2] = {0, simParams::SIMEND_SECONDS()};
			Integrator solver;
			bool rk45;
			// Update the integrator parameters using the information in the XML Node
			setIntegratorParams(solver);			

			if(rk45) {
				solver.setParam("nsteps", (vals[1] - vals[0])/schedParams::SIMSTEP_SECONDS());
				posData = solver.rk45(eomsObject, Matrix(2,1,vals), initState).ODE_RESULT;
			}
			else {
				cout << "Using rk4 integrator..." << endl;
				posData = solver.rk4(eomsObject, Matrix(2,1,vals), initState).ODE_RESULT;
			}
			cout << "DONE!" << endl;
			//return LLA2ECI((posData.lower_bound(simTime))->second, JD);			
			return LLA2ECI(this->getStateAtTime(simTime), JD);
		}
	}
	else if(posType == POSITION_TYPE_PREDETERMINED_ECI)
	{
		if(hasrun) {
			//return (posData.lower_bound(simTime))->second;
			return this->getStateAtTime(simTime);
		}
		else
		{
			cout << "Integrating and resampling position data... " << endl;
			this->setPropagationModel();

			if (this->propagatorType == RK45)
			{
				double vals[2] = {0, simParams::SIMEND_SECONDS()};
				Integrator solver;

				// Update the integrator parameters using the information in the XML Node
				setIntegratorParams(solver);	

				solver.setParam("nsteps", (vals[1] - vals[0])/schedParams::SIMSTEP_SECONDS());

				cout << "Using rk45 integrator..." << endl;
				posData = solver.rk45(eomsObject, Matrix(2,1,vals), initState).ODE_RESULT;
			}
			else if (this->propagatorType == RK4)
			{
				double vals[2] = {0, simParams::SIMEND_SECONDS()};
				Integrator solver;

				// Update the integrator parameters using the information in the XML Node
				setIntegratorParams(solver);

				cout << "Using rk4 integrator..." << endl;
				posData = solver.rk4(eomsObject, Matrix(2,1,vals), initState).ODE_RESULT;
			}
			else if (this->propagatorType == SGP4)
			{
				cout << "Using sgp4 integrator..." << endl;
				// creat an sgp4 object and propagate
			}
			cout << "DONE!" << endl;
			//return (posData.lower_bound(simTime))->second;
			return this->getStateAtTime(simTime);
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
			//return (posData.lower_bound(simTime))->second;
			return this->getStateAtTime(simTime);
		else
		{
			cout << "Integrating and resampling position data... ";
			double vals[2] = {0, simParams::SIMEND_SECONDS()};
			Integrator solver;
			bool rk45;
			// Update the integrator parameters using the information in the XML Node
			setIntegratorParams(solver);//, rk45);

			if(rk45) {
				solver.setParam("nsteps", (vals[1] - vals[0])/schedParams::SIMSTEP_SECONDS());
				posData = solver.rk45(eomsObject, Matrix(2,1,vals), initState).ODE_RESULT;
			}
			else {
				cout << "Using rk4 integrator..." << endl;
				posData = solver.rk4(eomsObject, Matrix(2,1,vals), initState).ODE_RESULT;
			}
			cout << "DONE!" << endl;
			//return (posData.lower_bound(simTime))->second;
			return this->getStateAtTime(simTime);
		}
	}
	else if(posType == POSITION_TYPE_PREDETERMINED_ECI)
	{
		if(hasrun)
			//return ECI2LLA((posData.lower_bound(simTime))->second, JD);
			return ECI2LLA(this->getStateAtTime(simTime), JD);
		else
		{
			cout << "Integrating and resampling position data... ";
			double vals[2] = {0, simParams::SIMEND_SECONDS()};
			Integrator solver;

			bool rk45 = true;
			// Update the integrator parameters using the information in the XML Node
			setIntegratorParams(solver);//, rk45);

			//posData = resample_spline(solver.rk45(eomsObject, Matrix(2,1,vals), initState).ODE_RESULT, resampletimes);
			if(rk45) {
				solver.setParam("nsteps", (vals[1] - vals[0])/schedParams::SIMSTEP_SECONDS());
				posData = solver.rk45(eomsObject, Matrix(2,1,vals), initState).ODE_RESULT;
			}
			else {
				cout << "Using rk4 integrator..." << endl;
				posData = solver.rk4(eomsObject, Matrix(2,1,vals), initState).ODE_RESULT;
			}
			cout << "DONE!" << endl;
			//return ECI2LLA((posData.lower_bound(simTime))->second, JD);
			return ECI2LLA(this->getStateAtTime(simTime), JD);
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

Matrix Position::getStateAtTime(const double& simTime) {
	map< double, Matrix >::const_iterator odeResultIt;
	// TODO: Make nRows and nCols a Position State Parameter for easy access
	unsigned nRows, nCols;
	this->posData[0].getSize(nRows, nCols);
	Matrix lowerState(nRows, 1, 0.0);
	Matrix upperState(nRows, 1, 0.0);
	Matrix interp(nRows, 1, 0.0);
	Matrix slope(nRows, 1, 0.0);
	
	double lowerTime;
	double upperTime;

	odeResultIt = posData.lower_bound(simTime);

	if(odeResultIt == posData.end())  {
		std::cout << "end of map reached!" << std::endl;
		odeResultIt--;
		return odeResultIt->second.getSubMatrix(colon(1,nRows),1);
	}
	else if (odeResultIt->first == simTime) {
		return odeResultIt->second.getSubMatrix(colon(1,nRows),1);
	}
	else {
		upperState = odeResultIt->second.getSubMatrix(colon(1,nRows), 1);
		upperTime = odeResultIt->first;
		lowerState = (--odeResultIt)->second.getSubMatrix(colon(1,nRows),1);
		lowerTime = odeResultIt->first;
	}

	slope = (upperState - lowerState)/(upperTime - lowerTime);
	return slope*(simTime - lowerTime) + lowerState;
}

void Position::setIntegratorParams(Integrator& solver) {

	if(!integratorNode.isEmpty()) {
		if(integratorNode.isAttributeSet("h")) {
			double h =  atof(integratorNode.getAttribute("h"));
			solver.setParam("h", h);
		}
		if(integratorNode.isAttributeSet("rtol")) {
			double rtol =  atof(integratorNode.getAttribute("rtol"));
			solver.setParam("rtol", rtol);
		}
		if(integratorNode.isAttributeSet("atol")) {
			double atol =  atof(integratorNode.getAttribute("atol"));
			solver.setParam("atol", atol);
		}
		if(integratorNode.isAttributeSet("eps")) {
			double eps =  atof(integratorNode.getAttribute("eps"));
			solver.setParam("eps", eps);
		}
		
	}
}

void Position::setPropagationModel()
{
	if (!this->integratorNode.isEmpty())
	{
		if(integratorNode.isAttributeSet("integrator")) {
			if(_strcmpi(integratorNode.getAttribute("integrator"),"rk4")==0)
				this->propagatorType = RK4;
			else if (_strcmpi(integratorNode.getAttribute("integrator"),"rk45")==0)
				this->propagatorType = RK45;
			else if(_strcmpi(integratorNode.getAttribute("integrator"),"sgp4")==0)
				this->propagatorType = SGP4;
			else if(_strcmpi(integratorNode.getAttribute("integrator"),"trapz")==0)
				this->propagatorType = TRAPZ;
			else
				this->propagatorType = RK45;
		}
	}
}

}} // end namespace horizon::geom

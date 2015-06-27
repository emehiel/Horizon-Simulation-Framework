#ifndef POSITION_H
#define POSITION_H

#include "../util/Matrix.h"
#include "../util/eoms/EOMS.h"
#include "../util/spline/Spline.h"
#include "geomUtil.h"
#include "../util/integrate/Integrator.h"
#include "../util/integrate/Integrator.h"
#include "../../simParams.h"
#include "../../schedParams.h"
#include "horizon/util/adapt/EOMSAdapter.h"

namespace horizon {
namespace geom {

	using std::map;
	using std::string;
	using std::pair;
	using horizon::util::Matrix;
	using horizon::util::atom;
	using horizon::util::colon;
	using namespace horizon::util::eoms;
	using namespace horizon::util::integrate;
	using horizon::util::umath::interp_spline;
	using horizon::util::umath::resample_spline;
	using horizon::util::adapt::createEOMSObject;

/**
 * A class that specifies the position of a given object in the system in a given coordinate frame.
 * The two coordinate frames used are ECI and LLA. 
 
 * ECI refers to an unchanging coordinate frame which is relatively fixed with respect to the Solar 
 * System. The z axis runs along the Earth's rotational axis pointing North, the x axis points in 
 * the direction of the vernal equinox, and the y axis completes the right-handed orthogonal system.
 * Units are (Kilometers, Kilometers, Kilometers).

 * LLA refers to the geodetic latitude, longitude, and altitude above the planetary ellipsoid. Units
 * are (Radians, Radians, Kilometers)
 *
 * @author Cory O'Connor
 */
class Position {

	/// \todo TODO: Make a LHLV coordinate frame available for Position

	/** 
	 * The list of data containing either precalculated position information or the fixed position
	 * in whatever reference frame. Note: this could also contain velocity data if the output of
	 * an EOMS object is the full state and not just the position state. This is likely to happen
	 * in an orbital propagator (from the orbital_EOMS class).
	 */
	map<double, Matrix> posData;

	/** 
	 * The position info type, containing the reference frame the position information is in, and 
	 * if the target is fixed, in a predetermined path, or in a dynamically changing path. The
	 * following are examples of typical types of each of the 6 types.
	 * <B>POSITION_TYPE_STATIC_LLA<B>
	 * A house, bridge, bunker, etc.. Something that doesnt ever move, and is specified in LLA
	 * coordinates.
	 * <B>POSITION_TYPE_STATIC_ECI<B>
	 * Something that doesnt move with respect to the solar system as a whole. Most likely the
	 * only things under these categories will be stars.
	 * <B>POSITION_TYPE_PREDETERMINED_LLA<B>
	 * An object that has a predetermined path on Earth and doesn't change their movement in
	 * response to anything going on in the simulation. Good examples include a train or a boat. 
	 * <B>POSITION_TYPE_PREDETERMINED_ECI<B>
	 * Most space assets will fall under this category. The typical example is a satellite with 
	 * predetermined orbital parameters.
	 * <B>POSITION_TYPE_DYNAMIC_LLA<B>
	 * Things that are capable of varying their position on the Earth in response to events in the
	 * scope of the simulation. These can be assets or targets. A good example would be
	 * a bomber, UAV, tank, infantry group... In this case, the Position object is just a holder
	 * class for the EOMS object, the initial conditions, and the coordinate frame.
	 * <B>POSITION_TYPE_DYNAMIC_ECI<B>
	 * A non-earth object capable of changing it's trajectory. Spacecraft with propulsion, aliens...
	 * In this case, the Position object is just a holder class for the EOMS object, the initial 
	 * conditions, and the coordinate frame.
	 */
	const string posType;

	/**
	 * The EOMS object that describes the equations of motion (kinematic) of the object the position
	 * belongs to. It is worth noting that the eoms object could include output for other states
	 * (not only the position, but also possibly the velocity) but it is up to the modeler of the
	 * simulation to remember this and reference the correct information. Storage of velocity data
	 * can happen in the state or in the Position variable posData, and thus if it is required 
	 * information it must be concatenated with the position data to form a complete 
	 * initial state variable. Likewise the output may come in a full state vector and thus the 
	 * "position" part of the EOMS output must be separated from the other information.
	 */
	EOMS* eomsObject;

	double posDataTimeStep;

	XMLNode integratorNode;

public:

	/**
	 * Creates a new position with the information in the specified XMLNode
	 * @param positionXMLNode The XMLNode containing the position data
	 */
	Position(XMLNode& positionXMLNode);

	/**
	 * Creates a new fixed position with the specified fixed values in the specified coordinate
	 * frame. This makes the member variable posData a 3x1 matrix containing only position data.
	 * @param type The type of Position object this is. See above under posType.
	 * @param data1 The first of the fixed Matrix elements (Longitude for LLA, X for ECI).
	 * @param data2 The second of the fixed Matrix elements (Latitude for LLA, Y for ECI).
	 * @param data3 The third of the fixed Matrix elements (Altitude for LLA, Z for ECI).
	 */
	Position(const string& type, const double& data1, const double& data2, const double& data3);

	/**
	 * Creates a new fixed Position with the specified position Matrix in the specified coordinate
	 * frame. This makes the member variable posData a 3x1 matrix containing only position data.
	 * @param type The type of position object this is. See above under posType.
	 * @param pos A Matrix with the three elements specifying position (LLA or ECI).
	 */
	Position(const string& type, const Matrix& pos);

	/**
	 * Creates a new variable Position (either dynamic or predetermined) with an EOMS object
	 * describing the equations of motion in the specified coordinate frame. When instantiated,
	 * if the object is predetermined, it will propagate the data and save the result. If it is
	 * a dynamic object however, the 
	 * @param type The type of position object this is. See above under posType.
	 * @param equationsOfMotion The EOMS object describing the equations of motion, possibly 
	 * associated with an EOMSInput class too for EOMS objects that are being controlled.
	 * @param data1 The first initial condition (simulation start) Matrix elements (Longitude for LLA, X for ECI).
	 * @param data2 The second initial (simulation start) Matrix elements (Latitude for LLA, Y for ECI).
	 * @param data3 The third initial (simulation start) Matrix elements (Altitude for LLA, Z for ECI).
	 */
	Position(const string& type, EOMS* equationsOfMotion, const double& data1, const double& data2, 
		const double& data3, const double& data4, const double& data5, const double& data6);

	/**
	 * Creates a new variable Position (either dynamic or predetermined) with an EOMS object
	 * describing the equations of motion in the specified coordinate frame. When instantiated,
	 * if the object is predetermined, it will propagate the data and save the result. If it is
	 * a dynamic object however, the 
	 * @param type The type of position object this is. See above under posType.
	 * @param equationsOfMotion The EOMS object describing the equations of motion, possibly 
	 * associated with an EOMSInput class too for EOMS objects that are being controlled.
	 * @param initialConditions A Matrix with the initial conditions of the EOMS object 
	 * equationsOfMotion.
	 */
	Position(const string& type, EOMS* equationsOfMotion, const Matrix& initialConditions);

	/** Destroys the Position object. */
	~Position();

	void setPosDataTimeStep(double step);

	/** 
	 * Gets the type of Position object. Similar for all Position types. 
	 * @return A string containing the type of Position object.
	 */
	const string getType();

	/**
	 * Gets the EOMS object associated with this Position class. Only appropriate for Positions of
	 * either type DYNAMIC. If of a different type (PREDETERMINED OR STATIC), returns a null pointer.
	 * @return A pointer to the EOMS object associated with this Position.
	 */
	EOMS* getEOMS();

	/**
	 * Gets the initial conditions of this Position. Only appropriate for DYNAMIC Positions in either
	 * coordinate frame. If of a different type (PREDETERMINED OR STATIC), returns an empty Matrix.
	 * @return A 3x1 Matrix containing the initial conditions, as set in the constructor.
	 */
	const Matrix getIC();

	/** 
	 * Gets the position of an object at a certain simluation time in the ECI frame. If the object was
	 * not set in this frame, it calculates the transformation to the alternate frame. Be careful
	 * when using this method for posData containing velocity vectors as it will assume that the first
	 * 3 elements in the posData are the position information. 

	 * Specific position type information:
	 * POSITION_TYPE_STATIC_LLA = Converts, then returns normal 3x1 Matrix containing ECI position data.
	 * POSITION_TYPE_STATIC_ECI = Returns normal 3x1 Matrix containing ECI position data.
	 * POSITION_TYPE_PREDETERMINED_LLA = Propagates (if not already done), converts, then returns normal 
	 * 3x1 Matrix containing ECI position data.
	 * POSITION_TYPE_PREDETERMINED_ECI = Propagates (if not already done), then returns normal 
	 * 3x1 Matrix containing ECI position data.
	 * POSITION_TYPE_DYNAMIC_LLA = ERROR! The position does not hold the final data for this type. The 
	 * calling object must create an EOMSinput class and an integrator and calculate the result on a
	 * case by case basis.
	 * POSITION_TYPE_DYNAMIC_ECI = ERROR! The position does not hold the final data for this type. The 
	 * calling object must create an EOMSinput class and an integrator and calculate the result on a
	 * case by case basis.

	 * @param simTime The simulation time (in seconds) from the simulation start.
	 * @return A Matrix containing ECI position data at the given time.
	 */
	const Matrix getStateECI(const double& simTime);

	Matrix getStateAtTime(const double& simTime);

	const Matrix getPosECI(const double& simTime);
	const Matrix getVelECI(const double& simTime);

	/** 
	 * Gets the position of an object at a certain simluation time in the LLA frame. If the object was
	 * not set in this frame, it calculates the transformation to the alternate frame. Be careful
	 * when using this method for posData containing velocity vectors as it will assume that the first
	 * 3 elements in the posData are the position information. 

	 * Specific position type information:
	 * POSITION_TYPE_STATIC_LLA = Returns normal 3x1 Matrix containing LLA position data.
	 * POSITION_TYPE_STATIC_ECI = Converts, then returns normal 3x1 Matrix containing LLA position data.
	 * POSITION_TYPE_PREDETERMINED_LLA = Propagates (if not already done), then returns normal 
	 * 3x1 Matrix containing LLA position data.
	 * POSITION_TYPE_PREDETERMINED_ECI = Propagates (if not already done), converts, then returns normal 
	 * 3x1 Matrix containing LLA position data.
	 * POSITION_TYPE_DYNAMIC_LLA = ERROR! The position does not hold the final data for this type. The 
	 * calling object must create an EOMSinput class and an integrator and calculate the result on a
	 * case by case basis.
	 * POSITION_TYPE_DYNAMIC_ECI = ERROR! The position does not hold the final data for this type. The 
	 * calling object must create an EOMSinput class and an integrator and calculate the result on a
	 * case by case basis.

	 * @param simTime The simulation time (in seconds) from the simulation start.
	 * @return A Matrix containing LLA position data at the given time.
	 */
	const Matrix getPosLLA(const double& simTime);

	bool hasLOSTo(Position* posECI, double simTime);

	bool hasLOSTo(Matrix posECI, double simTime);

	enum propagationModel
	{
		TRAPZ,
		RK4,
		RK45,
		SGP4
	};

private:

	void setIntegratorParams(Integrator& solver);
	void setPropagationModel();

	propagationModel propagatorType;

};

}} // end namespace horizon::geom

#endif /*POSITION_H_*/

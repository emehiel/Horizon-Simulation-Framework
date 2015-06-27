// Access Subsystem Keys
#define SUBNAME_ACCESS "AccessToTarget"

// Adcs Subsystem Keys
#define SUBNAME_ADCS "Adcs"
#define STATEVARNAME_POINTVEC "ECI_Pointing_Vector(XYZ)"
#define STATEVARNAME_LOOKANGLE "LookAngleOffNadir_deg"
#define STATEVARNAME_SUNEL "SunElAtTarg_deg"

#define POINTVEC_KEY StateVarKey<Matrix> (STATEVARNAME_POINTVEC)
#define LOOKANGLE_KEY StateVarKey<double> (STATEVARNAME_LOOKANGLE)
#define SUNEL_KEY StateVarKey<double> (STATEVARNAME_SUNEL)

// Comm Subsystem Keys
#define SUBNAME_COMM "Comm"
#define STATEVARNAME_DATARATE "DataRate(MB/s)"

#define DATARATE_KEY StateVarKey<double> (STATEVARNAME_DATARATE)

// EOSensor Subsystem Keys
#define SUBNAME_EOSENSOR "EOSensor"
#define STATEVARNAME_IMAGEDATAVOLUME "Image_Data_Volume"
#define STATEVARNAME_EOON "EOSensorOn"
#define STATEVARNAME_GSD "gsd_cm"

#define IMAGEDATAVOLUME_KEY StateVarKey<double> (STATEVARNAME_IMAGEDATAVOLUME)
#define EOON_KEY StateVarKey<bool> (STATEVARNAME_EOON)
#define GSD_KEY StateVarKey<double> (STATEVARNAME_GSD)

// Power Subsystem Keys
#define SUBNAME_POWER "Power"
#define STATEVARNAME_DOD "DepthofDischarge"
#define STATEVARNAME_POWIN "SolarPanelPowerIn"

#define DOD_KEY StateVarKey<double>(STATEVARNAME_DOD)
#define POWIN_KEY StateVarKey<double>(STATEVARNAME_POWIN)

// Ssdr Subsystem Keys
#define SUBNAME_SSDR "SSDR"
#define STATEVARNAME_BITSONBUFFER "BitsOnDataBuffer_bits"

#define BITSONBUFFER_KEY StateVarKey<double> (STATEVARNAME_BITSONBUFFER)
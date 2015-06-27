//
// Globals.cs
//
// Copyright (c) 2003-2006 Michael F. Henry
//
using System;

namespace Zeptomoby.OrbitTools
{
	/// <summary>
	/// Summary description for Globals.
	/// </summary>
	abstract public class Globals
	{
      #region Constants

      public const double PI           = 3.141592653589793;
      public const double TWOPI        = 2.0 * Globals.PI;
      public const double RADS_PER_DEG = Globals.PI / 180.0;

      public const double GM          = 398601.2;   // Earth gravitational constant, km^3/sec^2
      public const double GEOSYNC_ALT = 42241.892;  // km
      public const double EARTH_RAD   = 6370.0;     // km
      public const double EARTH_DIA   = 12800.0;    // km
      public const double DAY_SIDEREAL= (23 * 3600) + (56 * 60) + 4.09;  // sec
      public const double DAY_SOLAR   = (24 * 3600);   // sec

      public const double AE          = 1.0;
      public const double AU          = 149597870.0;  // Astronomical unit (km) (IAU 76)
      public const double SR          = 696000.0;     // Solar radius (km)      (IAU 76)
      public const double TWOTHRD     = 2.0 / 3.0;
      public const double XKMPER      = 6378.135;     // Earth equatorial radius - kilometers (WGS '72)
      public const double F           = 1.0 / 298.26; // Earth flattening (WGS '72)
      public const double GE          = 398600.8;     // Earth gravitational constant (WGS '72)
      public const double J2          = 1.0826158E-3; // J2 harmonic (WGS '72)
      public const double J3          = -2.53881E-6;  // J3 harmonic (WGS '72)
      public const double J4          = -1.65597E-6;  // J4 harmonic (WGS '72)
      public const double CK2         = J2 / 2.0;
      public const double CK4         = -3.0 * J4 / 8.0;
      public const double XJ3         = J3;
      public const double E6A         = 1.0e-06;
      public const double QO          = Globals.AE + 120.0 / Globals.XKMPER;
      public const double S           = Globals.AE + 78.0  / Globals.XKMPER;
      public const double MIN_PER_DAY = 1440.0;        // Minutes per day (solar)
      public const double SEC_PER_DAY = 86400.0;       // Seconds per day (solar)
      public const double OMEGA_E     = 1.00273790934; // Earth rotation per sidereal day
      public static double XKE        = Math.Sqrt(3600.0 * GE / 
                                           (Globals.XKMPER * Globals.XKMPER * Globals.XKMPER)); // sqrt(ge) ER^3/min^2
      public static double QOMS2T     = Math.Pow((QO - Globals.S), 4); //(QO - S)^4 ER^4

      public const double EQ_KM_WGS72           = (PI * (2.0 * XKMPER));       // Equatorial diameter, km
      public const double EQ_AU_WGS72           = EQ_KM_WGS72 / AU;            // Equatorial diameter, AU
      public const double EQ_KMPERSEC_SOL_WGS72 = EQ_KM_WGS72 / SEC_PER_DAY;   // km/sec @ equator, solar day
      public const double EQ_KMPERSEC_SID_WGS72 = EQ_KMPERSEC_SOL_WGS72 * OMEGA_E;// km/sec, sidereal day
      public const double EQ_AUPERSEC_SOL_WGS72 = EQ_AU_WGS72 / SEC_PER_DAY;      // AE/sec @ eq, solar day
      public const double EQ_AUPERSEC_SID_WGS72 = EQ_AUPERSEC_SOL_WGS72 * OMEGA_E;// AE/sec @ eq, sidereal day

      #endregion

      #region Utility

      // ///////////////////////////////////////////////////////////////////////////
      public static double Sqr(double x) 
      {
         return (x * x);
      }

      // ///////////////////////////////////////////////////////////////////////////
      public static double Fmod2p(double arg)
      {
         double modu = (arg % TWOPI);

         if (modu < 0.0)
            modu += TWOPI;

         return modu;
      }

      // ///////////////////////////////////////////////////////////////////////////
      // Globals.AcTan()
      // ArcTangent of sin(x) / cos(x). The advantage of this function over arctan()
      // is that it returns the correct quadrant of the angle.
      public static double AcTan(double sinx, double cosx)
      {
         double ret;

         if (cosx == 0.0)
         {
            if (sinx > 0.0)
               ret = PI / 2.0;
            else
               ret = 3.0 * PI / 2.0;
         }
         else
         {
            if (cosx > 0.0)
               ret = Math.Atan(sinx / cosx);
            else
               ret = PI + Math.Atan(sinx / cosx);
         }

         return ret;
      }

      // ///////////////////////////////////////////////////////////////////////////
      public static double Rad2Deg(double r)
      {
         const double DEG_PER_RAD = 180.0 / PI;
         return r * DEG_PER_RAD;
      }

      // ///////////////////////////////////////////////////////////////////////////
      public static double Deg2Rad(double d)
      {
         const double RAD_PER_DEG = PI / 180.0;
         return d * RAD_PER_DEG;
      }	

      #endregion
   }
}

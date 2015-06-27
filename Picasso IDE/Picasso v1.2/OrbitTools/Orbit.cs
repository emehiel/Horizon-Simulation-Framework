//
// Orbit.cs
//
// Copyright (c) 2005-2009 Michael F. Henry
//
using System;

namespace Zeptomoby.OrbitTools
{
   /// <summary>
   /// This class accepts a single satellite's NORAD two-line element
   /// set and provides information regarding the satellite's orbit 
   /// such as period, axis length, ECI coordinates, velocity, etc.
   /// </summary>
   public class Orbit
   {
      private Tle       m_tle;
      private Julian    m_jdEpoch;
      private NoradBase m_NoradModel;

      // Caching variables
      private TimeSpan m_Period = new TimeSpan(0, 0, 0, -1);

      // Caching variables recovered from the input TLE elements
      private double m_aeAxisSemiMajorRec;  // semimajor axis, in AE units
      private double m_aeAxisSemiMinorRec;  // semiminor axis, in AE units
      private double m_rmMeanMotionRec;     // radians per minute
      private double m_kmPerigeeRec;        // perigee, in km
      private double m_kmApogeeRec;         // apogee, in km

      #region Properties

      public Julian   Epoch     { get { return m_jdEpoch;            }}
      public DateTime EpochTime { get { return m_jdEpoch.toTime();   }}

      // "Recovered" from the input elements
      public double SemiMajor   { get { return m_aeAxisSemiMajorRec; }}
      public double SemiMinor   { get { return m_aeAxisSemiMinorRec; }}
      public double MeanMotion  { get { return m_rmMeanMotionRec;    }}
      public double Major       { get { return 2.0 * SemiMajor;      }}
      public double Minor       { get { return 2.0 * SemiMinor;      }}
      public double Perigee     { get { return m_kmPerigeeRec;       }}
      public double Apogee      { get { return m_kmApogeeRec;        }}

      public double Inclination  { get { return radGet(Tle.Field.Inclination);           }}
      public double Eccentricity { get { return m_tle.getField(Tle.Field.Eccentricity);  }}
      public double RAAN         { get { return radGet(Tle.Field.Raan);                  }}
      public double ArgPerigee   { get { return radGet(Tle.Field.ArgPerigee);            }}
      public double BStar        { get { return m_tle.getField(Tle.Field.BStarDrag) / Globals.AE;}}
      public double Drag         { get { return m_tle.getField(Tle.Field.MeanMotionDt);  }}
      public double mnMotion     { get { return m_tle.getField(Tle.Field.MeanMotion);    }}

      public string SatNoradId   { get { return m_tle.NoradNumber; }}

      public TimeSpan Period 
      {
         get 
         { 
            if (m_Period.TotalSeconds < 0.0)
            {
               // Calculate the period using the recovered mean motion.
               if (MeanMotion == 0)
               {
                  m_Period = new TimeSpan(0, 0, 0);
               }
               else
               {
                  double secs  = (Globals.TWOPI / MeanMotion) * 60.0;
                  int    msecs = (int)((secs - (int)secs) * 1000);

                  m_Period = new TimeSpan(0, 0, 0, (int)secs, msecs);
               }
            }

            return m_Period;
         }
      }

      #endregion

      #region Construction

      // ///////////////////////////////////////////////////////////////////
      public Orbit(Tle tle)
      {
         m_tle     = tle;
         m_jdEpoch = m_tle.EpochJulian;

         // Recover the original mean motion and semimajor axis from the
         // input elements.
         double mm     = mnMotion;
         double rpmin  = mm * Globals.TWOPI / Globals.MIN_PER_DAY;   // rads per minute

         double a1     = Math.Pow(Globals.XKE / rpmin, Globals.TWOTHRD);
         double e      = Eccentricity;
         double i      = Inclination;
         double temp   = (1.5 * Globals.CK2 * (3.0 * Globals.Sqr(Math.Cos(i)) - 1.0) / 
                         Math.Pow(1.0 - e * e, 1.5));   
         double delta1 = temp / (a1 * a1);
         double a0     = a1 * 
                        (1.0 - delta1 * 
                        ((1.0 / 3.0) + delta1 * 
                        (1.0 + 134.0 / 81.0 * delta1)));

         double delta0 = temp / (a0 * a0);

         m_rmMeanMotionRec    = rpmin / (1.0 + delta0);
         m_aeAxisSemiMajorRec = a0 / (1.0 - delta0);
         m_aeAxisSemiMinorRec = m_aeAxisSemiMajorRec * Math.Sqrt(1.0 - (e * e));
         m_kmPerigeeRec       = Globals.XKMPER * (m_aeAxisSemiMajorRec * (1.0 - e) - Globals.AE);
         m_kmApogeeRec        = Globals.XKMPER * (m_aeAxisSemiMajorRec * (1.0 + e) - Globals.AE);

         if (Period.TotalMinutes >= 225.0)
         {
            // SDP4 - period >= 225 minutes.
            m_NoradModel = new NoradSDP4(this);
         }
         else
         {
            // SGP4 - period < 225 minutes
            m_NoradModel = new NoradSGP4(this);
         }
      }

      #endregion

      // ///////////////////////////////////////////////////////////////////////////
      // Cannot be a property because function signature is overloaded.
      public double mnAnomaly()
      {  
         return radGet(Tle.Field.MeanAnomaly);           
      }

      // ///////////////////////////////////////////////////////////////////////////
      // Returns the mean anomaly in radians at given GMT.
      // At epoch, the mean anomaly is given by the elements data.
      public double mnAnomaly(DateTime gmt)
      {
         double span = TPlusEpoch(gmt).TotalSeconds;
         double P    = Period.TotalSeconds;

         return (mnAnomaly() + (Globals.TWOPI * (span / P))) % Globals.TWOPI;
      }

      // ///////////////////////////////////////////////////////////////////////////
      // Returns elapsed time from epoch to given time.
      // Note: "Predicted" TLEs can have epochs in the future.
      public TimeSpan TPlusEpoch(DateTime gmt) 
      {
         return (gmt - EpochTime);
      }

      // ///////////////////////////////////////////////////////////////////////////
      // Returns elapsed time from epoch to current time.
      // Note: "Predicted" TLEs can have epochs in the future.
      public TimeSpan TPlusEpoch()
      {
         return TPlusEpoch(DateTime.Now);
      }

      /// <summary>
      /// Calculate ECI position/velocity for a given time past the epoch.
      /// </summary>
      /// <param name="tsince">Target time, in minutes past the TLE (UTC) epoch.</param>
      /// <returns>Kilometer-based position/velocity ECI coordinates.</returns>
      public Eci getPosition(double minutesPastEpoch) 
      {
         Eci eci = m_NoradModel.getPosition(minutesPastEpoch);

         eci.ae2km();

         return eci;
      }

      /// <summary>
      /// Calculate ECI position/velocity for a given time.
      /// </summary>
      /// <param name="gmt">Target time (GMT).</param>
      /// <returns>Kilometer-based position/velocity ECI coordinates.</returns>
      public Eci getPosition(DateTime gmt)
      {
         return getPosition(TPlusEpoch(gmt).TotalMinutes);
      }

      // ///////////////////////////////////////////////////////////////////////////
      // Return the name of the satellite.
      public string SatName()
      {
         return SatName(false);
      }

      // ///////////////////////////////////////////////////////////////////////////
      // SatName()
      // Return the name of the satellite. If requested, the NORAD number is
      // appended to the end of the name, i.e., "ISS (ZARYA) #25544".
      // The name of the satellite with the NORAD number appended is important
      // because many satellites, especially debris, have the same name and
      // would otherwise appear to be the same satellite in output data.
      public string SatName(bool fAppendId)
      {
         string str = m_tle.Name;

         if (fAppendId)
         {
            str = str + " #" + SatNoradId;
         }

         return str;
      }

      #region Utility

      // ///////////////////////////////////////////////////////////////////
      protected double radGet(Tle.Field fld) 
      { 
         return m_tle.getField(fld, Tle.Unit.Radians); 
      }

      // ///////////////////////////////////////////////////////////////////
      protected double degGet(Tle.Field fld) 
      { 
         return m_tle.getField(fld, Tle.Unit.Degrees); 
      }

      #endregion
   }
}

//
// Eci.cs
//
// Copyright (c) 2003-2009 Michael F. Henry
//
using System;

namespace Zeptomoby.OrbitTools
{
   /// <summary>
   /// Encapsulates an Earth-Centered Inertial coordinate position/velocity.
   /// </summary>
   public class Eci
   {
      private Vector      m_Position;
      private Vector      m_Velocity;
      private Julian      m_Date;
      private VectorUnits m_VectorUnits;

      protected enum VectorUnits
      {
         None, // not initialized
         Ae,
         Km,
      };

      #region Construction

      public Eci() 
      { 
         m_VectorUnits = VectorUnits.None;
      }

      public Eci(Vector pos, Vector vel, Julian date, bool IsAeUnits)
      {
         m_Position = pos;
         m_Velocity = vel;
         m_Date     = date;
         m_VectorUnits = (IsAeUnits ? VectorUnits.Ae : VectorUnits.None);
      }

      // ///////////////////////////////////////////////////////////////////
      // Calculate the ECI coordinates of the location "geo" at time "date".
      // Assumes geo coordinates are km-based.
      // Assumes the earth is an oblate spheroid as defined in WGS '72.
      // Reference: The 1992 Astronomical Almanac, page K11
      // Reference: www.celestrak.com (Dr. TS Kelso)
      public Eci(CoordGeo geo, Julian date)
      {
         m_VectorUnits = VectorUnits.Km;

         double mfactor = Globals.TWOPI * (Globals.OMEGA_E / Globals.SEC_PER_DAY);
         double lat = geo.Latitude;
         double lon = geo.Longitude;
         double alt = geo.Altitude;

         // Calculate Local Mean Sidereal Time (theta)
         double theta = date.toLMST(lon);
         double c = 1.0 / Math.Sqrt(1.0 + Globals.F * (Globals.F - 2.0) * Globals.Sqr(Math.Sin(lat)));
         double s = Globals.Sqr(1.0 - Globals.F) * c;
         double achcp = (Globals.XKMPER * c + alt) * Math.Cos(lat);

         m_Date = date;

         m_Position = new Vector();

         m_Position.X = achcp * Math.Cos(theta);                    // km
         m_Position.Y = achcp * Math.Sin(theta);                    // km
         m_Position.Z = (Globals.XKMPER * s + alt) * Math.Sin(lat); // km
         m_Position.W = Math.Sqrt(Globals.Sqr(m_Position.X) + 
            Globals.Sqr(m_Position.Y) + 
            Globals.Sqr(m_Position.Z));            // range, km

         m_Velocity = new Vector();

         m_Velocity.X = -mfactor * m_Position.Y;               // km / sec
         m_Velocity.Y =  mfactor * m_Position.X;
         m_Velocity.Z = 0.0;
         m_Velocity.W = Math.Sqrt(Globals.Sqr(m_Velocity.X) +  // range rate km/sec^2
            Globals.Sqr(m_Velocity.Y));
      }
      #endregion

      #region Properties

      public Vector   Position { get { return m_Position;  } }
      public Vector   Velocity { get { return m_Velocity;  } }
      public Julian   Date     { get { return m_Date; } }

      protected VectorUnits Units    
      { 
         get { return m_VectorUnits;  } 
         set { m_VectorUnits = value; }
      }

      #endregion

      public void SetUnitsAe() { Units = VectorUnits.Ae; }
      public void SetUnitsKm() { Units = VectorUnits.Km; }
      public bool UnitsAreAe() { return Units == VectorUnits.Ae; }
      public bool UnitsAreKm() { return Units == VectorUnits.Km; }

      // ///////////////////////////////////////////////////////////////////////////
      // Return the corresponding geodetic position (based on the current ECI
      // coordinates/Julian date).
      // Assumes the earth is an oblate spheroid as defined in WGS '72.
      // Side effects: Converts the position and velocity vectors to km-based units.
      // Reference: The 1992 Astronomical Almanac, page K12. 
      // Reference: www.celestrak.com (Dr. TS Kelso)
      public CoordGeo toGeo()
      {
         ae2km(); // Vectors must be in kilometer-based units

         double theta = Globals.AcTan(m_Position.Y, m_Position.X);
         double lon   = (theta - m_Date.toGMST()) % Globals.TWOPI;
   
         if (lon < 0.0) 
            lon += Globals.TWOPI;  // "wrap" negative modulo

         double r   = Math.Sqrt(Globals.Sqr(m_Position.X) + Globals.Sqr(m_Position.Y));
         double e2  = Globals.F * (2.0 - Globals.F);
         double lat = Globals.AcTan(m_Position.Z, r);

         const double DELTA = 1.0e-07;
         double phi;
         double c;

         do   
         {
            phi = lat;
            c   = 1.0 / Math.Sqrt(1.0 - e2 * Globals.Sqr(Math.Sin(phi)));
            lat = Globals.AcTan(m_Position.Z + Globals.XKMPER * c * e2 * Math.Sin(phi), r);
         }
         while (Math.Abs(lat - phi) > DELTA);
   
         double alt = r / Math.Cos(lat) - Globals.XKMPER * c;

         return new CoordGeo(lat, lon, alt); // radians, radians, kilometers
      }

      // ///////////////////////////////////////////////////////////////////////////
      // Convert the position and velocity vector units from Globals.AE-based units
      // to kilometer based units.
      public void ae2km()
      {
         if (UnitsAreAe())
         {  
            MulPos(Globals.XKMPER / Globals.AE);                                    // km
            MulVel((Globals.XKMPER / Globals.AE) * (Globals.MIN_PER_DAY / 86400));  // km/sec
            m_VectorUnits = VectorUnits.Km;
         }
      }

      protected void MulPos(double factor) 
      {
         m_Position.Mul(factor); 
      }

      protected void MulVel(double factor) 
      { 
         m_Velocity.Mul(factor); 
      }
   }
}

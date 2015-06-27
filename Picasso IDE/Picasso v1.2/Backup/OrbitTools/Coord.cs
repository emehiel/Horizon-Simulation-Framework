//
// Coord.cs
//
// Copyright (c) 2003-2006 Michael F. Henry
//
using System;

namespace Zeptomoby.OrbitTools
{
   //
   // Geocentric coordinates
   //
   public class CoordGeo  
   {
      private double m_Latitude;  // radians (negative south)
      private double m_Longitude; // radians (negative west)
      private double m_Altitude;  // km      (above ellipsoid model)

      #region Construction

      public CoordGeo()
      {
         m_Latitude = 0.0;
         m_Longitude = 0.0;
         m_Altitude = 0.0;
      }

      public CoordGeo(double lat, double lon, double alt)
      {
         m_Latitude = lat;
         m_Longitude = lon;
         m_Altitude = alt;
      }

      #endregion

      #region Properties

      // Negative value means south
      public double Latitude  
      { 
         get { return m_Latitude;  } 
         set { m_Latitude = value; }
      }

      // Negative value means west
      public double Longitude 
      { 
         get { return m_Longitude;  }
         set { m_Longitude = value; }
      }

      // Kilometers above ellipsoid model
      public double Altitude  
      {
         get { return m_Altitude;  }
         set { m_Altitude = value; }
      }

      #endregion
   }


   //
   // Topocentric-Horizon coordinates
   //
   public class CoordTopo  
   {
      private double m_Azimuth;    // radians
      private double m_Elevation;  // radians
      private double m_Range;      // kilometers
      private double m_RangeRate;  // kilometers/sec
                                   // Negative value means "towards observer"
      #region Construction

      public CoordTopo()
      {
         m_Azimuth        = 0.0;
         m_Elevation        = 0.0;       
         m_Range     = 0.0;    
         m_RangeRate = 0.0;
      }

      public CoordTopo(double az, double el, double rng, double rate)
      {
         m_Azimuth = az;
         m_Elevation = el;
         m_Range = rng;
         m_RangeRate = rate;
      }

      #endregion

      #region Properties

      // In radians
      public double Azimuth
      {
         get { return m_Azimuth;  }
         set { m_Azimuth = value; }
      }

      // In radians
      public double Elevation
      {
         get { return m_Elevation;  }
         set { m_Elevation = value; }
      }

      // In kilometers
      public double Range
      {
         get { return m_Range;  }
         set { m_Range = value; }
      }
      
      // Range rate of change in km/sec
      public double RangeRate
      {
         get { return m_RangeRate;  }
         set { m_RangeRate = value; }
      }

      #endregion
   }
}

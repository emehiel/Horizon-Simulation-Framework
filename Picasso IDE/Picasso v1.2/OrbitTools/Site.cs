//
// Site.cs
//
// Copyright (c) 2003-2006 Michael F. Henry
//
using System;

namespace Zeptomoby.OrbitTools
{
   /// <summary>
   /// The Site class encapsulates a location on earth.
   /// </summary>
   public class Site
   {
      private CoordGeo m_geo;  // lat, lon, alt of earth site

      #region Construction

      // //////////////////////////////////////////////////////////////////////////
      public Site(CoordGeo geo)
      {
         m_geo = geo;
      }

      // ///////////////////////////////////////////////////////////////////////////
      // c'tor accepting:
      //    Latitude  in degress (negative south)
      //    Longitude in degress (negative west)
      //    Altitude  in km
      public Site(double degLat, double degLon, double kmAlt)
      {
         m_geo = 
            new CoordGeo(Globals.Deg2Rad(degLat), Globals.Deg2Rad(degLon), kmAlt);
      }
   
      #endregion

      #region Properties

      public CoordGeo Geo
      {
         get { return m_geo;  }
         set { m_geo = value; }
      }

      public double Latitude  { get { return m_geo.Latitude;  } }
      public double Longitude { get { return m_geo.Longitude; } }
      public double Altitude  { get { return m_geo.Altitude;  } }

      #endregion
 
      // ///////////////////////////////////////////////////////////////////////////
      // getPosition()
      // Return the ECI coordinate of the site at the given time.
      public Eci getPosition(Julian date)
      {
         return new Eci(m_geo, date);
      }

      // ///////////////////////////////////////////////////////////////////////////
      // getLookAngle()
      // Return the topocentric (azimuth, elevation, etc.) coordinates for a target
      // object described by the input ECI coordinates.
      public CoordTopo getLookAngle(Eci eci)
      {
         // Calculate the ECI coordinates for this Site object at the time
         // of interest.
         Julian date    = eci.Date;
         Eci    eciSite = new Eci(m_geo, date); 

         // The Site ECI units are km-based; ensure target ECI units are same
         if (!eci.UnitsAreKm())
            throw new Exception("ECI units must be kilometer-based");

         Vector vecRgRate = new Vector(eci.Velocity.X - eciSite.Velocity.X,
                                       eci.Velocity.Y - eciSite.Velocity.Y,
                                       eci.Velocity.Z - eciSite.Velocity.Z);

         double x = eci.Position.X - eciSite.Position.X;
         double y = eci.Position.Y - eciSite.Position.Y;
         double z = eci.Position.Z - eciSite.Position.Z;
         double w = Math.Sqrt(Globals.Sqr(x) + Globals.Sqr(y) + Globals.Sqr(z));

         Vector vecRange = new Vector(x, y, z, w);

         // The site's Local Mean Sidereal Time at the time of interest.
         double theta = date.toLMST(Longitude);

         double sin_lat   = Math.Sin(Latitude);
         double cos_lat   = Math.Cos(Latitude);
         double sin_theta = Math.Sin(theta);
         double cos_theta = Math.Cos(theta);

         double top_s = sin_lat * cos_theta * vecRange.X + 
                        sin_lat * sin_theta * vecRange.Y - 
                        cos_lat * vecRange.Z;
         double top_e = -sin_theta * vecRange.X + 
                         cos_theta * vecRange.Y;
         double top_z = cos_lat * cos_theta * vecRange.X + 
                        cos_lat * sin_theta * vecRange.Y + 
                        sin_lat * vecRange.Z;
         double az    = Math.Atan(-top_e / top_s);

         if (top_s > 0.0)
            az += Globals.PI;

         if (az < 0.0)
            az += 2.0 * Globals.PI;

         double el   = Math.Asin(top_z / vecRange.W);
         double rate = (vecRange.X * vecRgRate.X + 
                        vecRange.Y * vecRgRate.Y + 
                        vecRange.Z * vecRgRate.Z) / vecRange.W;

         CoordTopo topo = new CoordTopo(az,         // azimuth, radians
                                        el,         // elevation, radians
                                        vecRange.W, // range, km
                                        rate);      // rate, km / sec
#if WANT_ATMOSPHERIC_CORRECTION
      // Elevation correction for atmospheric refraction.
      // Reference:  Astronomical Algorithms by Jean Meeus, pp. 101-104
      // Note:  Correction is meaningless when apparent elevation is below horizon
      topo.m_El += Globals.Deg2Rad((1.02 / 
                                    Math.Tan(Globals.Deg2Rad(Globals.Rad2Deg(el) + 10.3 / 
                                    (Globals.Rad2Deg(el) + 5.11)))) / 60.0);
      if (topo.m_El < 0.0)
         topo.m_El = el;    // Reset to true elevation

      if (topo.m_El > (Globals.PI / 2))
         topo.m_El = (Globals.PI / 2);
#endif
         return topo;
      }

      // ///////////////////////////////////////////////////////////////////////////
      public string toString()
      {
         bool LatNorth = true;
         bool LonEast  = true;

         if (m_geo.Latitude < 0.0) 
         {
            LatNorth = false;
         }

         if (m_geo.Longitude < 0.0)
         {
            LonEast = false;
         }

         string str = Math.Abs(Globals.Rad2Deg(m_geo.Latitude)).ToString("{0,6:f3} ");
            
         str += (LatNorth ? 'N' : 'S');

         str += Math.Abs(Globals.Rad2Deg(m_geo.Longitude)).ToString("{0,6:f3}");
         str += (LonEast ? 'E' : 'W');

         str += (m_geo.Altitude * 1000.0).ToString();
         str += "m"; // meters

         return str;
      }
   }
}

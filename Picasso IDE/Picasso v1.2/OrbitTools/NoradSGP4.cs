//
// NoradSGP4.cs
//
// Copyright (c) 2003-2006 Michael F. Henry
//
using System;

namespace Zeptomoby.OrbitTools
{
	/// <summary>
	/// NORAD SGP4 implementation.
	/// </summary>
	public class NoradSGP4 : NoradBase
	{
      private double m_c5; 
      private double m_omgcof;
      private double m_xmcof;
      private double m_delmo;
      private double m_sinmo;

      // ///////////////////////////////////////////////////////////////////////////
      public NoradSGP4(Orbit orbit) :
         base(orbit)
      {
         m_c5     = 2.0 * m_coef1 * m_aodp * m_betao2 * 
                    (1.0 + 2.75 * (m_etasq + m_eeta) + m_eeta * m_etasq);
         m_omgcof = m_Orbit.BStar * m_c3 * Math.Cos(m_Orbit.ArgPerigee);
         m_xmcof  = -Globals.TWOTHRD * m_coef * m_Orbit.BStar * Globals.AE / m_eeta;
         m_delmo  = Math.Pow(1.0 + m_eta * Math.Cos(m_Orbit.mnAnomaly()), 3.0);
         m_sinmo  = Math.Sin(m_Orbit.mnAnomaly());
      }

      // ///////////////////////////////////////////////////////////////////////////
      // getPosition() 
      // This procedure returns the ECI position and velocity for the satellite
      // in the orbit at the given number of minutes since the TLE epoch time
      // using the NORAD Simplified General Perturbation 4, near earth orbit
      // model.
      //
      // tsince - Time in minutes since the TLE epoch (GMT).
      // eci    - ECI object to hold position information.
      //           To convert the returned ECI position vector to km,
      //           multiply each component by: 
      //              (XKMMPER / Globals.AE).
      //           To convert the returned ECI velocity vector to km/sec, 
      //           multiply each component by:
      //              (XKMPER / Globals.AE) * (MIN_PER_DAY / 86400).
      public override Eci getPosition(double tsince)
      {
         // For m_perigee less than 220 kilometers, the isimp flag is set and
         // the equations are truncated to linear variation in Math.Sqrt a and
         // quadratic variation in mean anomaly.  Also, the m_c3 term, the
         // delta omega term, and the delta m term are dropped.
         bool isimp = false;
         if ((m_aodp * (1.0 - m_satEcc) / Globals.AE) < (220.0 / Globals.XKMPER + Globals.AE))
         {
            isimp = true;
         }

         double d2 = 0.0;
         double d3 = 0.0;
         double d4 = 0.0;

         double t3cof = 0.0;
         double t4cof = 0.0;
         double t5cof = 0.0;

         if (!isimp)
         {
            double c1sq = m_c1 * m_c1;

            d2 = 4.0 * m_aodp * m_tsi * c1sq;

            double temp = d2 * m_tsi * m_c1 / 3.0;

            d3 = (17.0 * m_aodp + m_s4) * temp;
            d4 = 0.5 * temp * m_aodp * m_tsi * 
               (221.0 * m_aodp + 31.0 * m_s4) * m_c1;
            t3cof = d2 + 2.0 * c1sq;
            t4cof = 0.25 * (3.0 * d3 + m_c1 * (12.0 * d2 + 10.0 * c1sq));
            t5cof = 0.2 * (3.0 * d4 + 12.0 * m_c1 * d3 + 6.0 * 
               d2 * d2 + 15.0 * c1sq * (2.0 * d2 + c1sq));
         }

         // Update for secular gravity and atmospheric drag. 
         double xmdf   = m_Orbit.mnAnomaly() + m_xmdot * tsince;
         double omgadf = m_Orbit.ArgPerigee + m_omgdot * tsince;
         double xnoddf = m_Orbit.RAAN + m_xnodot * tsince;
         double omega  = omgadf;
         double xmp    = xmdf;
         double tsq    = tsince * tsince;
         double xnode  = xnoddf + m_xnodcf * tsq;
         double tempa  = 1.0 - m_c1 * tsince;
         double tempe  = m_Orbit.BStar * m_c4 * tsince;
         double templ  = m_t2cof * tsq;

         if (!isimp)
         {
            double delomg = m_omgcof * tsince;
            double delm = m_xmcof * (Math.Pow(1.0 + m_eta * Math.Cos(xmdf), 3.0) - m_delmo);
            double temp = delomg + delm;

            xmp = xmdf + temp;
            omega = omgadf - temp;

            double tcube = tsq * tsince;
            double tfour = tsince * tcube;

            tempa = tempa - d2 * tsq - d3 * tcube - d4 * tfour;
            tempe = tempe + m_Orbit.BStar * m_c5 * (Math.Sin(xmp) - m_sinmo);
            templ = templ + t3cof * tcube + tfour * (t4cof + tsince * t5cof);
         }

         double a  = m_aodp * Globals.Sqr(tempa);
         double e  = m_satEcc - tempe;


         double xl = xmp + omega + xnode + m_xnodp * templ;
         double xn = Globals.XKE / Math.Pow(a, 1.5);

         return FinalPosition(m_satInc, omgadf, e, a, xl, xnode, xn, tsince);
      }
   }
}

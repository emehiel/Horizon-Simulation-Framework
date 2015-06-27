//
// Vector.cs
//
// Copyright (c) 2003-2006 Michael F. Henry
//
using System;

namespace Zeptomoby.OrbitTools
{
   /// <summary>
   /// Encapsultes a simple 4-component vector
   /// </summary>
   public class Vector
   {
      private double m_x;
      private double m_y;
      private double m_z;
      private double m_w;

      #region Construction

      public Vector()
      {
         m_x = 0.0;
         m_y = 0.0;
         m_z = 0.0;
         m_w = 0.0;
      }

      public Vector(double x, double y, double z)
      {
         m_x = x;
         m_y = y;
         m_z = z;
         m_w = 0.0;
      }

      public Vector(double x, double y, double z, double w)
      {
         m_x = x;
         m_y = y;
         m_z = z;
         m_w = w;
      }

      #endregion

      #region Properties

      public double X { get { return m_x; } set { m_x = value;} }
      public double Y { get { return m_y; } set { m_y = value;} }
      public double Z { get { return m_z; } set { m_z = value;} }
      public double W { get { return m_w; } set { m_w = value;} }

      #endregion

      // /////////////////////////////////////////////////////////////////////
      // Multiply each component in the vector by 'factor'.
      public void Mul(double factor)
      {
         m_x *= factor;
         m_y *= factor;
         m_z *= factor;
         m_w *= Math.Abs(factor);
      }

      // /////////////////////////////////////////////////////////////////////
      // Subtract a vector from this one.
      public void Sub(Vector vec)
      {
         m_x -= vec.m_x;
         m_y -= vec.m_y;
         m_z -= vec.m_z;
         m_w -= vec.m_w;
      }

      // /////////////////////////////////////////////////////////////////////
      // Calculate the angle between this vector and another
      public double Angle(Vector vec)
      {
         return Math.Acos(Dot(vec) / (Magnitude() * vec.Magnitude()));
      }

      // /////////////////////////////////////////////////////////////////////
      public double Magnitude()
      {
         return Math.Sqrt((m_x * m_x) + (m_y * m_y) + (m_z * m_z));
      }

      // /////////////////////////////////////////////////////////////////////
      // Return the dot product
      public double Dot(Vector vec)
      {
         return (m_x * vec.m_x) + (m_y * vec.m_y) + (m_z * vec.m_z);
      }
   }
}

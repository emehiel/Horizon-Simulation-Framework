//
// Tle.cs
//
// Copyright (c) 2003-2007 Michael F. Henry
//
using System;
using System.Collections;
using System.ComponentModel;
using System.Globalization;

// ////////////////////////////////////////////////////////////////////////
//
// NASA Two-Line Element Data format
//
// [Reference: Dr. T.S. Kelso / www.celestrak.com]
//
// Two line element data consists of three lines in the following format:
//
//  AAAAAAAAAAAAAAAAAAAAAA
//  1 NNNNNU NNNNNAAA NNNNN.NNNNNNNN +.NNNNNNNN +NNNNN-N +NNNNN-N N NNNNN
//  2 NNNNN NNN.NNNN NNN.NNNN NNNNNNN NNN.NNNN NNN.NNNN NN.NNNNNNNNNNNNNN
//  
//  Line 0 is a twenty-two-character name.
// 
//   Lines 1 and 2 are the standard Two-Line Orbital Element Set Format identical
//   to that used by NORAD and NASA.  The format description is:
//      
//     Line 1
//     Column    Description
//     01-01     Line Number of Element Data
//     03-07     Satellite Number
//     10-11     International Designator (Last two digits of launch year)
//     12-14     International Designator (Launch number of the year)
//     15-17     International Designator (Piece of launch)
//     19-20     Epoch Year (Last two digits of year)
//     21-32     Epoch (Julian Day and fractional portion of the day)
//     34-43     First Time Derivative of the Mean Motion
//               or Ballistic Coefficient (Depending on ephemeris type)
//     45-52     Second Time Derivative of Mean Motion (decimal point assumed;
//               blank if N/A)
//     54-61     BSTAR drag term if GP4 general perturbation theory was used.
//               Otherwise, radiation pressure coefficient.  (Decimal point assumed)
//     63-63     Ephemeris type
//     65-68     Element number
//     69-69     Check Sum (Modulo 10)
//               (Letters, blanks, periods, plus signs = 0; minus signs = 1)
//     Line 2
//     Column    Description
//     01-01     Line Number of Element Data
//     03-07     Satellite Number
//     09-16     Inclination [Degrees]
//     18-25     Right Ascension of the Ascending Node [Degrees]
//     27-33     Eccentricity (decimal point assumed)
//     35-42     Argument of Perigee [Degrees]
//     44-51     Mean Anomaly [Degrees]
//     53-63     Mean Motion [Revs per day]
//     64-68     Revolution number at epoch [Revs]
//     69-69     Check Sum (Modulo 10)
//        
//     All other columns are blank or fixed.
//          
// Example:
//      
// NOAA 6
// 1 11416U          86 50.28438588 0.00000140           67960-4 0  5293
// 2 11416  98.5105  69.3305 0012788  63.2828 296.9658 14.24899292346978

namespace Zeptomoby.OrbitTools
{
   /// <summary>
   /// This class encapsulates a single set of standard NORAD two-line elements.
   /// </summary>
   public class Tle
   {
      public enum Line
      {
         Zero = 0,
         One,
         Two
      };

      public enum Field
      {
         NoradNum,
         IntlDesc,
         SetNumber,     // TLE set number
         EpochYear,     // Epoch: Last two digits of year
         EpochDay,      // Epoch: Fractional Julian Day of year
         OrbitAtEpoch,  // Orbit at epoch
         Inclination,   // Inclination
         Raan,          // R.A. ascending node
         Eccentricity,  // Eccentricity
         ArgPerigee,    // Argument of perigee
         MeanAnomaly,   // Mean anomaly
         MeanMotion,    // Mean motion
         MeanMotionDt,  // First time derivative of mean motion
         MeanMotionDt2, // Second time derivative of mean motion
         BStarDrag      // BSTAR Drag
      }

      public enum Unit
      {
         Radians,   // radians
         Degrees,   // degrees
         Native     // TLE format native units (no conversion)
      }
 
      // Satellite name and two data lines
      private string m_strName;
      private string m_strLine1;
      private string m_strLine2;

      // Converted fields, in Double.Parse()-able form
      private Hashtable m_Field;

      // Cache of field values in "double" format. 
      // Key   - integer
      // Value - cached value
      private Hashtable m_Cache;

      // Generates key for cache
      private int Key(Unit u, Field f)
      { 
         return ((int)u * 100) + (int)f;
      }

      #region Construction and Initialization

      #region Column Offsets

      // Note: The column offsets are ZERO based.

      // Name
      private const int TLE_LEN_LINE_DATA      = 69; private const int TLE_LEN_LINE_NAME      = 22;

      // Line 1
      private const int TLE1_COL_SATNUM        =  2; private const int TLE1_LEN_SATNUM        =  5;
      private const int TLE1_COL_INTLDESC_A    =  9; private const int TLE1_LEN_INTLDESC_A    =  2;
      private const int TLE1_COL_INTLDESC_B    = 11; private const int TLE1_LEN_INTLDESC_B    =  3;
      private const int TLE1_COL_INTLDESC_C    = 14; private const int TLE1_LEN_INTLDESC_C    =  3;
      private const int TLE1_COL_EPOCH_A       = 18; private const int TLE1_LEN_EPOCH_A       =  2;
      private const int TLE1_COL_EPOCH_B       = 20; private const int TLE1_LEN_EPOCH_B       = 12;
      private const int TLE1_COL_MEANMOTIONDT  = 33; private const int TLE1_LEN_MEANMOTIONDT  = 10;
      private const int TLE1_COL_MEANMOTIONDT2 = 44; private const int TLE1_LEN_MEANMOTIONDT2 =  8;
      private const int TLE1_COL_BSTAR         = 53; private const int TLE1_LEN_BSTAR         =  8;
      private const int TLE1_COL_EPHEMTYPE     = 62; private const int TLE1_LEN_EPHEMTYPE     =  1;
      private const int TLE1_COL_ELNUM         = 64; private const int TLE1_LEN_ELNUM         =  4;

      // Line 2
      private const int TLE2_COL_SATNUM        = 2;  private const int TLE2_LEN_SATNUM        =  5;
      private const int TLE2_COL_INCLINATION   = 8;  private const int TLE2_LEN_INCLINATION   =  8;
      private const int TLE2_COL_RAASCENDNODE  = 17; private const int TLE2_LEN_RAASCENDNODE  =  8;
      private const int TLE2_COL_ECCENTRICITY  = 26; private const int TLE2_LEN_ECCENTRICITY  =  7;
      private const int TLE2_COL_ARGPERIGEE    = 34; private const int TLE2_LEN_ARGPERIGEE    =  8;
      private const int TLE2_COL_MEANANOMALY   = 43; private const int TLE2_LEN_MEANANOMALY   =  8;
      private const int TLE2_COL_MEANMOTION    = 52; private const int TLE2_LEN_MEANMOTION    = 11;
      private const int TLE2_COL_REVATEPOCH    = 63; private const int TLE2_LEN_REVATEPOCH    =  5;

      #endregion

      // //////////////////////////////////////////////////////////////////////////
      // Create from two-line elements data
      public Tle(string strName, string strLine1, string strLine2)
      {
         m_strName  = strName;
         m_strLine1 = strLine1;
         m_strLine2 = strLine2;

         Initialize();
      }

      // //////////////////////////////////////////////////////////////////////////
      public Tle(Tle tle) : 
         this(tle.Name, tle.Line1, tle.Line2)
      {
      }

      // //////////////////////////////////////////////////////////////////////////
      // Initialize()
      // Initialize the TLE object.
      private void Initialize()
      {
         m_Field = new Hashtable();
         m_Cache = new Hashtable();
   
         m_Field[Field.NoradNum] = m_strLine1.Substring(TLE1_COL_SATNUM, TLE1_LEN_SATNUM);
         m_Field[Field.IntlDesc] = m_strLine1.Substring(TLE1_COL_INTLDESC_A,
            TLE1_LEN_INTLDESC_A +
            TLE1_LEN_INTLDESC_B +   
            TLE1_LEN_INTLDESC_C);   
         m_Field[Field.EpochYear] = 
            m_strLine1.Substring(TLE1_COL_EPOCH_A, TLE1_LEN_EPOCH_A);

         m_Field[Field.EpochDay] = 
            m_strLine1.Substring(TLE1_COL_EPOCH_B, TLE1_LEN_EPOCH_B);
   
         if (m_strLine1[TLE1_COL_MEANMOTIONDT] == '-')
         {
            // value is negative
            m_Field[Field.MeanMotionDt] = "-0";
         }
         else
            m_Field[Field.MeanMotionDt] = "0";
   
         m_Field[Field.MeanMotionDt] += m_strLine1.Substring(TLE1_COL_MEANMOTIONDT + 1,
            TLE1_LEN_MEANMOTIONDT);
   
         // decimal point assumed; exponential notation
         m_Field[Field.MeanMotionDt2] = 
            ExpToDecimal(m_strLine1.Substring(TLE1_COL_MEANMOTIONDT2,
            TLE1_LEN_MEANMOTIONDT2));

         // decimal point assumed; exponential notation
         m_Field[Field.BStarDrag] = 
            ExpToDecimal(m_strLine1.Substring(TLE1_COL_BSTAR, TLE1_LEN_BSTAR));
         //TLE1_COL_EPHEMTYPE      
         //TLE1_LEN_EPHEMTYPE   

         m_Field[Field.SetNumber] = 
            m_strLine1.Substring(TLE1_COL_ELNUM, TLE1_LEN_ELNUM).TrimStart();

         // TLE2_COL_SATNUM         
         // TLE2_LEN_SATNUM         
   
         m_Field[Field.Inclination] = 
            m_strLine2.Substring(TLE2_COL_INCLINATION, TLE2_LEN_INCLINATION).TrimStart();
   
         m_Field[Field.Raan] = 
            m_strLine2.Substring(TLE2_COL_RAASCENDNODE, TLE2_LEN_RAASCENDNODE).TrimStart();

         // Eccentricity: decimal point is assumed
         m_Field[Field.Eccentricity] = "0." + m_strLine2.Substring(TLE2_COL_ECCENTRICITY,
            TLE2_LEN_ECCENTRICITY);
   
         m_Field[Field.ArgPerigee] = 
            m_strLine2.Substring(TLE2_COL_ARGPERIGEE, TLE2_LEN_ARGPERIGEE).TrimStart();
   
         m_Field[Field.MeanAnomaly] = 
            m_strLine2.Substring(TLE2_COL_MEANANOMALY, TLE2_LEN_MEANANOMALY).TrimStart();
   
         m_Field[Field.MeanMotion] = 
            m_strLine2.Substring(TLE2_COL_MEANMOTION, TLE2_LEN_MEANMOTION).TrimStart();
   
         m_Field[Field.OrbitAtEpoch] = 
            m_strLine2.Substring(TLE2_COL_REVATEPOCH, TLE2_LEN_REVATEPOCH).TrimStart();
      }

      #endregion

      #region Properties
      //
      // The properites are designed to be used with the property grid control
      //
      private const string CatTleData  = "TLE Data";
      private const string CatElements = "Elements";

      [Category(CatTleData)]
      public string Name  
      { 
         get { return m_strName;  }
      }

      [Category(CatTleData)]
      public string Line1 
      {
         get { return m_strLine1; }
      }

      [Category(CatTleData)]
      public string Line2 
      { 
         get { return m_strLine2; }
      }

      [Category(CatElements)]
      public string NoradNumber  
      { 
         get { return getField(Field.NoradNum, false); }
      }

      [Category(CatElements)]
      public string Eccentricity 
      { 
         get { return getField(Field.Eccentricity, false); }
      }

      [Category(CatElements)]
      public string Inclination  
      { 
         get { return getField(Field.Inclination, true); }
      }

      [Category(CatElements)]
      public string Epoch 
      { 
         get 
         { 
            return getField(Field.EpochYear).ToString() + 
                   getField(Field.EpochDay).ToString(); 
         }
      }

      [Category(CatElements)]
      public string IntlDescription 
      { 
         get { return getField(Field.IntlDesc, false); }
      }

      [Category(CatElements)]
      public string SetNumber 
      { 
         get { return getField(Field.SetNumber, false); }
      }

      [Category(CatElements)]
      public string OrbitAtEpoch 
      { 
         get { return getField(Field.OrbitAtEpoch, false); }
      }
         
      [Category(CatElements)]
      public string RAAscendingNode 
      { 
         get { return getField(Field.Raan, true); }
      }

      [Category(CatElements)]
      public string ArgPerigee 
      { 
         get { return getField(Field.ArgPerigee, true); }
      }

      [Category(CatElements)]
      public string MeanAnomaly 
      { 
         get { return getField(Field.MeanAnomaly, true); }
      }

      [Category(CatElements)]
      public string MeanMotion 
      { 
         get { return getField(Field.MeanMotion, true); }
      }

      [Category(CatElements)]
      public string MeanMotionDt 
      { 
         get { return getField(Field.MeanMotionDt, false); }
      }

      [Category(CatElements)]
      public string MeanMotionDt2 
      { 
         get { return getField(Field.MeanMotionDt2, false); }
      }

      [Category(CatElements)]
      public string BStarDrag 
      { 
         get { return getField(Field.BStarDrag, false); }
      }

      public Julian EpochJulian
      {
         get
         {
            int    epochYear = (int)getField(Tle.Field.EpochYear);
            double epochDay  =      getField(Tle.Field.EpochDay );

            if (epochYear < 57)
               epochYear += 2000;
            else
               epochYear += 1900;

            return new Julian(epochYear, epochDay);
         }
      }

      #endregion

      // /////////////////////////////////////////////////////////////////////
      /// <summary>
      /// Returns the requested TLE data field.
      /// </summary>
      /// <param name="fld">The field to return.</param>
      /// <returns>The requested field, in native form.</returns>
      public double getField(Field fld)
      {
         return getField(fld, Unit.Native);
      }

      // /////////////////////////////////////////////////////////////////////
      /// <summary>
      /// Returns the requested TLE data field as a type double.
      /// </summary>
      /// <remarks>
      /// The numeric return values are cached; requesting the same field 
      /// repeatedly incurs minimal overhead.
      /// </remarks>
      /// <param name="fld">The TLE field to retrieve.</param>
      /// <param name="units">Specifies the units desired.</param>
      /// <returns>
      /// The requested field's value, converted to the correct units if necessary.
      /// </returns>
      public double getField(Field fld, Unit units)
      {
         // Return cache contents if it exists, else populate cache.
         int key = Key(units, fld);

         if (m_Cache.ContainsKey(key))
         {
            // return cached value
            return (double)m_Cache[key];
         }
         else
         {
            // Value not in cache; add it
            double valNative = Double.Parse(m_Field[fld].ToString(), CultureInfo.InvariantCulture);
            double valConv   = ConvertUnits(valNative, fld, units); 
            m_Cache[key]     = valConv;

            return valConv;
         }
      }

      // /////////////////////////////////////////////////////////////////////
      /// <summary>
      /// Returns the requested TLE data field in native form as a text string.
      /// </summary>
      /// <param name="fld">The TLE field to retrieve.</param>
      /// <param name="AppendUnits">If true, the native units are appended to 
      /// the end of the returned string.</param>
      /// <returns>The requested field as a string.</returns>
      public string getField(Field fld, bool AppendUnits)
      {
         string str = m_Field[fld].ToString();
   
         if (AppendUnits)
         {
            str += getUnits(fld);
         }
      
         return str;
      }

      #region Utility

      // ///////////////////////////////////////////////////////////////////////////
      // Convert the given field into the requested units. It is assumed that
      // the value being converted is in the TLE format's "native" form.
      protected double ConvertUnits(double valNative, // value to convert
                                    Field fld,        // what field the value is
                                    Unit  units)      // what units to convert to
      {
         if (fld == Field.Inclination  ||
             fld == Field.Raan         ||
             fld == Field.ArgPerigee   ||
             fld == Field.MeanAnomaly)
         {
            // The native TLE format is DEGREES
            if (units == Unit.Radians)
            {
               return valNative * Globals.RADS_PER_DEG;
            }
         }

         return valNative;  // return value in unconverted native format
      }

      // ///////////////////////////////////////////////////////////////////////////
      protected string getUnits(Field fld) 
      {
         const string strDegrees    = " degrees";
         const string strRevsPerDay = " revs / day";

         switch (fld)
         {
            case Field.Inclination:
            case Field.Raan:
            case Field.ArgPerigee:
            case Field.MeanAnomaly:
               return strDegrees;

            case Field.MeanMotion:
               return strRevsPerDay;

            default:
               return string.Empty;   
         }
      }

      // //////////////////////////////////////////////////////////////////////////
      // ExpToDecimal()
      // Converts TLE-style exponential notation of the form [ |-]00000[+|-]0 to
      // decimal notation. Assumes implied decimal point to the left of the first
      // number in the string, i.e., 
      //       " 12345-3" =  0.00012345
      //       "-23429-5" = -0.0000023429   
      //       " 40436+1" =  4.0436
      protected static string ExpToDecimal(string str)
      {
         const int COL_SIGN     = 0;
         const int LEN_SIGN     = 1;

         const int COL_MANTISSA = 1;
         const int LEN_MANTISSA = 5;

         const int COL_EXPONENT = 6;
         const int LEN_EXPONENT = 2;

         string Sign     = str.Substring(COL_SIGN,     LEN_SIGN);
         string Mantissa = str.Substring(COL_MANTISSA, LEN_MANTISSA);
         string Exponent = str.Substring(COL_EXPONENT, LEN_EXPONENT);

         double val = Double.Parse(Sign +"0." + Mantissa + "e" + Exponent, CultureInfo.InvariantCulture);

         return val.ToString("f9");
      }

      // //////////////////////////////////////////////////////////////////////////
      // IsTleFormat()
      // Returns 'true' if "str" is a valid data line of a two-line element set,
      //   else false.
      //
      // To be valid a line must:
      //      Have as the first character the line number
      //      Have as the second character a blank
      //      Be TLE_LEN_LINE_DATA characters long
      //      Have a valid checksum (note: no longer required as of 12/96)
      //      
      public static bool IsValidLine(string str, Line line)
      {
         str.TrimStart();
         str.TrimEnd();
   
         int nLen = str.Length;
   
         if (nLen != TLE_LEN_LINE_DATA)
            return false;
   
         // First character in string must be a line number
         if ((str[0] - '0') != (int)line)
            return false;
   
         // Second char in string must be blank
         if (str[1] != ' ')
            return false;
   
         return true;
      }

      // //////////////////////////////////////////////////////////////////////////
      // CheckSum()
      // Calculate the check sum for a given line of TLE data, the last character
      // of which is the current checksum. (Although there is no check here,
      // the current checksum should match the one calculated.)
      // The checksum algorithm: 
      //    Each number in the data line is summed, modulo 10.
      //    Non-numeric characters are zero, except minus signs, which are 1.
      //
      static int CheckSum(string str)
      {
         // The length is "- 1" because we don't include the current (existing)
         // checksum character in the checksum calculation.
         int len  = str.Length - 1;
         int xsum = 0;
   
         for (int i = 0; i < len; i++)
         {
            char ch = str[i];

            if (Char.IsDigit(ch))
               xsum += (ch - '0');
            else if (ch == '-')
               xsum++;
         }
   
         return (xsum % 10);
   
      }

      #endregion
   }
}

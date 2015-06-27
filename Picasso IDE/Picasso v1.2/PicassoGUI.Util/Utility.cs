using System;
using System.Collections.Generic;
using System.Text;

namespace PicassoGUI.Util
{
    public static class Utility
    {
        public static Double HMS2UT(Double h, Double m, Double s)
        {
            return h + m / 60.0 + s / 3600.0;
        }

        public static Double YMDUT2JD(Double y, Double m, Double d, Double ut)
        {
            return 367.0 * y - Math.Floor(7.0 * (y + Math.Floor((m + 9.0) / 12.0)) / 4.0) + Math.Floor(275.0 * m / 9.0) + d + 1721013.5 + ut / 24.0;
        }
    }
}

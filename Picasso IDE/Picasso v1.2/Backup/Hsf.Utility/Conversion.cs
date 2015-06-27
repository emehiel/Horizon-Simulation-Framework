using System;
using System.Collections.Generic;
using System.Text;

namespace Hsf.Utility
{
    public static class Conversion
    {
        public static Double HMS2UT(Double h, Double m, Double s)
        {
            return h + m / 60.0 + s / 3600.0;
        }

        public static Double YMDUT2JD(Double y, Double m, Double d, Double ut)
        {
            return 367.0 * y - Math.Floor(7.0 * (y + Math.Floor((m + 9.0) / 12.0)) / 4.0) + Math.Floor(275.0 * m / 9.0) + d + 1721013.5 + ut / 24.0;
        }

        public static int Longitude2Pixel(double longitude, int width)
        {
            // Converts longitude (in degrees) to pixel X-coordinate, given map width
            return (int)((longitude + 180.0) * width / 360.0);
        }

        public static int Latitude2Pixel(double latitude, int height)
        {
            // Converts latitude (in degrees to pixel Y-coordiante, given map height; remember, up is positive in LLA
            return (int)(((-1.0 * latitude) + 90.0) * height / 180.0);
        }

        public static double Pixel2Longitude(int x, int width)
        {
            // Converts pixel X-coordinate to longitude measurement, given map width
            return (double)x * 360.0 / (double)width - 180.0;
        }

        public static double Pixel2Latitude(int y, int height)
        {
            // Converts pixel Y-coordinate to latitude measurement, given map height; remember, up is positive in LLA
            return -1.0 * ((double)y * 180.0 / (double)height - 90.0);
        }
    }
}

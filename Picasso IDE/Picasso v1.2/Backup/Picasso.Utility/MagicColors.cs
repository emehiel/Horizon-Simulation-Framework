using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;

namespace Picasso.Utility
{
    public static class MagicColors
    {
        static public Color RandomColor()
        {
            // Returns random color
            int seed = (int)(DateTime.UtcNow.TimeOfDay.Ticks);
            Random random = new Random(seed);
            return Color.FromArgb(random.Next(0, 255), random.Next(50, 255), random.Next(0, 255));
        }

        static public Color RandomColor(int threshold)
        {
            // Returns random color within a threshold of white and black
            int seed = (int)(DateTime.UtcNow.TimeOfDay.Ticks);
            Random random = new Random(seed);
            return Color.FromArgb(random.Next(threshold, 255 - threshold), random.Next(threshold, 255 - threshold), random.Next(threshold, 255 - threshold));
        }

        static public float ColorDistance(Color c1, Color c2)
        {
            // Defines the difference between the two colors between 0.0 (opposite) and 1.0 (identical)
            float max = (float)(Math.Sqrt(255.0 * 255.0 * 3.0));
            int rDiff = c1.R - c2.R;
            int gDiff = c1.G - c2.G;
            int bDiff = c1.B - c2.B;
            return 1.0f - (float)Math.Sqrt((double)(rDiff * rDiff + gDiff * gDiff + bDiff * bDiff)) / max;
        }

        static public Color RandomColor(Color prevColor, float tolerance)
        {
            Thread.Sleep(1000);
            Color newColor = RandomColor();
            while (ColorDistance(newColor, prevColor) < tolerance)
            {
                newColor = RandomColor();
            }
            return newColor;
        }
    }
}

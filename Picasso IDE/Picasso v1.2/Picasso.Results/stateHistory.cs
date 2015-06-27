using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using Picasso.Utility;

namespace Picasso.Results
{
    public class stateHistory
    {
        // Defines the history of a satellite's key states: time, position, coverage
        public List<statePoint> history;
        public Color color;
        public string name;

        public stateHistory()
        {
            color = MagicColors.RandomColor();
            history = new List<statePoint>();
            name = "unknown satellite";
        }

        public void addPoint(statePoint point)
        {
            history.Add(point);
            sort();
        }

        public void addPoint(float t, float latitude, float longitude, float coverage)
        {
            statePoint p = new statePoint();
            p.t = t;
            p.latitude = latitude;
            p.longitude = longitude;
            p.coverage = coverage;
            history.Add(p);
            sort();
        }

        private void sort()
        {
            // Bubble-sorts points by time history
            bool sorted = false;
            while (!sorted)
            {
                sorted = true;
                for (int i = 0; i < history.Count - 1; i++)
                {
                    if (history[i].t > history[i + 1].t)
                    {
                        statePoint tmp = history[i];
                        history[i] = history[i + 1];
                        history[i + 1] = tmp;
                        sorted = false;
                    }
                }
            }
        }
    }
}

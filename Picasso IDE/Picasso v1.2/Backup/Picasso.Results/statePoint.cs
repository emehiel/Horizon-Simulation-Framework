using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Picasso.Results
{
    public class statePoint
    {
        // Defines a time-point of required state information (position); units in sec, km
        public float t;
        public float longitude;
        public float latitude;
        public float coverage;

        public statePoint()
        {
            t = 0.0f;
            longitude = 0.0f;
            latitude = 0.0f;
            coverage = 0.0f;
        }
    }
}

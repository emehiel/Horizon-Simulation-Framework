using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Picasso.Results
{
    class aTexCoord {
	    public float u;
	    public float v;

        public aTexCoord()
        {
            u = 0.0f;
            v = 0.0f;
        }

        public aTexCoord(float newU, float newV)
        {
            u = newU;
            v = newV;
        }

        public void Set(float newU, float newV)
        {
            u = newU;
            v = newV;
        }
    }
}

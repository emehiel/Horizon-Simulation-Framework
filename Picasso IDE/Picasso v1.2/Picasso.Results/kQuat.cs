using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Picasso.Results
{
    class kQuat
    {
        public float scl;
        public float i;
        public float j;
        public float k;

        public kQuat()
        {
            scl = 0.0f;
            i = 1.0f;
            j = 0.0f;
            k = 0.0f;
        }

        public kQuat(kQuat o1)
        {
            scl = o1.scl;
            i = o1.i;
            j = o1.j;
            k = o1.k;
        }

        public void convertValuedToOperational()
        {
            // Convert a valued rotational quaternion into an operational one
            float tmp = (float)Math.Sin(scl / 2.0f);
            scl = (float)Math.Cos(scl / 2.0f);
            i = tmp * i;
            j = tmp * j;
            k = tmp * k;
        }

        public void convertOperationalToValued()
        {
            // Convert a operational rotational quaternion into a valued one
            if (scl < -1.0f) scl = -1.0f;
            if (scl > 1.0f) scl = 1.0f;
            scl = 2.0f * (float)Math.Acos(scl);
            float tmp = (float)Math.Sin(scl / 2.0f);
            i = i / tmp;
            j = j / tmp;
            k = k / tmp;
        }

        public static kQuat operator *(kQuat o1, kQuat o2)
        {
            kQuat toReturn = new kQuat();
            toReturn.scl = o1.scl * o2.scl - o1.i * o2.i - o1.j * o2.j - o1.k * o2.k;
            toReturn.i = o1.scl * o2.i + o1.i * o2.scl + o1.j * o2.k - o1.k * o2.j;
            toReturn.j = o1.scl * o2.j - o1.i * o2.k + o1.j * o2.scl + o1.k * o2.i;
            toReturn.k = o1.scl * o2.k + o1.i * o2.j - o1.j * o2.i + o1.k * o2.scl;
            return toReturn;
        }

        public void normalize()
        {
            float mag = (float)Math.Sqrt(i * i + j * j + k * k);
            i = i / mag;
            j = j / mag;
            k = k / mag;
        }
    }
}

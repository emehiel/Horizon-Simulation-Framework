using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Picasso.Results
{
    public class aVertex
    {
        // Members (location)
        public float x;
        public float y;
        public float z;

        // Constructors
        public aVertex()
        {
            x = 0.0f; y = 0.0f; z = 0.0f;
        }

        public aVertex(float px, float py, float pz) {
            x = px; y = py; z = pz; 
        }

        // Vertex accessors
        public void set(float px, float py, float pz)
        {
            x = px; y = py; z = pz;
        }

        // Operators
        public float mag()
        {
            return (float)Math.Sqrt(x * x + y * y + z * z);
        }

        public static aVertex operator -(aVertex o1, aVertex o2)
        {
            aVertex toReturn = new aVertex();
            toReturn.set(o1.x - o2.x, o1.y - o2.y, o1.z - o2.z);
            return toReturn;
        }

        public static aVertex operator +(aVertex o1, aVertex o2)
        {
            aVertex toReturn = new aVertex();
            toReturn.set(o1.x + o2.x, o1.y + o2.y, o1.z + o2.z);
            return toReturn;
        }

        public static aVertex operator *(aVertex o1, float o2)
        {
            aVertex toReturn = new aVertex();
            toReturn.set(o1.x * o2, o1.y * o2, o1.z * o2);
            return toReturn;
        }

        public aVertex cross(aVertex operand)
        {
            aVertex toReturn = new aVertex();
            toReturn.set(y * operand.z - z * operand.y, z * operand.x - x * operand.z, x * operand.y - y * operand.x);
            return toReturn;
        }

        public aVertex norm()
        {
            aVertex toReturn = new aVertex();
            float mag = this.mag();
            toReturn.set(x / mag, y / mag, z / mag);
            return toReturn;
        }
    };
}

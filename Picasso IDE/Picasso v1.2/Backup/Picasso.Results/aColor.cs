using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CsOpenGl;

namespace Picasso.Results
{
    class aColor {
	    public float r;
	    public float g;
	    public float b;
	    public float a;
    	
        public aColor() { 
	        r = 1.0f; g = 1.0f; b = 1.0f; a = 1.0f; 
        }

        public void set(float fr, float fg, float fb, float fa) { 
	        r = fr; g = fg; b = fb; a = fa; 
        }

        public void set() { 
	        GL.glColor4f(r, g, b, a); 
        }

        public void apply()
        {
            float[] matAmbient = new float[4];
            matAmbient[0] = r; matAmbient[1] = g; matAmbient[2] = b; matAmbient[3] = a;
            float[] matDiffuse = new float[4];
            matDiffuse[0] = r + 0.3f; if (matDiffuse[0] > 1.0f) matDiffuse[0] = 1.0f;
            matDiffuse[1] = g + 0.3f; if (matDiffuse[1] > 1.0f) matDiffuse[1] = 1.0f;
            matDiffuse[2] = b + 0.3f; if (matDiffuse[2] > 1.0f) matDiffuse[2] = 1.0f;
            matDiffuse[3] = a + 0.3f; if (matDiffuse[3] > 1.0f) matDiffuse[3] = 1.0f;
            GL.glMaterialfv(GL.GL_FRONT, GL.GL_DIFFUSE, matDiffuse);
            GL.glMaterialfv(GL.GL_FRONT, GL.GL_AMBIENT, matAmbient);
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CsOpenGl;

namespace Picasso.Results
{
    public class aSkybox
    {
        private aMesh mesh;
        private float r;
        private int n;

        public float Radius
        {
            get { return r; }
            set
            {
                r = value;
                mesh.loadSphere(r, n);
            }
        }

        public int Resolution
        {
            get { return n; }
            set
            {
                n = value;
                mesh.loadSphere(r, n);
            }
        }

        public aSkybox()
        {
            mesh = new aMesh();
            r = 10.0f;
            n = 32;
            mesh.loadSphere(r, n);
        }

        public void render()
        {
            GL.glMatrixMode(GL.GL_MODELVIEW);
            GL.glLoadIdentity();
            mesh.render();
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CsOpenGl;

namespace Picasso.Results
{
    class aCamera
    {
        private float px;
        private float py;
        private float pz;

        private float ux;
        private float uy;
        private float uz;

        private float tx;
        private float ty;
        private float tz;

        private aSkybox skybox;

        private float _farClip;

        public float FieldAngle;
        public float AspectRatio;
        public float NearClip;

        public float FarClip
        {
            get { return _farClip; }
            set
            {
                _farClip = value;
                skybox.Radius = 0.85f * _farClip;
            }
        }

        public aCamera()
        {
            px = 0.0f; py = 0.0f; pz = 0.0f;
            ux = 0.0f; uy = 0.0f; uz = 0.0f;
            tx = 0.0f; ty = 0.0f; tz = 0.0f;
            skybox = new aSkybox();
            FieldAngle = (float)Math.PI / 2.0f;
            AspectRatio = 1.5f;
            NearClip = 0.1f;
            FarClip = 100.0f;
        }

        public void setTarget(float x, float y, float z)
        {
            tx = x; ty = y; tz = z;
        }

        public void setPosition(float x, float y, float z)
        {
            px = x; py = y; pz = z;
        }

        public void setUp(float x, float y, float z)
        {
            ux = x; uy = y; uz = z;
        }

        public void setRight(float x, float y, float z)
        {
            // Calculates up vector from right vector orientation, assuming position and target are constant
            float atX = tx - px;
            float atY = ty - py;
            float atZ = tz - pz;
            ux = y * atZ - z * atY;
            uy = z * atX - x * atZ;
            uz = x * atY - y * atX;
            float mag = (float)Math.Sqrt((float)(ux * ux + uy * uy + uz * uz));
            ux = ux / mag; uy = uy / mag; uz = uz / mag;
        }

        public void applyCamera()
        {
            // Set up projection matrix
            GL.glMatrixMode(GL.GL_PROJECTION);
            GL.glLoadIdentity();
            GLU.gluPerspective(FieldAngle * 360.0f / (2.0f * (float)Math.PI), AspectRatio, NearClip, FarClip);
            GLU.gluLookAt(px, py, pz, tx, ty, tz, ux, uy, uz);
            GL.glMatrixMode(GL.GL_MODELVIEW);

            // Render skybox (assume texture already bound)
            GL.glLoadIdentity();
            skybox.render();
        }

        public void rotateFocus(float dTht, float dPhi) {
	        // Rotates camera about focus point by given increments in spherical coordinates
	        float relX = px - tx;
	        float relY = py - ty;
	        float relZ = pz - tz;
	        float mag = (float)Math.Sqrt(relX*relX + relY*relY + relZ*relZ);
            float pi = (float)Math.PI;
            float twoPi = 2.0f * pi;

	        // Calculate and increment spherical coordinates
	        float tht = (float)Math.Atan(relZ / relX);
	        if (relX < 0.0f) {
		        if (relZ < 0.0f) {
			        tht += pi;
		        } else {
			        tht -= pi;
		        }
	        }
	        tht += dTht;
	        float phi = (float)Math.Acos(relY / mag) + dPhi;

	        // Safecheck bounds - theta is periodical, phi is singular
	        while (tht < 0.0) {
		        tht += twoPi;
	        }
	        while (tht > twoPi) {
		        tht -= twoPi;
	        }
	        float singularityTolerance = 0.01f;
	        if (phi < singularityTolerance) phi = singularityTolerance;
	        if (phi > pi - singularityTolerance) phi = pi - singularityTolerance;

	        // Translate back into cartesian and set new position
	        relX = mag * (float)Math.Cos(tht) * (float)Math.Sin(phi);
	        relY = mag * (float)Math.Cos(phi);
	        relZ = mag * (float)Math.Sin(tht) * (float)Math.Sin(phi);
            px = tx + relX; py = ty + relY; pz = tz + relZ;

	        // Recalculate up vector from right
	        float rightX = relZ;
	        float rightY = 0.0f;
	        float rightZ = -1.0f * relX;
	        float upX = relY * rightZ - relZ * rightY;
	        float upY = relZ * rightX - relX * rightZ;
	        float upZ = relX * rightY - relY * rightX;
	        mag = (float)Math.Sqrt(upX*upX + upY*upY + upZ*upZ);
            ux = upX / mag; uy = upY / mag; uz = upZ / mag;
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CsOpenGl;

namespace Picasso.Results
{
    class aObject
    {
        protected aVertex _location;
        protected kQuat _orientation;
        protected aColor _color;
        protected aVertex _linearVelocity;
        protected kQuat _angularVelocity;
        public aMesh Mesh;

        public aObject()
        {
            Mesh = new aMesh();
            Mesh.loadCube(1.0f);
            _location = new aVertex();
            _orientation = new kQuat();
            _color = new aColor();
            _linearVelocity = new aVertex();
            _angularVelocity = new kQuat();
        }

        public virtual void Render()
        {
            // Set up material properties from color
            float[] matAmbient = new float[4];
            matAmbient[0] = _color.r; matAmbient[1] = _color.g; matAmbient[2] = _color.b; matAmbient[3] = _color.a;
            float[] matDiffuse = new float[4];
            matDiffuse[0] = _color.r + 0.3f; if (matDiffuse[0] > 1.0f) matDiffuse[0] = 1.0f;
            matDiffuse[1] = _color.g + 0.3f; if (matDiffuse[1] > 1.0f) matDiffuse[1] = 1.0f;
            matDiffuse[2] = _color.b + 0.3f; if (matDiffuse[2] > 1.0f) matDiffuse[2] = 1.0f;
            matDiffuse[3] = _color.a + 0.3f; if (matDiffuse[3] > 1.0f) matDiffuse[3] = 1.0f;
            GL.glMaterialfv(GL.GL_FRONT, GL.GL_DIFFUSE, matDiffuse);
            GL.glMaterialfv(GL.GL_FRONT, GL.GL_AMBIENT, matAmbient);

            // Draw mesh
            GL.glPushMatrix(); {
                GL.glTranslatef(_location.x, _location.y, _location.z);
                GL.glRotatef(_orientation.scl * 360.0f / (2.0f * (float)Math.PI), _orientation.i, _orientation.j, _orientation.k);
                Mesh.render();
            } GL.glPopMatrix();
        }

        public virtual void Update(float dt)
        {
            _location = _location + _linearVelocity * dt;
            kQuat operationalAngularVelocity = new kQuat(_angularVelocity);
            operationalAngularVelocity.scl = operationalAngularVelocity.scl * dt;
            if (_angularVelocity.scl == 0.0f || _angularVelocity.scl == 2.0 * Math.PI) {
                // No angular velocity; keep old orientation
            } else if (_orientation.scl == 0.0f || _orientation.scl == 2.0 * Math.PI) {
                // Frame has not yet rotated; apply one step of velocity
                _orientation = operationalAngularVelocity;
            } else {
                // Convert both quaternions to operational; combine; revert to valued
                operationalAngularVelocity.convertValuedToOperational();
                _orientation.convertValuedToOperational();
                _orientation = operationalAngularVelocity * _orientation;
                _orientation.convertOperationalToValued();
            }

            while (_orientation.scl > 2.0 * Math.PI) {
                _orientation.scl = _orientation.scl - 2.0f * (float)Math.PI;
            }
            while(_orientation.scl < 0.0) {
                _orientation.scl = _orientation.scl + 2.0f * (float)Math.PI;
            }
            _orientation.normalize();    
        }

        public void SetPosition(float x, float y, float z) {
            _location.x = x;
            _location.y = y;
            _location.z = z;
        }

        public void SetOrientation(float ang, float x, float y, float z) {
            _orientation.scl = ang;
            _orientation.i = x;
            _orientation.j = y;
            _orientation.k = z;
        }

        public void SetLinearVelocity(float x, float y, float z) {
            _linearVelocity.x = x;
            _linearVelocity.y = y;
            _linearVelocity.z = z;
        }

        public void SetAngularVelocity(float ang, float x, float y, float z) {
            _angularVelocity.scl = ang;
            _angularVelocity.i = x;
            _angularVelocity.j = y;
            _angularVelocity.k = z;
        }

        public void SetColor(float r, float g, float b)
        {
            _color.r = r;
            _color.g = g;
            _color.b = b;
            _color.a = 1.0f;
        }
    }
}

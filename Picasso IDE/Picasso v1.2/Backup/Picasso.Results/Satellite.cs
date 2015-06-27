using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CsOpenGl;

namespace Picasso.Results
{
    class Satellite : aObject
    {
        private float _mass = 1.0f; // Doesn't actually matter vs earth
        private float _earthMass = 1000.0f;

        public Satellite()
        {
            Mesh.loadSatellite();
        }

        public override void Update(float dt)
        {
            // Assume earth is at 0, 0, 0
            float G = 0.003f; // Correct gravitational contant for unit skew
            float r = _location.mag();
            float gF = G * _mass * _earthMass / (r * r);
            aVertex gForce = _location.norm() * -1.0f * gF;
            _linearVelocity += gForce * (dt / _mass);
            base.Update(dt);
        }
    }
}

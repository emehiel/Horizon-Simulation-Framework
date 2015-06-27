using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CsOpenGl;

namespace Picasso.Results
{
    class aMesh {
        // Data
	    protected int _numVertices;
        protected List<aVertex> _vertices;
        protected List<aVertex> _normals;
        protected List<aTexCoord> _texCoords;

        public aMesh() {
	        _numVertices = 0;
	        _vertices = new List<aVertex>();
	        _normals = new List<aVertex>();
            _texCoords = new List<aTexCoord>();
        }

        public bool loadCube(float size) {
	        // Loads vertices defining size x size x size tetrahedral, where size is the length of one side
	        clear();
	        size = size / 2.0f;
        	
	        // Set up each corner vertex
	        aVertex vert0 = new aVertex(-size, size, size);
	        aVertex vert1 = new aVertex(size, size, size);
	        aVertex vert2 = new aVertex(size, -size, size);
	        aVertex vert3 = new aVertex(-size, -size, size);
	        aVertex vert4 = new aVertex(size, size, -size);
	        aVertex vert5 = new aVertex(-size, size, -size);
	        aVertex vert6 = new aVertex(-size, -size, -size);
	        aVertex vert7 = new aVertex(size, -size, -size);
        	
	        // Create vertex arrays
	        _numVertices = 36;
	        _vertices.Add(vert0); _vertices.Add(vert2); _vertices.Add(vert1); // Front face
	        _vertices.Add(vert0); _vertices.Add(vert3); _vertices.Add(vert2);
	        _vertices.Add(vert1); _vertices.Add(vert7); _vertices.Add(vert4); // Right face
	        _vertices.Add(vert1); _vertices.Add(vert2); _vertices.Add(vert7);
	        _vertices.Add(vert4); _vertices.Add(vert6); _vertices.Add(vert5); // Back face
	        _vertices.Add(vert4); _vertices.Add(vert7); _vertices.Add(vert6);
	        _vertices.Add(vert5); _vertices.Add(vert3); _vertices.Add(vert0); // Left face
	        _vertices.Add(vert5); _vertices.Add(vert6); _vertices.Add(vert3);
	        _vertices.Add(vert0); _vertices.Add(vert4); _vertices.Add(vert5); // Top face
	        _vertices.Add(vert0); _vertices.Add(vert1); _vertices.Add(vert4);
	        _vertices.Add(vert2); _vertices.Add(vert6); _vertices.Add(vert7); // Bottom face
	        _vertices.Add(vert2); _vertices.Add(vert3); _vertices.Add(vert6);

	        refreshNormals();
	        return true;
        }

        public bool loadTetra(float size) {
	        // Loads vertices defining size x size x size cube, where size is the length of one side.
	        clear();
	        float L = size / (float)Math.Sqrt(3.0f);
	        float l = 2 * size / 3;
        	
	        // Set up each corner vertex
	        aVertex vert0 = new aVertex(0.0f, l, 0.0f);
	        aVertex vert1 = new aVertex(size / 2, -l / 2, -L / 2);
	        aVertex vert2 = new aVertex(0.0f, -l / 2, L);
	        aVertex vert3 = new aVertex(-size / 2, -l / 2, -L / 2);
        	
	        // Create vertex arrays
	        _numVertices = 12;
	        _vertices.Add(vert0); _vertices.Add(vert2); _vertices.Add(vert1); // Right face
	        _vertices.Add(vert0); _vertices.Add(vert1); _vertices.Add(vert3); // Back face
	        _vertices.Add(vert0); _vertices.Add(vert3); _vertices.Add(vert2); // Left face
	        _vertices.Add(vert1); _vertices.Add(vert2); _vertices.Add(vert3); // Bottom face

	        refreshNormals();
	        return true;
        }

        public bool loadSphere(float size, int numFaces) {
	        // Loads vertices defining sphere with radius size and the given number of faces
            clear();
	        int nPhi = (int)(Math.Sqrt(numFaces / 2.0f));
	        int nTht = nPhi * 2;
	        float phi0, tht0, phi1, tht1;
	        _numVertices = 6 * nPhi * nTht;
        	
	        // Create triangles by rotating about sphere
	        for (int i = 0; i < nPhi; i++) {
		        // Construct vertices for each latitude
		        for (int j = 0; j < nTht; j++) {
                    // Redeclare to avoid reference crossover
                    aVertex vert0 = new aVertex();
                    aVertex vert1 = new aVertex();
                    aVertex vert2 = new aVertex();
                    aVertex vert3 = new aVertex();

                    // Construct vertices for each longitude, beginning with spherical coordinates
			        phi0 = (((float)i) / ((float)nPhi)) * (float)Math.PI;
                    phi1 = (((float)(i + 1)) / ((float)nPhi)) * (float)Math.PI;
                    tht0 = (((float)j) / ((float)nTht)) * 2 * (float)Math.PI;
                    tht1 = (((float)(j + 1)) / ((float)nTht)) * 2 * (float)Math.PI;
        			
			        // Create vertices in cartesian from spherical coordinates
                    vert0.set(size * (float)Math.Sin(phi0) * (float)Math.Cos(tht0), size * (float)Math.Cos(phi0), -1 * size * (float)Math.Sin(phi0) * (float)Math.Sin(tht0));
                    vert1.set(size * (float)Math.Sin(phi1) * (float)Math.Cos(tht0), size * (float)Math.Cos(phi1), -1 * size * (float)Math.Sin(phi1) * (float)Math.Sin(tht0));
                    vert2.set(size * (float)Math.Sin(phi1) * (float)Math.Cos(tht1), size * (float)Math.Cos(phi1), -1 * size * (float)Math.Sin(phi1) * (float)Math.Sin(tht1));
                    vert3.set(size * (float)Math.Sin(phi0) * (float)Math.Cos(tht1), size * (float)Math.Cos(phi0), -1 * size * (float)Math.Sin(phi0) * (float)Math.Sin(tht1));
        			
			        // Specify triangles
			        _vertices.Add(vert0); _vertices.Add(vert2); _vertices.Add(vert1);
			        _vertices.Add(vert0); _vertices.Add(vert3); _vertices.Add(vert2);
                    _texCoords.Add(new aTexCoord((float)(tht0 / (2.0f * Math.PI)), (float)(phi0 / Math.PI)));
                    _texCoords.Add(new aTexCoord((float)(tht1 / (2.0f * Math.PI)), (float)(phi1 / Math.PI)));
                    _texCoords.Add(new aTexCoord((float)(tht0 / (2.0f * Math.PI)), (float)(phi1 / Math.PI)));
                    _texCoords.Add(new aTexCoord((float)(tht0 / (2.0f * Math.PI)), (float)(phi0 / Math.PI)));
                    _texCoords.Add(new aTexCoord((float)(tht1 / (2.0f * Math.PI)), (float)(phi0 / Math.PI)));
                    _texCoords.Add(new aTexCoord((float)(tht1 / (2.0f * Math.PI)), (float)(phi1 / Math.PI)));
		        }
	        }
	        refreshNormals();
	        return true;
        }

        public bool loadSatellite()
        {
            // Load sample satellite mesh
            clear();

            // Main body
            // Top
            _vertices.Add(new aVertex(0.1f, 0.1f, 0.1f));
            _vertices.Add(new aVertex(0.1f, 0.1f, -0.1f));
            _vertices.Add(new aVertex(-0.1f, 0.1f, 0.1f));
            _vertices.Add(new aVertex(0.1f, 0.1f, -0.1f));
            _vertices.Add(new aVertex(-0.1f, 0.1f, -0.1f));
            _vertices.Add(new aVertex(-0.1f, 0.1f, 0.1f));

            // Right
            _vertices.Add(new aVertex(0.1f, 0.1f, 0.1f));
            _vertices.Add(new aVertex(0.1f, -0.1f, 0.1f));
            _vertices.Add(new aVertex(0.1f, -0.1f, -0.1f));
            _vertices.Add(new aVertex(0.1f, -0.1f, -0.1f));
            _vertices.Add(new aVertex(0.1f, 0.1f, -0.1f));
            _vertices.Add(new aVertex(0.1f, 0.1f, 0.1f));

            // Front
            _vertices.Add(new aVertex(0.1f, 0.1f, 0.1f));
            _vertices.Add(new aVertex(-0.1f, 0.1f, 0.1f));
            _vertices.Add(new aVertex(0.0f, 0.0f, 0.15f));
            _vertices.Add(new aVertex(-0.1f, 0.1f, 0.1f));
            _vertices.Add(new aVertex(-0.1f, -0.1f, 0.1f));
            _vertices.Add(new aVertex(0.0f, 0.0f, 0.15f));
            _vertices.Add(new aVertex(-0.1f, -0.1f, 0.1f));
            _vertices.Add(new aVertex(0.1f, -0.1f, 0.1f));
            _vertices.Add(new aVertex(0.0f, 0.0f, 0.15f));
            _vertices.Add(new aVertex(0.1f, -0.1f, 0.1f));
            _vertices.Add(new aVertex(0.1f, 0.1f, 0.1f));
            _vertices.Add(new aVertex(0.0f, 0.0f, 0.15f));

            // Bottom
            _vertices.Add(new aVertex(0.1f, -0.1f, 0.1f));
            _vertices.Add(new aVertex(-0.1f, -0.1f, 0.1f));
            _vertices.Add(new aVertex(0.1f, -0.1f, -0.1f));
            _vertices.Add(new aVertex(-0.1f, -0.1f, 0.1f));
            _vertices.Add(new aVertex(-0.1f, -0.1f, -0.1f));
            _vertices.Add(new aVertex(0.1f, -0.1f, -0.1f));

            // Back
            _vertices.Add(new aVertex(-0.1f, 0.1f, -0.1f));
            _vertices.Add(new aVertex(0.1f, 0.1f, -0.1f));
            _vertices.Add(new aVertex(-0.1f, -0.1f, -0.1f));
            _vertices.Add(new aVertex(-0.1f, -0.1f, -0.1f));
            _vertices.Add(new aVertex(0.1f, 0.1f, -0.1f));
            _vertices.Add(new aVertex(0.1f, -0.1f, -0.1f));

            // Left
            _vertices.Add(new aVertex(-0.1f, 0.1f, 0.1f));
            _vertices.Add(new aVertex(-0.1f, 0.1f, -0.1f));
            _vertices.Add(new aVertex(-0.1f, -0.1f, 0.1f));
            _vertices.Add(new aVertex(-0.1f, -0.1f, 0.1f));
            _vertices.Add(new aVertex(-0.1f, 0.1f, -0.1f));
            _vertices.Add(new aVertex(-0.1f, -0.1f, -0.1f));

            // Right panel
            // Joint
            _vertices.Add(new aVertex(0.1f, 0.0f, 0.0f));
            _vertices.Add(new aVertex(0.3f, -0.2f, 0.05f));
            _vertices.Add(new aVertex(0.3f, 0.2f, 0.05f));
            _vertices.Add(new aVertex(0.3f, 0.2f, 0.05f));
            _vertices.Add(new aVertex(0.3f, 0.2f, -0.05f));
            _vertices.Add(new aVertex(0.1f, 0.0f, 0.0f));
            _vertices.Add(new aVertex(0.3f, 0.2f, -0.05f));
            _vertices.Add(new aVertex(0.3f, -0.2f, -0.05f));
            _vertices.Add(new aVertex(0.1f, 0.0f, 0.0f));
            _vertices.Add(new aVertex(0.3f, -0.2f, -0.05f));
            _vertices.Add(new aVertex(0.1f, 0.0f, 0.0f));
            _vertices.Add(new aVertex(0.3f, -0.2f, 0.05f));

            // Front
            _vertices.Add(new aVertex(1.0f, -0.2f, 0.05f));
            _vertices.Add(new aVertex(1.0f, 0.2f, 0.05f));
            _vertices.Add(new aVertex(0.3f, 0.2f, 0.05f));
            _vertices.Add(new aVertex(0.3f, 0.2f, 0.05f));
            _vertices.Add(new aVertex(0.3f, -0.2f, 0.05f));
            _vertices.Add(new aVertex(1.0f, -0.2f, 0.05f));

            // Right
            _vertices.Add(new aVertex(1.0f, 0.2f, -0.05f));
            _vertices.Add(new aVertex(1.0f, 0.2f, 0.05f));
            _vertices.Add(new aVertex(1.0f, -0.2f, -0.05f));
            _vertices.Add(new aVertex(1.0f, -0.2f, -0.05f));
            _vertices.Add(new aVertex(1.0f, 0.2f, 0.05f));
            _vertices.Add(new aVertex(1.0f, -0.2f, 0.05f));

            // Top
            _vertices.Add(new aVertex(1.0f, 0.2f, 0.05f));
            _vertices.Add(new aVertex(1.0f, 0.2f, -0.05f));
            _vertices.Add(new aVertex(0.3f, 0.2f, 0.05f));
            _vertices.Add(new aVertex(0.3f, 0.2f, 0.05f));
            _vertices.Add(new aVertex(1.0f, 0.2f, -0.05f));
            _vertices.Add(new aVertex(0.3f, 0.2f, -0.05f));

            // Back
            _vertices.Add(new aVertex(1.0f, 0.2f, -0.05f));
            _vertices.Add(new aVertex(1.0f, -0.2f, -0.05f));
            _vertices.Add(new aVertex(0.3f, 0.2f, -0.05f));
            _vertices.Add(new aVertex(0.3f, 0.2f, -0.05f));
            _vertices.Add(new aVertex(1.0f, -0.2f, -0.05f));
            _vertices.Add(new aVertex(0.3f, -0.2f, -0.05f));

            // Bottom
            _vertices.Add(new aVertex(1.0f, -0.2f, 0.05f));
            _vertices.Add(new aVertex(0.3f, -0.2f, 0.05f));
            _vertices.Add(new aVertex(1.0f, -0.2f, -0.05f));
            _vertices.Add(new aVertex(1.0f, -0.2f, -0.05f));
            _vertices.Add(new aVertex(0.3f, -0.2f, 0.05f));
            _vertices.Add(new aVertex(0.3f, -0.2f, -0.05f));

            // Left panel
            // Joint
            _vertices.Add(new aVertex(-0.1f, 0.0f, 0.0f));
            _vertices.Add(new aVertex(-0.3f, 0.2f, 0.05f));
            _vertices.Add(new aVertex(-0.3f, -0.2f, 0.05f));
            _vertices.Add(new aVertex(-0.3f, 0.2f, 0.05f));
            _vertices.Add(new aVertex(-0.1f, 0.0f, 0.0f));
            _vertices.Add(new aVertex(-0.3f, 0.2f, -0.05f));
            _vertices.Add(new aVertex(-0.3f, 0.2f, -0.05f));
            _vertices.Add(new aVertex(-0.1f, 0.0f, 0.0f));
            _vertices.Add(new aVertex(-0.3f, -0.2f, -0.05f));
            _vertices.Add(new aVertex(-0.3f, -0.2f, -0.05f));
            _vertices.Add(new aVertex(-0.3f, -0.2f, 0.05f));
            _vertices.Add(new aVertex(-0.1f, 0.0f, 0.0f));

            // Front
            _vertices.Add(new aVertex(-1.0f, -0.2f, 0.05f));
            _vertices.Add(new aVertex(-0.3f, 0.2f, 0.05f));
            _vertices.Add(new aVertex(-1.0f, 0.2f, 0.05f));
            _vertices.Add(new aVertex(-0.3f, 0.2f, 0.05f));
            _vertices.Add(new aVertex(-1.0f, -0.2f, 0.05f));
            _vertices.Add(new aVertex(-0.3f, -0.2f, 0.05f));

            // Left
            _vertices.Add(new aVertex(-1.0f, 0.2f, -0.05f));
            _vertices.Add(new aVertex(-1.0f, -0.2f, -0.05f));
            _vertices.Add(new aVertex(-1.0f, 0.2f, 0.05f));
            _vertices.Add(new aVertex(-1.0f, -0.2f, -0.05f));
            _vertices.Add(new aVertex(-1.0f, -0.2f, 0.05f));
            _vertices.Add(new aVertex(-1.0f, 0.2f, 0.05f));

            // Top
            _vertices.Add(new aVertex(-1.0f, 0.2f, 0.05f));
            _vertices.Add(new aVertex(-0.3f, 0.2f, 0.05f));
            _vertices.Add(new aVertex(-1.0f, 0.2f, -0.05f));
            _vertices.Add(new aVertex(-0.3f, 0.2f, 0.05f));
            _vertices.Add(new aVertex(-0.3f, 0.2f, -0.05f));
            _vertices.Add(new aVertex(-1.0f, 0.2f, -0.05f));

            // Back
            _vertices.Add(new aVertex(-1.0f, 0.2f, -0.05f));
            _vertices.Add(new aVertex(-0.3f, 0.2f, -0.05f));
            _vertices.Add(new aVertex(-1.0f, -0.2f, -0.05f));
            _vertices.Add(new aVertex(-0.3f, 0.2f, -0.05f));
            _vertices.Add(new aVertex(-0.3f, -0.2f, -0.05f));
            _vertices.Add(new aVertex(-1.0f, -0.2f, -0.05f));

            // Bottom
            _vertices.Add(new aVertex(-1.0f, -0.2f, 0.05f));
            _vertices.Add(new aVertex(-1.0f, -0.2f, -0.05f));
            _vertices.Add(new aVertex(-0.3f, -0.2f, 0.05f));
            _vertices.Add(new aVertex(-1.0f, -0.2f, -0.05f));
            _vertices.Add(new aVertex(-0.3f, -0.2f, -0.05f));
            _vertices.Add(new aVertex(-0.3f, -0.2f, 0.05f));

            _numVertices = _vertices.Count;
            refreshNormals();
            return true;
        }

        public void clear() {
            _vertices.Clear();
            _normals.Clear();
            _texCoords.Clear();
        }

        public virtual void render() {
            GL.glBegin(GL.GL_TRIANGLES);
            {
		        for (int i = 0; i < _numVertices / 3; i++) {
			        GL.glNormal3f(_normals[i].x, _normals[i].y, _normals[i].z);
                    if (_texCoords.Count > 0)
                    {
                        GL.glTexCoord2f(_texCoords[3 * i].u, _texCoords[3 * i].v);
                    }
			        GL.glVertex3f(_vertices[3*i].x, _vertices[3*i].y, _vertices[3*i].z);
                    if (_texCoords.Count > 0)
                    {
                        GL.glTexCoord2f(_texCoords[3 * i + 1].u, _texCoords[3 * i + 1].v);
                    }
                    GL.glVertex3f(_vertices[3 * i + 1].x, _vertices[3 * i + 1].y, _vertices[3 * i + 1].z);
                    if (_texCoords.Count > 0)
                    {
                        GL.glTexCoord2f(_texCoords[3 * i + 2].u, _texCoords[3 * i + 2].v);
                    }
                    GL.glVertex3f(_vertices[3 * i + 2].x, _vertices[3 * i + 2].y, _vertices[3 * i + 2].z);
		        }
	        } GL.glEnd();
        }

        public void refreshNormals() {
	        // Calculate normals
            _normals.Clear();
	        aVertex one, two;
	        for (int i = 0; i < _numVertices / 3; i++) {
		        one = _vertices[3*i+1] - _vertices[3*i];
		        two = _vertices[3*i+2] - _vertices[3*i];
		        _normals.Add((two.cross(one)).norm());
	        }
        }
    }
}

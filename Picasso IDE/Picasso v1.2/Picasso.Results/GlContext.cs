using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using CsOpenGl;

namespace Picasso.Results
{
    public partial class GlContext : BaseGlContext
    {
        private long lastMs = 0;
        private float angle = 0;
        private aObject _earth = new aObject();
        private Satellite _sat = new Satellite();
        private float[] lightDiffuse = new float[4];
        private float[] lightAmbient = new float[4];
        private float[] lightPosition = new float[4];
        private aCamera camera;
        public uint[] textureData;

        public GlContext()
        {
            // Setup default light (sun)
            lightAmbient[0] = 0.8f; lightAmbient[1] = 0.8f; lightAmbient[2] = 0.8f; lightAmbient[3] = 1.0f;
            lightDiffuse[0] = 1.0f; lightDiffuse[1] = 1.0f; lightDiffuse[2] = 1.0f; lightDiffuse[3] = 1.0f;
            lightPosition[0] = 2.0f; lightPosition[1] = 2.0f; lightPosition[2] = 1.0f; lightPosition[3] = 2.0f;

            // Setup earth
            _earth.SetPosition(0.0f, 0.0f, 0.0f);
            _earth.SetOrientation(0.409f, 0.2f, 0.8f, 0.0f);
            _earth.SetAngularVelocity(0.45f, 0.2f, 0.8f, 0.0f);
            _earth.SetColor(0.7f, 0.7f, 0.7f);
            _earth.Mesh.loadSphere(2.0f, 8192);

            // Setup sample satellite
            _sat.SetPosition(0.0f, 0.0f, 3.0f);
            _sat.SetColor(0.8f, 0.4f, 0.2f);
            _sat.SetLinearVelocity(1.0f, 0.0f, 0.0f);
            _sat.SetAngularVelocity(0.0f, 0.4f, 0.2f, 0.1f);

            InitializeComponent();
            textureData = new uint[3];

            // Set up camera
            camera = new aCamera();
            camera.AspectRatio = (float)Width / (float)Height;
            camera.FarClip = 50.0f;
            camera.setPosition(1.0f, 1.0f, 5.0f);
            camera.setTarget(0.0f, 0.0f, 0.0f);
            camera.setRight(5.0f, 0.0f, -1.0f);
        }

        public void Render()
        {
            // Render OpenGL contents
            if (DC == 0 || RC == 0)
                return;
            WGL.wglMakeCurrent(DC, RC);

            // Clear buffers and load frame
            GL.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT);
            GL.glBindTexture(GL.GL_TEXTURE_2D, textureData[2]);
            camera.applyCamera();
            GL.glLoadIdentity();

            GL.glDisable(GL.GL_TEXTURE_2D);
            _sat.Render();
            GL.glEnable(GL.GL_TEXTURE_2D);
            GL.glBindTexture(GL.GL_TEXTURE_2D, textureData[0]);
            _earth.Render();

            // Flush and swap
            GL.glFlush();
            WGL.wglSwapBuffers(DC);
        }

        public void Renew()
        {
            // Update movement and changes with new dt
            if (lastMs == 0)
                lastMs = DateTime.Now.Ticks;
            long currentMs = DateTime.Now.Ticks;
            long milliseconds = (currentMs - lastMs) / 10000;
            lastMs = currentMs;
            angle += (float)(milliseconds) / 15.0f;

            float dt = (float)milliseconds / 1000.0f;
            _earth.Update(dt);
            _sat.Update(dt);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            Renew();
            Render();
        }

        protected override void GlInitialize(object sender, EventArgs e)
        {
            base.GlInitialize(sender, e);
            GL.glShadeModel(GL.GL_SMOOTH);
            GL.glClearDepth(1.0f);
            GL.glHint(GL.GL_PERSPECTIVE_CORRECTION_Hint, GL.GL_NICEST);
            GL.glEnable(GL.GL_TEXTURE_2D);
            GL.glLightfv(GL.GL_LIGHT1, GL.GL_AMBIENT, lightAmbient);
            GL.glLightfv(GL.GL_LIGHT1, GL.GL_DIFFUSE, lightDiffuse);
            GL.glLightfv(GL.GL_LIGHT1, GL.GL_POSITION, lightPosition);
            GL.glEnable(GL.GL_LIGHT1);
            GL.glEnable(GL.GL_LIGHTING);

            // Load textures
            GL.glGenTextures(3, textureData);

            Bitmap img = (Bitmap)global::Picasso.Results.Properties.Resources.land_shallow_topo_1024;
            Rectangle rect = new Rectangle(0, 0, img.Width, img.Height);
            System.Drawing.Imaging.BitmapData bmd = img.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadOnly, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
            GL.glBindTexture(GL.GL_TEXTURE_2D, textureData[0]);
            GL.glTexImage2D(GL.GL_TEXTURE_2D, 0, (int)GL.GL_RGB8, img.Width, img.Height, 0, GL.GL_BGR_EXT, GL.GL_UNSIGNED_byte, bmd.Scan0);
            GL.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MIN_FILTER, (int)GL.GL_LINEAR);
            GL.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MAG_FILTER, (int)GL.GL_LINEAR);
            img.UnlockBits(bmd);
            img.Dispose();

            Bitmap img2 = (Bitmap)global::Picasso.Results.Properties.Resources.starbox;
            Rectangle rect2 = new Rectangle(0, 0, img2.Width, img2.Height);
            System.Drawing.Imaging.BitmapData bmd2 = img2.LockBits(rect2, System.Drawing.Imaging.ImageLockMode.ReadOnly, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
            GL.glBindTexture(GL.GL_TEXTURE_2D, textureData[1]);
            GL.glTexImage2D(GL.GL_TEXTURE_2D, 0, (int)GL.GL_RGB8, img2.Width, img2.Height, 0, GL.GL_BGR_EXT, GL.GL_UNSIGNED_byte, bmd2.Scan0);
            GL.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MIN_FILTER, (int)GL.GL_LINEAR);
            GL.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MAG_FILTER, (int)GL.GL_LINEAR);
            img2.UnlockBits(bmd2);
            img2.Dispose();

            Bitmap img3 = (Bitmap)global::Picasso.Results.Properties.Resources.starbox;
            Rectangle rect3 = new Rectangle(0, 0, img3.Width, img3.Height);
            System.Drawing.Imaging.BitmapData bmd3 = img3.LockBits(rect3, System.Drawing.Imaging.ImageLockMode.ReadOnly, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
            GL.glBindTexture(GL.GL_TEXTURE_2D, textureData[2]);
            GL.glTexImage2D(GL.GL_TEXTURE_2D, 0, (int)GL.GL_RGB8, img3.Width, img3.Height, 0, GL.GL_BGR_EXT, GL.GL_UNSIGNED_byte, bmd3.Scan0);
            GL.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MIN_FILTER, (int)GL.GL_LINEAR);
            GL.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MAG_FILTER, (int)GL.GL_LINEAR);
            img3.UnlockBits(bmd3);
            img3.Dispose();
        }

        public void rotateView(float dTht, float dPhi)
        {
            camera.rotateFocus(dTht, dPhi);
        }
    }
}

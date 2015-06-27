using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace CsOpenGl
{
	public class BaseGlContext : UserControl
	{
        // Window, context handles, and other data members
		private uint m_uint_HWND = 0;
		private uint m_uint_DC = 0;
		private uint m_uint_RC = 0;
        public Container components = null;

        // Window, context accessors
        public uint HWND
		{
			get { return m_uint_HWND; }
		}
		public uint DC
		{
			get { return m_uint_DC; }
		}
		public uint RC
		{
			get { return m_uint_RC; }
		}
		
        // Constructors
        public BaseGlContext()
		{
			InitializeComponent();
			this.components = new System.ComponentModel.Container();
			this.Load += new EventHandler(GlInitialize);
			this.SizeChanged += new EventHandler(GlResize);
		}

        private void InitializeComponent()
        {
            this.Name = "BaseGlContext";
            this.Size = new System.Drawing.Size(300, 300);
        }

        // Overriden control behavior
		protected override void Dispose(bool disposing)
		{
			if (disposing)
			{
				if(m_uint_RC != 0)
				{
					WGL.wglDeleteContext(m_uint_RC);
					m_uint_RC = 0;
				}
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose(disposing);
		}

        protected override void OnPaintBackground(PaintEventArgs e)
		{ 
			// Overriding background painting prevents flickering without resorting to double-buffering
		}

        protected override void OnPaint(System.Windows.Forms.PaintEventArgs e)
        {
            // Check for context handles
            if (m_uint_DC == 0 || m_uint_RC == 0)
            {
                return;
            }

            // Don't render anything with base context; just clear and flush
            GL.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT);
            GL.glLoadIdentity();
            GL.glFlush();
            WGL.wglSwapBuffers(m_uint_DC);
        }

        // Context control event handlers
		protected virtual void GlInitialize(object sender, EventArgs e)
		{
            // Set up and declare contexts
			m_uint_HWND = (uint)this.Handle.ToInt32();
			m_uint_DC = WGL.GetDC(m_uint_HWND);
			WGL.wglSwapBuffers(m_uint_DC);

            // Define pixel format
            WGL.PIXELFORMATDESCRIPTOR pfd = new WGL.PIXELFORMATDESCRIPTOR();
			WGL.ZeroPixelDescriptor(ref pfd);
			pfd.nVersion = 1; 
			pfd.dwFlags = (WGL.PFD_DRAW_TO_WINDOW |  WGL.PFD_SUPPORT_OPENGL |  WGL.PFD_DOUBLEBUFFER); 
			pfd.iPixelType = (byte)(WGL.PFD_TYPE_RGBA);
			pfd.cColorBits = 32;
			pfd.cDepthBits = 32;
			pfd.iLayerType = (byte)(WGL.PFD_MAIN_PLANE);
			int pixelFormatIndex = 0;
			pixelFormatIndex = WGL.ChoosePixelFormat(m_uint_DC, ref pfd);

            // Check and set pixel format
			if (pixelFormatIndex == 0)
			{
				MessageBox.Show("Unable to retrieve pixel format");
				return;
			}
			if (WGL.SetPixelFormat(m_uint_DC, pixelFormatIndex, ref pfd) == 0)
			{
				MessageBox.Show("Unable to set pixel format");
				return;
			}

			// Initialize rendering context
			m_uint_RC = WGL.wglCreateContext(m_uint_DC);
			if (m_uint_RC == 0)
			{
				MessageBox.Show("Unable to get rendering context");
				return;
			}
			if (WGL.wglMakeCurrent(m_uint_DC, m_uint_RC) == 0)
			{
				MessageBox.Show("Unable to make rendering context current");
				return;
			}

			// Initialize OpenGl view and properties
			GlResize(null, null);
			GL.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			GL.glEnable(GL.GL_DEPTH_TEST);
			GL.glDepthFunc(GL.GL_LEQUAL);
			//GL.glEnable(GL.GL_CULL_FACE);
			GL.glCullFace(GL.GL_BACK);
		}

		protected virtual void GlResize(object sender, EventArgs e)
		{
            // Check for and refresh context handles
			if (m_uint_DC == 0 || m_uint_RC == 0)
				return;
			if (this.Width == 0 || this.Height == 0)
				return;
			WGL.wglMakeCurrent(m_uint_DC, m_uint_RC);

            // Reset view and model matrix stacks with new dimensions
			GL.glViewport(0, 0, this.Width, this.Height);
			GL.glMatrixMode(GL.GL_PROJECTION);
			GL.glLoadIdentity();
			GLU.gluPerspective(60.0, ((double)(this.Width) / (double)(this.Height)), 1.0, 1000.0);
			GL.glMatrixMode(GL.GL_MODELVIEW);
			GL.glLoadIdentity();
		}
	}
}

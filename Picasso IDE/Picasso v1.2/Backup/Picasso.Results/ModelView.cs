using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Picasso.Forms;
using CsOpenGl;

namespace Picasso.Results
{
    public partial class ModelView : PicassoSubform
    {
        private bool _isPanning;
        private Point _prevLoc;

        public ModelView(Form parent)
        {
            InitializeComponent();
            MdiParent = parent;
        }

        public override Hsf.Elements.HsfElement Element
        {
            get
            {
                return new ResultsElement();
            }
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            // Context must always be re-drawn; update, draw, and repeat (invalidation forces new OnPaint call)
            glContext1.Renew();
            glContext1.Render();
            Invalidate();
        }

        private void ModelView_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                _isPanning = true;
                _prevLoc = e.Location;
            }
        }

        private void ModelView_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                _isPanning = false;
            }
        }

        private void ModelView_MouseMove(object sender, MouseEventArgs e)
        {
            if (_isPanning)
            {
                Point diff = new Point(e.Location.X - _prevLoc.X, e.Location.Y - _prevLoc.Y);
                float xFactor = 0.001f;
                float yFactor = 0.002f;
                glContext1.rotateView((float)diff.X * xFactor, -(float)diff.Y * yFactor);
                _prevLoc.X = e.Location.X;
                _prevLoc.Y = e.Location.Y;
            }
        }
    }
}


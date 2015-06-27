using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Picasso.Controls
{
    public partial class SlideBar : UserControl
    {
        private float _value;
        public int barThickness;
        public int buttonRadius;

        public float Value
        {
            get { return _value; }
            set
            {
                if (value < 0.0f) {
                    _value = 0.0f;
                }
                else if (value > 1.0f)
                {
                    _value = 1.0f;
                }
                else
                {
                    _value = value;
                }
            }
        }

        public SlideBar()
        {
            InitializeComponent();
            barThickness = 4;
            buttonRadius = 4;
            _value = 0.1f;
            DoubleBuffered = true;
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            int yOffset = (Height - barThickness) / 2;
            Pen p = new Pen(Color.LightGreen);
            p.Width = barThickness;
            Point left = new Point(0, yOffset);
            Point right = new Point(Width, yOffset);
            e.Graphics.DrawLine(p, left, right);
            e.Graphics.DrawLine(p, new Point(0, 0), new Point(0, yOffset * 2));
            e.Graphics.DrawLine(p, new Point(Width, 0), new Point(Width, yOffset * 2));
            e.Graphics.DrawEllipse(p, Width * Value, yOffset - buttonRadius, buttonRadius * 2, buttonRadius);
        }
    }
}

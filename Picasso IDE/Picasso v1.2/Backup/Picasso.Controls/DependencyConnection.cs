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
    public partial class DependencyConnection : UserControl
    {
        public SubsystemIcon DependentIcon;
        public SubsystemIcon IndependentIcon;
        public Point OpenPoint;
        public int Thickness;
        private Color shade;

        public DependencyConnection()
        {
            DependentIcon = null;
            IndependentIcon = null;
            OpenPoint = new Point(0, 0);
            shade = Color.Black;
            BackColor = Color.Transparent;
            Thickness = 2;
            InitializeComponent();
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            // Draw line from right side of independent icon to left side of dependent icon, or between one icon and an open point
            Graphics g = e.Graphics;
            Pen p = new Pen(shade, (float)Thickness);
            Point p1 = new Point();
            Point p2 = new Point();

            // Check for point arrangement
            if (IndependentIcon != null && DependentIcon != null)
            {
                p1.X = IndependentIcon.Location.X + IndependentIcon.Width - Thickness;
                p1.Y = IndependentIcon.Location.Y + IndependentIcon.Height / 2;
                p2.X = DependentIcon.Location.X + Thickness;
                p2.Y = DependentIcon.Location.Y + DependentIcon.Height / 2;
            }
            else
            {
                if (IndependentIcon == null && DependentIcon == null)
                {
                    return;
                }
                else
                {
                    if (IndependentIcon == null)
                    {
                        p1 = OpenPoint;
                        p2.X = DependentIcon.Location.X + Thickness;
                        p2.Y = DependentIcon.Location.Y + DependentIcon.Height / 2;
                    }
                    else if (DependentIcon == null)
                    {
                        p1.X = IndependentIcon.Location.X + IndependentIcon.Width - Thickness;
                        p1.Y = IndependentIcon.Location.Y + IndependentIcon.Height / 2;
                        p2 = OpenPoint;
                    }
                    else
                    {
                        p1.X = IndependentIcon.Location.X + IndependentIcon.Width - Thickness;
                        p1.Y = IndependentIcon.Location.Y + IndependentIcon.Height / 2;
                        p2.X = DependentIcon.Location.X + Thickness;
                        p2.Y = DependentIcon.Location.Y + DependentIcon.Height / 2;
                    }
                }
            }

            // Draw line
            g.DrawLine(p, p1, p2);
        }

        private void DependencyConnection_MouseMove(object sender, MouseEventArgs e)
        {
            if (DependentIcon == null || IndependentIcon == null)
            {
                Refresh();
            }
        }
    }
}

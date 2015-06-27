using System;
using System.Collections.Generic;
using System.Drawing;
using System.Xml;
using System.Xml.Serialization;
using System.Windows.Forms;
using PicassoGUI.Util;

namespace PicassoGUI.Controls
{
    [Serializable]
    public class DependencyCollector : DesignItem
    {
        public DesignItem Input { get; set; }
        public String DependencyCombination { get; set; }

        #region Constructors

        public DependencyCollector()
        {
            this.DependencyCombination = "++";
        }

        public DependencyCollector(String name, Point location) : base(location, new Size(50, 100))
        {
            this.Properties = new DependencyCollectorProperties();
            ((DependencyCollectorProperties)this.Properties).Name = name;
            ((DependencyCollectorProperties)this.Properties).ID = Guid.NewGuid();
            this.DependencyCombination = "++";
            this.Size = new Size(50, 100);
            // Inputs
            Point p1 = Point.Add(this.Location, new Size(0, this.Height / 3));
            Point p2 = Point.Add(this.Location, new Size(0, this.Height * 2 / 3));
            // Output
            Point p3 = Point.Add(this.Location, new Size(this.Width - 10, this.Height / 2 - 5));
            this.Ports.Add(new Port(p1, new Point(-10, 0)));
            this.Ports.Add(new Port(p2, new Point(-10, 0)));
            this.Ports.Add(new Port(p3, new Point(10, 0)));
        }

        public override void DesignItem_Paint(object sender, Graphics g)
        {
            base.DesignItem_Paint(sender, g);

            Rectangle rc = this.Border;
            rc.Inflate(0, -5);

            Font font = new Font("Arial", 8);
            StringFormat sf = new StringFormat();
            sf.LineAlignment = StringAlignment.Near;
            sf.Alignment = StringAlignment.Near;

            for (int i = 0; i < this.Ports.Count - 1; i++)
            {
                g.DrawString(this.DependencyCombination.Substring(i, 1), font, new SolidBrush(Color.Black), Point.Add(this.Ports[i].Location, new Size(10, -3)));
            }
        }
        #endregion
        
        #region Helper methods

        public void AddPort()
        {
            Port p = new Port();
            p.Size = new Size(10, 10);
            p.ConnectionOffset = new Point(-10, 0);
            this.Ports.Insert(this.Ports.Count - 2, p);
            this.DependencyCombination += "+";
            this.PlacePorts();
        }

        protected override void PlacePorts()
        {
            int nPorts = this.Ports.Count;
            // Inputs
            Point pn;
            for (int p = 0; p < this.Ports.Count - 1; p++)
            {
                pn = Point.Add(this.Location, new Size(0, this.Height * (p + 1) / nPorts - 1));
                this.Ports[p].Location = pn;
            }
            //Point p1 = Point.Add(this.Location, new Size(0, this.Height / 3));
            //Point p2 = Point.Add(this.Location, new Size(0, this.Height * 2 / 3));
            // Output
            Point p3 = Point.Add(this.Location, new Size(this.Width - 11, this.Height / 2 - 5));
            //this.Ports[0].Location = p1;
            //this.Ports[1].Location = p2;
            this.Ports[nPorts - 1].Location = p3;
        }

        #endregion
    }
}
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using System.Xml;
using System.Xml.Serialization;

namespace PicassoGUI.Util
{
    [Serializable]
    public class Corner 
    {
        private Rectangle border;
        public Rectangle Border
        {
            get { return this.border; }
            set { this.border = value; }
        }

        public Point Location
        {
            get { return this.border.Location; }
            set { this.border.Location = value; }
        }
        public Size Size
        {
            get { return this.border.Size; }
            set { this.border.Size = value; }
        }
        public bool Selected { get; set; }
        [XmlIgnore]
        public Cursor Cursor { get; set; }
        public bool IsDragging
        {
            get { return this.dragging; }
            set { this.dragging = value; }
        }

        public bool IsVisible(Point p)
        {
            this.Selected = this.border.Contains(p);
            return this.Selected;
        }

        private bool dragging = false;

        public Corner()
        {
            this.Initialize();
        }

        public Corner(Point location)
        {
            this.Initialize();
            this.Location = location;
        }

        private void Initialize()
        {
            this.border = new Rectangle(new Point(0, 0), new Size(6, 6));
            this.Cursor = Cursors.Hand;
        }

        public void Corner_Paint(object sender, Graphics g)
        {
            Rectangle dr = new Rectangle(Point.Subtract(this.Location, new Size(this.Size.Width / 2, this.Size.Height / 2)), this.Size);
            if (this.IsDragging)
                g.FillRectangle(Brushes.Green, dr);
            else if (this.Selected)
                g.FillRectangle(Brushes.Red, dr);
            else
                g.FillRectangle(Brushes.Black, dr);
        }
    }
}

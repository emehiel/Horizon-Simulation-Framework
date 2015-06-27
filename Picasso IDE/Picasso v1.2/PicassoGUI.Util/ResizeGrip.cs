using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Windows.Forms;
using System.Xml.Serialization;

namespace PicassoGUI.Util
{
    [Serializable]
    public class ResizeGrip
    {
        [XmlIgnore]
        public Cursor Cursor { get; set; }

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

        private Rectangle border;
        public Rectangle Border
        {
            get { return this.border; }
            set
            {
                this.border.Location = value.Location;
                this.border.Size = value.Size;
            }
        }

        public ResizeGrip() {
            this.Cursor = Cursors.Hand;

        }

        public ResizeGrip(Cursor cursor, Point location, Size size)
        {
            this.Cursor = cursor;
            this.Border = new Rectangle(location, size);
        }

        public bool IsVisible(Point p)
        {
            return this.Border.Contains(p);
        }
    }
}

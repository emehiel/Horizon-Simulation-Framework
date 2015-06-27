using System;
using System.Drawing;
using System.Windows.Forms;
using System.Xml.Serialization;

//namespace PicassoGUI.Util
namespace PicassoGUI.Controls
{
    [Serializable]
    public class Port
    {
        private Rectangle border;
        private Point connectionLocation;
        private Point portEntryLocation;
        
        public Rectangle Border
        {
            get { return this.border; }
            set
            {
                this.border.Location = value.Location;
                this.border.Size = value.Size;
            }
        }
        public Guid PortID { get; set; }
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
        public int Width
        {
            get { return this.border.Width;}
            set { this.border.Width = value; }
        }
        public int Height
        {
            get { return this.border.Height;}
            set { this.border.Height = value; }
        }
        [XmlIgnore]
        public Cursor Cursor { get; set; }
        public Point Center
        {
            get
            {
                return new Point(this.border.Location.X + this.Width / 2,this.border.Location.Y + this.Height / 2);
            }
        }
        public Point ConnectionLocation
        {
            get { return Point.Add(this.Center, (Size)this.ConnectionOffset); }
            set { this.connectionLocation = value; }
        }
        public Point PortEntryLocation
        {
            get { return Point.Add(this.Center, new Size(this.ConnectionOffset.X*3, this.ConnectionOffset.Y*3)); }
            set { this.portEntryLocation = value; }
        }
        public Point ConnectionOffset { get; set; }
        public bool IsInput { get; set; }

        public Port()
        {
            this.Cursor = Cursors.Cross;
        }

        public Port(Point location, Point connectionOffset)
        {
            //this.delta = connectionLocation;
            this.Border = new Rectangle(location, new Size(10, 10));
            this.PortID = Guid.NewGuid();
            //this.entryDelta = Point.Add(connectionLocation, new Size(this.delta.X * 3, this.delta.Y * 3));
            //this.ConnectionLocation = Point.Add(this.Center, connectionLocation);
            this.ConnectionOffset = connectionOffset;
            this.Cursor = Cursors.Cross;
        }

        public bool IsVisible(Point p)
        {
            return this.Border.Contains(p);
        }
    }

}

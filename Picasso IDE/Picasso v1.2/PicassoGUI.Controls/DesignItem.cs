using System;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Xml;
using System.Xml.Serialization;
using PicassoGUI.Util;

namespace PicassoGUI.Controls
{
    [Serializable]
//    abstract public partial class DesignItem : UserControl
    // TODO:  The main reason to change this (and Subsystem/Port) to UserControl is for context menus...
    public partial class DesignItem
    {
        #region DesignItem Properties to Show

        public DesignItemProperties Properties { get; set; }

        #endregion

        #region DesignItem Class Properties

        public bool Selected { get; set; }

        private Rectangle border;
        public Rectangle Border
        {
            get { return this.border; }
            set { this.border = value; }
        }

        //public bool Selected { get; set; }
        public List<Port> Ports { get; set; }
        [XmlIgnore]
        public List<ResizeGrip> Grips { get; set; }
        public int BorderWidth { get; set; }
        public int BorderSelectedWidth { get; set; }
        
        [XmlIgnore]
        public Guid ID  // Inherited from Properties
        {
            get
            {
                return this.Properties.ID;
            }
            set
            {
                this.Properties.ID = value;
            }
        }
        [XmlIgnore]
        public String Name // Inherited from Properties
        {
            get
            {
                return this.Properties.Name;
            }
            set
            {
                this.Properties.Name = value;
            }
        }
        [XmlIgnore]
        public Point Location
        {
            get { return this.border.Location; }
            set
            {
                this.border.Location = value;
                if (this.Grips.Count > 0)
                    this.PlaceGrips();
                if (this.Ports.Count > 0)
                    this.PlacePorts();
            }
        }
        public Size MinimumSize { get; set; }
        [XmlIgnore]
        public int Width
        {
            get { return this.border.Width; }
            set { this.border.Width = value; }
        }
        [XmlIgnore]
        public int Height
        {
            get { return this.border.Height; }
            set { this.border.Height = value; }
        }
        [XmlIgnore]
        public Size Size
        {
            get { return this.border.Size; }
            set
            {
                this.border.Size = value;
                this.border.Width = value.Width;
                this.border.Height = value.Height;
            }
        }
        [XmlIgnore]
        public Port SelectedPort { get; set; }
        [XmlIgnore]
        public Point Center
        {
            get
            {
                return new Point(Point.Add(this.border.Location, this.border.Size).X / 2,
                           Point.Add(this.border.Location, this.border.Size).Y / 2);
            }
        }

        [XmlIgnore]
        public Cursor Cursor { get; set; }

        public bool IsDragging
        {
            get { return this.dragging; }
        }
        public bool IsResizing
        {
            get { return this.resizing; }
        }
        public bool IsStartingConnector
        {
            get { return this.startConnector; }
        }

        #endregion

        #region Private Memebers

        // Initial Size of Subsystem prior to Resize Event
        Size initialSize = new Size();

        Point p0 = new Point();
        Point s = new Point();
        private Point dragPoint;
        bool dragging = false;
        bool resizing = false;
        bool startConnector = false;
        
        #endregion

        #region Constructors

        public DesignItem()
        {
            this.Initialize();
            this.Name = "New DesignItem";
        }

        public DesignItem(Point location, Size size)
        {
            this.Initialize();
            this.Location = location;
            this.Size = size;
            PlaceGrips();
        }

        #endregion

        #region Events

        public virtual void DesignItem_Paint(object sender, Graphics g)// PaintEventArgs e)
        {
            Rectangle rc = this.border;
            rc.Inflate(-5, -5);
            
            if (this.GetType() == typeof(Subsystem))
                g.FillRectangle(new SolidBrush(((SubsystemProperties)this.Properties).Asset.Color), rc);
            else if (this.GetType() == typeof(DependencyCollector))
                g.FillRectangle(new SolidBrush(((DependencyCollectorProperties)this.Properties).Asset.Color), rc);

            if (Selected)
            {
                g.DrawRectangle(new Pen(Color.Black, this.BorderSelectedWidth), rc);
                foreach (Port port in Ports)
                {
                    if (port.IsInput)
                        g.FillRectangle(Brushes.CadetBlue, port.Border);
                    else
                        g.FillRectangle(Brushes.White, port.Border);

                    g.DrawRectangle(Pens.Black, port.Border);
                }
            }
            else
                g.DrawRectangle(new Pen(Color.Black, this.BorderWidth), rc);
        }
        public void BeginDrag(MouseEventArgs e)
        {
            dragPoint = Point.Subtract(e.Location, new Size(this.border.Location));
            dragging = true;
        }
        public void BeginResize(MouseEventArgs e)
        {
            s = this.Location;
            initialSize = this.Size;
            p0 = e.Location;
            resizing = true;
        }
        public void BeginConnection()
        {
            this.startConnector = false;
        }

        public void Drag(MouseEventArgs e)//DrawingCanvas sender, MouseEventArgs e)
        {
            //sender.Invalidate(this.border, false);
            
            this.Location = Point.Subtract(e.Location, new Size(dragPoint));
            this.PlacePorts();
            this.PlaceGrips();

            //sender.Invalidate(this.border, false);
            //foreach (Connector c in sender.Connectors)
            //    sender.Invalidate(c.Region, false);

        }
        
        public void Resize(MouseEventArgs e)//DrawingCanvas sender, MouseEventArgs e)
        {
            //sender.Invalidate(this.border, false);

            Point dp = Point.Subtract(e.Location, (Size)p0);
            this.Size = Size.Add(initialSize, new Size(dp));
            if (this.Width < this.MinimumSize.Width)
            {
                this.Width = this.MinimumSize.Width;
                if (this.Height < this.MinimumSize.Height)
                    this.Height = this.MinimumSize.Height;
            }
            else if (this.Height < this.MinimumSize.Height)
                this.Height = this.MinimumSize.Height;

            this.PlaceGrips();
            this.PlacePorts();

            //sender.Invalidate(this.border, false);
            //foreach (Connector c in sender.Connectors)
            //    sender.Invalidate(c.Region, false);
        }
        
        public void EndDrag()
        {
            dragging = false;
        }
        public void EndResize()
        {
            resizing = false;
        }

    #endregion

        #region Helper methods

        public bool IsVisible(Point p)
        {
            this.Selected = this.border.Contains(p);
            bool onPort = false;
            if (this.Selected)
            {
                foreach (Port port in this.Ports)
                    if (port.IsVisible(p))
                    {
                        this.Cursor = port.Cursor;
                        onPort = true;
                        this.SelectedPort = port;
                        break;
                    }
                if (!onPort)
                {
                    foreach (ResizeGrip grip in this.Grips)
                        if (grip.IsVisible(p))
                        {
                            this.Cursor = grip.Cursor;
                            break;
                        }
                        else
                            this.Cursor = Cursors.SizeAll;
                }
            }

            return this.Selected;
        }

        public void SetAction(Point p)
        {
            this.dragging = false;
            this.resizing = false;
            this.startConnector = false;

            if (this.Cursor == Cursors.SizeAll)
                this.dragging = true;
            else if (this.Cursor == this.Grips[0].Cursor)
                this.resizing = true;
            else if (this.Cursor == this.Ports[0].Cursor)
            {
                foreach (Port port in this.Ports)
                {
                    if (port.IsVisible(p))
                    {
                        if (!port.IsInput)
                        {
                            this.startConnector = true;
                            this.SelectedPort = port;
                            break;
                        }
                    }
                    else
                        this.SelectedPort = null;
                }
            }
        }

        public bool OnPort(Point p)
        {
            bool onPort = false;
            foreach (Port port in this.Ports)
            {
                if (port.IsVisible(p))
                {
                    this.SelectedPort = port;
                    onPort = true;
                    break;
                }
                else
                    this.SelectedPort = null;
            }
            return onPort;
        }

        protected virtual void PlacePorts()
        {
            /*
            Point p1 = Point.Add(this.Location, new Size(this.Width / 2 - 5, 0));
            Point p2 = Point.Add(this.Location, new Size(this.Width - 10, this.Height / 2 - 5));
            Point p3 = Point.Add(this.Location, new Size(this.Width / 2 - 5, this.Height - 10));
            Point p4 = Point.Add(this.Location, new Size(0, this.Height / 2 - 5));
            this.Ports[0].Location = p1;
            this.Ports[1].Location = p2;
            this.Ports[2].Location = p3;
            this.Ports[3].Location = p4;
            */
        }

        protected void PlaceGrips()
        {
            //Point p1 = Point.Add(this.Location, new Size(0, 0));
            //Point p2 = Point.Add(this.Location, new Size(this.Width - 10, 0));
            Point p3 = Point.Add(this.Location, new Size(this.Width - 10, this.Height - 10));
            //Point p4 = Point.Add(this.Location, new Size(0, this.Height - 10));
            //this.Grips[0].Location = p1;
            //this.Grips[1].Location = p2;
            //this.Grips[2].Location = p3;
            this.Grips[0].Location = p3;
            //this.Grips[3].Location = p4;
        }

        public override string ToString()
        {
            return this.Name;
        }

        private void Initialize()
        {
            this.Properties = new DesignItemProperties();
            this.Ports = new List<Port>();
            this.Grips = new List<ResizeGrip>();

            this.Location = new Point(0,0);
            //this.Cursor = Cursors.SizeAll;
            this.Selected = false;

            //this.Name.Location = new Point((this.Width - this.subsystemLabel.Width) / 2, (this.Height - this.subsystemLabel.Height) / 2);

            this.MinimumSize = new Size(100,80);
            this.Size = this.MinimumSize;
            this.BorderWidth = 1;
            this.BorderSelectedWidth = 2;

            Point p3 = Point.Add(this.Location, new Size(this.Width - 10, this.Height - 10));
            this.Grips.Add(new ResizeGrip(Cursors.SizeNWSE, p3, new Size(10, 10)));
        }
        
        #endregion
    }
}

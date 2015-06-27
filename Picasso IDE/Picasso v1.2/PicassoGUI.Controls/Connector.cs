using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Xml;
using System.Xml.Serialization;
using System.Runtime.Serialization;
using System.ComponentModel;
using PicassoGUI.Util;

namespace PicassoGUI.Controls
{
    [Serializable]
    public class Connector
    {
        public Guid OutputSubsystemID { get; set; }
        public Guid InputSubsystemID { get; set; }
        public Guid OutputPortID { get; set; }
        public Guid InputPortID { get; set; }
        public GraphicsPath Path { get; set; }
        public Guid ConnectorID { get; set; }
        public Point Start { get; set; }
        public Point End { get; set; }
        public bool Selected { get; set; }
        [XmlIgnore]
        public Cursor Cursor { get; set; }
        [XmlIgnore]
        public Port OutputPort { get; set; }
        [XmlIgnore]
        public Port InputPort { get; set; }
        [XmlIgnore]
        public DesignItem OutputSubsystem
        {
            get { return this.Dependency.Output; }
            set { this.Dependency.Output = value; }
        }
        [XmlIgnore]
        public DesignItem InputSubsystem
        {
            get { return this.Dependency.Input; }
            set { this.Dependency.Input = value; }
        }
        [XmlIgnore]
        public Region Region
        {
            get
            {
                // TODO:  Change this to Path.GetBounds()?
                float maxx = this.Path.PathPoints[0].X;
                float minx = this.Path.PathPoints[0].X;
                float maxy = this.Path.PathPoints[0].Y;
                float miny = this.Path.PathPoints[0].Y;
                foreach (PointF point in this.Path.PathPoints)
                {
                    if (point.X > maxx)
                        maxx = point.X;
                    if (point.X < minx)
                        minx = point.X;

                    if (point.Y > maxy)
                        maxy = point.Y;
                    if (point.Y < miny)
                        miny = point.Y;
                }
                float w = maxx - minx;
                float h = maxy - miny;
                RectangleF rc = new RectangleF(minx, miny, w, h);
                rc.Inflate((float)10.0, (float)10.0);
                return new Region(rc);
            }
        }
        public bool IsConnecting
        {
            get { return this.connecting; }
        }

        public Dependency Dependency { get; set; }

        public Point[] Points
        {
            get
            {
                Point[] p = new Point[this.Corners.Count];
                int i = 0;
                foreach (Corner c in this.Corners)
                {
                    p[i] = c.Location;
                    i++;
                }
                return p;
            }
            //set
            //{
            //    foreach(Point p in this.Points)
            //       this.Corners.
            //}
        }
        public List<SegmentType> ConnectorSegmentTypes { get; set; }
        public List<Corner> Corners { get; set; }
        public bool IsDraggingLine
        {
            get { return this.draggingLine; }
        }
        public bool IsDraggingCorner
        {
            get { return this.dragginCorner; }
        }

        private bool draggingLine = false;
        private bool dragginCorner = false;
        private bool connecting = false;

        private int draggingSegment = -1;

        private Corner selectedCorner = null;
        private Point dragPoint = new Point();

        // The pen used to draw an unconnected connection
        private Pen unconnectedPen = new Pen(Color.DarkGray, 1);
        private Pen connectedPen = new Pen(Color.Black, 1);
        private Pen couldConnectPen = new Pen(Color.DarkSlateGray, 1);
        private Pen selectedPen = new Pen(Color.Black, 2);
        private Pen visiblePen = new Pen(Color.Transparent, 5);

        private CustomLineCap inCap;
        private CustomLineCap outCap;

        public Connector()
        {
            InitializeComponent();
        }

        public Connector(Point start, Point end, DesignItem output, DesignItem input)
        {
            InitializeComponent();       
     
            this.Start = start;
            this.End = end;
            int m1x = (this.End.X - this.Start.X) / 2 + this.Start.X;
            Point mid1 = new Point(m1x, this.Start.Y);
            Point mid2 = new Point(m1x, this.End.Y);

            this.Corners.Add(new Corner(this.Start));
            this.ConnectorSegmentTypes.Add(SegmentType.horizontal);
            this.Corners.Add(new Corner(mid1));
            this.ConnectorSegmentTypes.Add(SegmentType.vertical);
            this.Corners.Add(new Corner(mid2));
            this.ConnectorSegmentTypes.Add(SegmentType.horizontal);
            this.Corners.Add(new Corner(this.End));

            this.Path.AddLines(this.Points);
            
            if (input == null)
            {
                this.InputSubsystem = null;
                this.InputSubsystemID = new Guid();
                this.InputPort = null;
                this.InputPortID = new Guid();
            }
            else
            {
                this.InputSubsystem = input;
                this.InputSubsystemID = input.ID;
                this.InputPort = input.SelectedPort;
                this.InputPortID = input.SelectedPort.PortID;
            }
            this.OutputSubsystem = output;
            this.OutputSubsystemID = output.ID;
            this.OutputPort = output.SelectedPort;
            this.OutputPortID = output.SelectedPort.PortID;
        }

        public void BeginConnect(MouseEventArgs e)
        {
            this.End = e.Location;
            this.connecting = true;
            
        }
        public void BeginCornerDrag(MouseEventArgs e)
        {
            this.dragPoint = e.Location;
            foreach (Corner c in this.Corners)
            {
                if (c.Selected)
                {
                    this.selectedCorner = c;
                    c.IsDragging = true;
                    break;
                }
            }
            //this.dragginCorner = true;
        }
        public void BeginLineDrag(MouseEventArgs e)
        {
            this.dragPoint = e.Location;
            this.draggingLine = true;
            this.draggingSegment = this.WhichSegmentIsVisible(e.Location);
        }

        public void Connecting(DrawingCanvas sender, MouseEventArgs e)
        {
            sender.Invalidate(this.Region, false);

            this.End = e.Location;

            sender.Invalidate(this.Region, false);
        }
        public void DraggingCorner(DrawingCanvas sender, MouseEventArgs e)
        {
            sender.Invalidate(this.Region, false);

            if (this.selectedCorner != null)
                this.selectedCorner.Location = e.Location;

            sender.Invalidate(this.Region, false);
        }
        public void DraggingLine(DrawingCanvas sender, MouseEventArgs e)
        {
            sender.Invalidate(this.Region, false);

            if (this.draggingSegment >= 0)
            {
                if (this.ConnectorSegmentTypes[this.draggingSegment] == SegmentType.vertical)
                {
                    if (this.draggingSegment == 0)
                    {
                        this.Corners.Insert(0, new Corner(this.Corners[0].Location));
                        if (this.ConnectorSegmentTypes[0] == SegmentType.horizontal)
                            this.ConnectorSegmentTypes.Insert(0, SegmentType.vertical);
                        else
                            this.ConnectorSegmentTypes.Insert(0, SegmentType.horizontal);
                        this.draggingSegment++;
                    }
                    else if (this.draggingSegment == this.ConnectorSegmentTypes.Count - 1)
                    {
                        this.Corners.Insert(draggingSegment + 1, new Corner(this.Corners.Last().Location));
                        if (this.ConnectorSegmentTypes[draggingSegment] == SegmentType.horizontal)
                            this.ConnectorSegmentTypes.Insert(draggingSegment + 1, SegmentType.vertical);
                        else
                            this.ConnectorSegmentTypes.Insert(draggingSegment + 1, SegmentType.horizontal);
                    }

                    this.Corners[this.draggingSegment].Location = new Point(e.X, this.Corners[this.draggingSegment].Location.Y);
                    this.Corners[this.draggingSegment + 1].Location = new Point(e.X, this.Corners[this.draggingSegment + 1].Location.Y);
                }
                else if (this.ConnectorSegmentTypes[this.draggingSegment] == SegmentType.horizontal)
                {
                    if (this.draggingSegment == 0)
                    {
                        this.Corners.Insert(0, new Corner(this.Corners[0].Location));
                        if (this.ConnectorSegmentTypes[0] == SegmentType.horizontal)
                            this.ConnectorSegmentTypes.Insert(0, SegmentType.vertical);
                        else
                            this.ConnectorSegmentTypes.Insert(0, SegmentType.horizontal);
                        this.draggingSegment++;
                    }
                    else if (this.draggingSegment == this.ConnectorSegmentTypes.Count - 1)
                    {
                        this.Corners.Insert(draggingSegment + 1, new Corner(this.Corners.Last().Location));
                        if (this.ConnectorSegmentTypes[draggingSegment] == SegmentType.horizontal)
                            this.ConnectorSegmentTypes.Insert(draggingSegment + 1, SegmentType.vertical);
                        else
                            this.ConnectorSegmentTypes.Insert(draggingSegment + 1, SegmentType.horizontal);
                    }
                    this.Corners[this.draggingSegment].Location = new Point(this.Corners[this.draggingSegment].Location.X, e.Y);
                    this.Corners[this.draggingSegment + 1].Location = new Point(this.Corners[this.draggingSegment + 1].Location.X, e.Y);
                }
            }

            sender.Invalidate(this.Region, false);
        }

        public void EndConnection(DesignItem sender, MouseEventArgs e)
        {
            if (this.OutputSubsystemID != sender.ID)
            {
                this.InputSubsystem = sender;
                this.InputSubsystemID = sender.ID;
                this.InputPort = sender.SelectedPort;
                this.InputPortID = sender.SelectedPort.PortID;
                this.connecting = false;
                //this.connected = true;
                this.End = this.InputPort.ConnectionLocation;
            }
        }
        public void EndCornerDrag()
        {
            //this.dragginCorner = false;
            foreach (Corner c in this.Corners)
                c.IsDragging = false;
            this.selectedCorner.IsDragging = false;
            this.selectedCorner = null;
            this.dragginCorner = false;
            //this.selectedCorner = -1;
        }
        public void EndLineDrag()
        {
            this.draggingLine = false;
            this.draggingSegment = -1;
        }

        public void Connector_Paint(object sender, Graphics e)// PaintEventArgs e)
        {
            this.DeterminePath();
            // Use this algorithm if the connector is on a port
            if (this.InputPort != null)
            {
                // Create the path to draw

                if (this.Selected)
                {
                    foreach (Corner c in this.Corners)
                        c.Corner_Paint(sender, e);
                }
                    //e.FillPath(Brushes.Black, this.cornerPath);
                
                e.DrawPath(this.connectedPen, this.Path);
            }
            else
                // Use this algorithm if the connector is NOT on a port.
                e.DrawPath(this.unconnectedPen, this.Path);
        }

        public bool IsVisible(Point p)
        {
            this.Selected = this.Path.IsOutlineVisible(p, visiblePen);
            return this.Selected;
        }

        public void SetAction(Point p)
        {
            bool onCorner = false;
            foreach (Corner c in this.Corners)
            {
                this.dragginCorner = c.IsVisible(p);
                if (this.dragginCorner)
                    break;
            }             
            if (!onCorner)
                this.draggingLine = this.Path.IsOutlineVisible(p, this.visiblePen);
        }

        private void DeterminePath()
        {
            this.Start = this.OutputPort.ConnectionLocation;

            int m1x = (this.End.X - this.Start.X) / 2 + this.Start.X;
            Point mid1 = new Point(m1x, this.Start.Y);
            Point mid2 = new Point(m1x, this.End.Y);

            this.Corners[0].Location = this.Start;
            if (this.IsConnecting)
            {
                this.Corners[1].Location = mid1;
                this.Corners[2].Location = mid2;
            }
            
            if (this.InputPort != null)
            {
                int cons = this.Corners.Count - 2;
                this.End = this.InputPort.ConnectionLocation;
                if (this.InputSubsystem.IsDragging || this.InputSubsystem.IsResizing)
                {
                    if (this.ConnectorSegmentTypes[cons] == SegmentType.horizontal)
                        this.Corners[cons].Location = new Point(this.Corners[cons].Location.X, this.End.Y);
                    else
                        this.Corners[cons].Location = new Point(this.End.X, this.Corners[cons].Location.Y);
                }
            }
            if (this.OutputPort != null)
            {
                this.Start = this.OutputPort.ConnectionLocation;
                if (this.OutputSubsystem.IsDragging || this.OutputSubsystem.IsResizing)
                {
                    if (this.ConnectorSegmentTypes[0] == SegmentType.vertical)
                        this.Corners[1].Location = new Point(this.Corners[0].Location.X, this.Corners[1].Location.Y);
                    else
                        this.Corners[1].Location = new Point(this.Corners[1].Location.X, this.Start.Y);
                }
            }
            
            
            if (this.IsDraggingCorner)
            {
                int cornerIndex = this.Corners.IndexOf(this.selectedCorner);

                if (cornerIndex - 2 < 0)
                {
                    this.Corners.Insert(1, new Corner(this.Corners[0].Location));
                    if (this.ConnectorSegmentTypes[0] == SegmentType.horizontal)
                        this.ConnectorSegmentTypes.Insert(0, SegmentType.vertical);
                    else
                        this.ConnectorSegmentTypes.Insert(0, SegmentType.horizontal);
                    cornerIndex++;
                }
                else if (cornerIndex + 2 >= this.Corners.Count)
                {
                    this.Corners.Insert(cornerIndex+1, new Corner(this.Corners.Last().Location));
                    if (this.ConnectorSegmentTypes[cornerIndex] == SegmentType.horizontal)
                        this.ConnectorSegmentTypes.Insert(cornerIndex+1, SegmentType.vertical);
                    else
                        this.ConnectorSegmentTypes.Insert(cornerIndex+1, SegmentType.horizontal);

                    cornerIndex++;
                }

                if (this.ConnectorSegmentTypes[cornerIndex - 1] == SegmentType.horizontal)
                {
                    this.Corners[cornerIndex - 1].Location = new Point(this.Corners[cornerIndex - 1].Location.X, this.Corners[cornerIndex].Location.Y);
                    this.Corners[cornerIndex + 1].Location = new Point(this.Corners[cornerIndex].Location.X, this.Corners[cornerIndex + 1].Location.Y);
                }
                else if (this.ConnectorSegmentTypes[cornerIndex - 1] == SegmentType.vertical)
                {
                    this.Corners[cornerIndex - 1].Location = new Point(this.Corners[cornerIndex].Location.X, this.Corners[cornerIndex - 1].Location.Y);
                    this.Corners[cornerIndex + 1].Location = new Point(this.Corners[cornerIndex + 1].Location.X, this.Corners[cornerIndex].Location.Y);
                }
            }
             
            this.Corners[this.Corners.Count - 1].Location = this.End;

            this.Path.Reset();
            this.Path.AddLines(this.Points);
        }

        private int WhichSegmentIsVisible(Point p)
        {
            GraphicsPath g = new GraphicsPath();
            int i = 0;
            for(i = 0; i < this.ConnectorSegmentTypes.Count; i++)
            {
                g.Reset();
                g.AddLine(this.Corners[i].Location, this.Corners[i + 1].Location);
                if (g.IsOutlineVisible(p, visiblePen))
                    break;
            }
            return i;
        }

        private void InitializeComponent()
        {
            this.OutputPort = new Port();
            this.InputPort = new Port();
            this.Dependency = new Dependency();
            this.Cursor = Cursors.Cross;
            this.Selected = false;
            this.ConnectorID = Guid.NewGuid();

            unconnectedPen.DashPattern = new float[] { 5f, 5f };

            // New startCap path for unconnected connector
            GraphicsPath outCapPath = new GraphicsPath();
            // Create the outline for our custom end cap.
            outCapPath.AddLine(new Point(2, 2), new Point(2, -2));
            outCapPath.AddLine(new Point(2, -2), new Point(-2, -2));
            outCapPath.AddLine(new Point(-2, -2), new Point(-2, 2));
            outCapPath.AddLine(new Point(-2, 2), new Point(2, 2));
            // Construct the end cap.
            outCap = new CustomLineCap(null, outCapPath);

            // New startCap path for unconnected connector
            GraphicsPath inCapPath = new GraphicsPath();
            // Create the outline for our custom end cap.
            inCapPath.AddLine(new Point(4, 0), new Point(0, 4));
            inCapPath.AddLine(new Point(0, 4), new Point(-4, 0));
            inCapPath.AddLine(new Point(-4, 0), new Point(4, 0));
            // Construct the end cap.
            inCap = new CustomLineCap(null, inCapPath);

            this.unconnectedPen.CustomStartCap = outCap;
            this.unconnectedPen.CustomEndCap = inCap;

            this.connectedPen.CustomStartCap = outCap;
            this.connectedPen.CustomEndCap = inCap;

            this.selectedPen.CustomStartCap = outCap;
            this.selectedPen.CustomEndCap = inCap;

            this.Path = new GraphicsPath();
            this.ConnectorSegmentTypes = new List<SegmentType>();
            //this.cornerPath = new GraphicsPath();
            this.Corners = new List<Corner>();
        }
    }
    public enum SegmentType
    {
        vertical,
        horizontal
    }
}
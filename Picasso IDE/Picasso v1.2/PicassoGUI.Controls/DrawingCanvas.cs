using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;
using PicassoGUI.Util;

namespace PicassoGUI.Controls
{
    public partial class DrawingCanvas : UserControl
    {
        public List<DesignItem> DesignItems;
        public List<Connector> Connectors;
        //public PropertyGrid PropertyGrid { get; set; }
        public DesignItem SelectedDesignItem { get { return this.selectedDesignItem; } }
        public Connector SelectedConnector { get { return this.selectedConnector; } }

        private DesignItem selectedDesignItem;
        private Connector selectedConnector;
        private Cursor defaultCursor = Cursors.Arrow;
        private Connector tempConnector = null;
        private Point _originalLocation;

        public DrawingCanvas()
        {
            InitializeComponent();

            this.DesignItems = new List<DesignItem>();
            this.Connectors = new List<Connector>();
        }

        public DrawingCanvas(PropertyGrid props)
        {
            InitializeComponent();

            this.DesignItems = new List<DesignItem>();
            this.Connectors = new List<Connector>();
            //this.PropertyGrid = props;
        }

        public void AddObject(object designItem)
        {
           this.DesignItems.Add((DesignItem)designItem);
        }

        public void AddConnector(Connector connector)
        {
            this.Connectors.Add(connector);
        }

        private void drawingCanvas_Paint(object sender, PaintEventArgs e)
        {
               foreach (DesignItem o in this.DesignItems)
                    o.DesignItem_Paint(this, e.Graphics);
                foreach (Connector c in this.Connectors)
                    c.Connector_Paint(this, e.Graphics);
                if (this.tempConnector != null)
                    this.tempConnector.Connector_Paint(this, e.Graphics);
        }

        private void drawingCanvas_MouseDown(object sender, MouseEventArgs e)
        {
            foreach (DesignItem o in this.DesignItems)
            {
                if (e.Button == MouseButtons.Left && o.IsVisible(e.Location))
                {
                    this.selectedDesignItem = o;
                    this.Capture = true;
                    o.SetAction(e.Location);

                    if (o.IsDragging)
                    {
                        o.BeginDrag(e);
                        _originalLocation = Location;
                        //this.PropertyGrid.SelectedObject = o.Properties;
                        break;
                    }
                    else if (o.IsResizing)
                    {
                        o.BeginResize(e);
                        break;
                    }
                    else if (o.IsStartingConnector)
                    {
                        this.tempConnector = new Connector(o.SelectedPort.ConnectionLocation, e.Location, o, null);
                        //this.Connectors.Add(tempConnector);
                        this.tempConnector.BeginConnect(e);
                        o.BeginConnection();
                        //this.PropertyGrid.SelectedObject = tempConnector.Dependency;
                        break;
                    }
                }
                else if (e.Button == MouseButtons.Right && o.IsVisible(e.Location))
                {
                    ((Subsystem)o).ShowContextMenu(PointToScreen(e.Location));
                }
            }
            foreach (Connector connector in this.Connectors)
            {
                connector.SetAction(e.Location);

                if (connector.Selected)
                    //this.PropertyGrid.SelectedObject = connector.Dependency;

                if (connector.IsDraggingCorner)
                {
                    connector.BeginCornerDrag(e);
                    break;
                }
                if (connector.IsDraggingLine)
                {
                    connector.BeginLineDrag(e);
                    break;
                }
            }
        }

        private void drawingCanvas_MouseMove(object sender, MouseEventArgs e)
        {
            bool onDesignItem = false;
            // TODO:  Do something about z-order...

            foreach (DesignItem o in this.DesignItems)
            {
                // update the border to reflect selected state
                //if (o.Selected)
                    this.Invalidate(o.Border, false);
                // update the cursor in the DrawingCanvas
                if (o.IsVisible(e.Location))
                {
                    this.Cursor = o.Cursor;
                    onDesignItem = true;
                    break;
                }
                else
                {
                    this.Cursor = this.defaultCursor;
                }
            }

            if (!onDesignItem)
            {
                foreach (Connector c in this.Connectors)
                {
                    if (c.Selected)
                        this.Invalidate(c.Region, false);

                    if (c.IsVisible(e.Location))
                    {
                        bool onCorner = false;
                        foreach (Corner corner in c.Corners)
                        {
                            if (corner.IsVisible(e.Location))
                            {
                                this.Cursor = corner.Cursor;
                                onCorner = true;
                                break;
                            }
                        }
                        if (!onCorner)
                        {
                            this.Cursor = c.Cursor;
                            this.selectedConnector = c;
                            break;
                        }
                    }
                    else
                    {
                        this.Cursor = this.defaultCursor;
                        this.selectedConnector = null;
                    }
                }
            }

            // Start the action!
            foreach (DesignItem o in this.DesignItems)
            {
                if (o.IsDragging)
                {
                    //o.Drag(this, e);
                    o.Drag(e);
                    break;
                }
                else if (o.IsResizing)
                {
                    //o.Resize(this, e);
                    o.Resize(e);
                    break;
                }
            }
            foreach (Connector c in this.Connectors)
            {
                if (c.IsDraggingCorner)
                {
                    c.DraggingCorner(this, e);
                    break;
                }
                if (c.IsDraggingLine)
                {
                    c.DraggingLine(this, e);
                    break;
                }
            }
            if (this.tempConnector != null)
                this.tempConnector.Connecting(this, e);

            this.Invalidate(true);
        }

        private void drawingCanvas_MouseUp(object sender, MouseEventArgs e)
        {
            foreach (DesignItem o in this.DesignItems)
            {
                if (o.IsDragging)
                {
                    o.EndDrag();
                }
                else if (o.IsResizing)
                {
                    o.EndResize();
                }
            }

            if (this.tempConnector != null)
            {
                if (this.tempConnector.IsConnecting)
                {
                    foreach (DesignItem o in this.DesignItems)
                        if (o.OnPort(e.Location))
                        {
                            this.tempConnector.EndConnection(o, e);
                            if (this.tempConnector.InputPort != null)
                            {
                                this.tempConnector.InputPort.IsInput = true;
                                this.tempConnector.OutputPort.IsInput = false;
                                this.AddConnector(tempConnector);
                                break;
                            }
                        }

                    //this.Invalidate(this.tempConnector.Region, false);
                    this.Invalidate(true);
                    this.tempConnector = null;
                }
            }
            foreach (Connector connector in this.Connectors)
            {
                if (connector.IsDraggingCorner)
                {
                    connector.EndCornerDrag();
                    break;
                }
                if (connector.IsDraggingLine)
                {
                    connector.EndLineDrag();
                    break;
                }
            }

        }

        private void DrawingCanvas_KeyDown(object sender, KeyEventArgs e)
        {
            if (this.Focused)
            {
                if (e.KeyCode == Keys.Down && this.selectedDesignItem.Location.Y < this.Parent.Size.Height - 50)
                    this.selectedDesignItem.Location += new Size(0, 1);
                else if (e.KeyCode == Keys.Up && this.selectedDesignItem.Location.Y > 0)
                    this.selectedDesignItem.Location += new Size(0, -1);
                else if (e.KeyCode == Keys.Left && this.selectedDesignItem.Location.X > 0)
                    this.selectedDesignItem.Location += new Size(-1, 0);
                else if (e.KeyCode == Keys.Right && this.selectedDesignItem.Location.X < this.Parent.Size.Width - 50)
                    this.selectedDesignItem.Location += new Size(1, 0);
                else if (e.KeyCode == Keys.Delete || e.KeyCode == Keys.Back)
                {
                    // TODO:  this would work better without multiple copies of the designItem List
                    if (this.selectedDesignItem != null)
                    {
                        this.DesignItems.Remove(this.selectedDesignItem);
                        this.Invalidate(this.selectedDesignItem.Border);
                        this.selectedDesignItem = null;
                    }
                    else if (this.selectedConnector != null)
                    {
                        this.Connectors.Remove(this.selectedConnector);
                        this.Invalidate(this.selectedConnector.Region);
                        this.selectedConnector = null;
                    }
                    //FIX
                    //((MainForm)this.ParentForm).HasChanged = true;
                }
            }
            if (this.selectedDesignItem != null)
            {
                Rectangle rt = this.selectedDesignItem.Border;
                rt.Inflate(10, 10);
                this.Invalidate(rt, false);
            }
            foreach (Connector c in this.Connectors)
                this.Invalidate(c.Region, false);
        }

        private void addToolStripMenuItem_Click(object sender, EventArgs e)
        {

            if (this.selectedDesignItem.GetType() == typeof(DependencyCollector))
            {
                ((DependencyCollector)this.selectedDesignItem).AddPort();

            }
            else if (this.selectedDesignItem.GetType() == typeof(Subsystem))
            {
                Point loc = this.PointToScreen(this.selectedDesignItem.Location);
                Point mLoc = MousePosition;
                ((Subsystem)this.selectedDesignItem).AddPort(mLoc.X < (loc.X + this.selectedDesignItem.Width / 2));
            }

            Rectangle rt = this.selectedDesignItem.Border;
            rt.Inflate(10, 10);
            this.Invalidate(rt, false);
            foreach (Connector c in this.Connectors)
                this.Invalidate(c.Region, false);
        }

        private void DrawingCanvas_DoubleClick(object sender, EventArgs e)
        {
            // Create and place new asset, subsystem
            Asset myAsset = new Asset("Sample Asset");
            Point location = new Point(100,100);
            Subsystem newSubsystem = new Subsystem(myAsset, "Sample Subsystem", location);
            AddObject((object)newSubsystem);

            // Open up subsystem window on double-click - but subsystem block will need to be a control, first

        }

    }
}
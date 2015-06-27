using System;
using System.Windows.Forms;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Text;
using System.Xml;
using System.Xml.Serialization;
using PicassoGUI.Util;
using MementoModel;

namespace PicassoGUI.Controls
{
    [Serializable]
    public class Subsystem : DesignItem, IMCloneable
    {
        private ContextMenuStrip subsystemContextMenu = new ContextMenuStrip();
        private ToolStripMenuItem cutSubToolStripMenuItem = new ToolStripMenuItem();
        private ToolStripMenuItem copySubToolStripMenuItem = new ToolStripMenuItem();
        private ToolStripMenuItem pasteSubToolStripMenuItem = new ToolStripMenuItem();
        private ToolStripMenuItem propertiesSubToolStripMenuItem = new ToolStripMenuItem();
        private ToolStripSeparator subContextMenuSeparator = new ToolStripSeparator();

        public String ScriptFileName
        {
            get
            {
                if (this.UsesCustomScriptFile)
                    return ((SubsystemProperties)this.Properties).CustomScriptFileName;
                else
                {
                    String temp = ((SubsystemProperties)this.Properties).Name;
                    temp = temp.Replace(" ", "_");
                    return temp + "_LUA.txt";
                }
            }
            set
            {
                if (this.UsesCustomScriptFile)
                    ((SubsystemProperties)this.Properties).CustomScriptFileName = value;
            }
        }
        public List<SubsystemParameter> Parameters { get; set; }

        public LUAScript InitFunction { get; set; }
        public LUAScript CanPerformFunction { get; set; }
        public LUAScript CanExtendFucntion { get; set; }
        public List<LUAScript> OtherFunctions { get; set; }

        public bool UsesCustomScriptFile { get; set; }

        #region Constructors

        public Subsystem()
        {
            this.Initialize();
            this.Properties = new SubsystemProperties();
            this.Properties.Name = "New Subsystem";
        }

        public Subsystem(Asset asset, String name, Point location) : base(location, new Size(100,100))
        {
            this.Initialize();
            ((SubsystemProperties)this.Properties).Asset = asset;
            this.Properties.Name = name;
            this.InitFunction.Name = this.Name.Replace(" ", "_") + "_init";
            this.CanExtendFucntion.Name = this.Name.Replace(" ", "_") + "_canExtend";
            this.CanPerformFunction.Name = this.Name.Replace(" ", "_") + "_canPerform";
        
            Point outP2 = Point.Add(this.Location, new Size(this.Width - 11, this.Height / 2 - 5));
            Point inP2 = Point.Add(this.Location, new Size(1, this.Height / 2 - 5));

            Port port1 = new Port(outP2, new Point(10, 0));
            Port port2 = new Port(inP2, new Point(-10, 0));
            port1.IsInput = false;
            port2.IsInput = true;
            this.Ports.Add(port1);
            this.Ports.Add(port2);
        }

        #endregion

        #region Helper methods

        public void ShowContextMenu(Point p)
        {
            this.subsystemContextMenu.Show(p);
        }

        public void AddPort(Boolean isInput)
        {
            Port p = new Port();
            p.Size = new Size(10, 10);
            p.IsInput = isInput;
            int portNumber = 0;

            if (isInput)
            {
                p.ConnectionOffset = new Point(10, 0);
            }
            else
            {
                p.ConnectionOffset = new Point(-10, 0);
                portNumber = this.Ports.Count - 1;
            }

            this.Ports.Insert(portNumber, p);
            this.PlacePorts();
        }

        public override void DesignItem_Paint(object sender, Graphics g)
        {
            base.DesignItem_Paint(sender, g);

            Rectangle rc = this.Border;
            rc.Inflate(-5, -5);

            Font font = new Font("Arial", 8);
            StringFormat sf = new StringFormat();
            sf.LineAlignment = StringAlignment.Center;
            sf.Alignment = StringAlignment.Center;

            g.DrawString(this.Name, font, new SolidBrush(Color.Black), rc, sf);
        }

        protected override void PlacePorts()
        {
            /*
            Point p1 = Point.Add(this.Location, new Size(this.Width / 2 - 5, 1));
            Point p2 = Point.Add(this.Location, new Size(this.Width - 11, this.Height / 2 - 5));
            Point p3 = Point.Add(this.Location, new Size(this.Width / 2 - 5, this.Height - 11));
            Point p4 = Point.Add(this.Location, new Size(1, this.Height / 2 - 5));
            this.Ports[0].Location = p1;
            this.Ports[1].Location = p2;
            this.Ports[2].Location = p3;
            this.Ports[3].Location = p4;
             */

            int inputs = 0;
            int outputs = 0;
            foreach (Port p in this.Ports)
                if (p.IsInput)
                    inputs++;

            outputs = this.Ports.Count - inputs;
            int x = 0;
            int y = 0;
            int ipcount = 1;
            int opcount = 1;

            foreach (Port p in this.Ports)
            {
                if (p.IsInput)
                {
                    x = this.Location.X + 1;
                    y = this.Location.Y + ipcount * this.Height / (inputs + 1) - 5;
                    ipcount++;
                }
                else
                {
                    x = this.Location.X + this.Width - 11;
                    y = this.Location.Y + opcount * this.Height / (outputs + 1) - 5;
                    opcount++;
                }

                p.Location = new Point(x, y);
            }

        }

        private void Initialize()
        {
            this.Properties = new SubsystemProperties();
            this.Properties.ID = Guid.NewGuid();
            this.Parameters = new List<SubsystemParameter>();
            this.UsesCustomScriptFile = false;

            this.InitFunction = new LUAScript();
            this.CanPerformFunction = new LUAScript();
            this.CanExtendFucntion = new LUAScript();
            this.OtherFunctions = new List<LUAScript>();

            this.cutSubToolStripMenuItem.Name = "cutSubToolStripMenuItem";
            this.cutSubToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.X)));
            this.cutSubToolStripMenuItem.Size = new System.Drawing.Size(150, 22);
            this.cutSubToolStripMenuItem.Text = "&Cut";
            this.cutSubToolStripMenuItem.Click += new EventHandler(copySubToolStripMenuItem_Click);

            this.copySubToolStripMenuItem.Name = "copySubToolStripMenuItem";
            this.copySubToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.C)));
            this.copySubToolStripMenuItem.Size = new System.Drawing.Size(150, 22);
            this.copySubToolStripMenuItem.Text = "&Copy";
            this.copySubToolStripMenuItem.Click += new EventHandler(copySubToolStripMenuItem_Click);

            this.pasteSubToolStripMenuItem.Name = "pasteSubToolStripMenuItem";
            this.pasteSubToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.V)));
            this.pasteSubToolStripMenuItem.Size = new System.Drawing.Size(150, 22);
            this.pasteSubToolStripMenuItem.Text = "&Paste";
            this.pasteSubToolStripMenuItem.Click += new EventHandler(copySubToolStripMenuItem_Click);

            this.subContextMenuSeparator.Name = "subContextMenuSeparator";

            this.propertiesSubToolStripMenuItem.Name = "propertiesSubToolStripMenuItem";
            //this.propertiesSubToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.C)));
            this.propertiesSubToolStripMenuItem.Size = new System.Drawing.Size(150, 22);
            this.propertiesSubToolStripMenuItem.Text = "&Properties";
            this.propertiesSubToolStripMenuItem.Click += new EventHandler(copySubToolStripMenuItem_Click);

            this.subsystemContextMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
                this.cutSubToolStripMenuItem,
                this.copySubToolStripMenuItem,
                this.pasteSubToolStripMenuItem,
                this.subContextMenuSeparator,
                this.propertiesSubToolStripMenuItem});
        }

        void copySubToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.CopyToClipboard();
        }


        public void CopyToClipboard()
        {
            // Copy this object to clipboard
        }
        #endregion

        #region Memento

        public object Clone()
        {
            // Return shallow copy of this object
            Subsystem copy = new Subsystem();
            copy.Location = Location;
            return copy;
        }

        public void Clone(object value)
        {
            // Shallow copies value to this object
            Subsystem castValue = (Subsystem)value;
            Location = castValue.Location;
        }

        #endregion
    }
}

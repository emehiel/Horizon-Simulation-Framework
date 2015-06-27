using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Hsf.Elements;
using Picasso.Dialogs;

namespace Picasso.Controls
{
    public partial class DrawingCanvas : UserControl
    {
        public List<SubsystemIcon> SubsystemIcons;
        public List<DependencyConnection> Connections;
        private DependencyConnection floater;

        public DrawingCanvas()
        {
            InitializeComponent();
            SubsystemIcons = new List<SubsystemIcon>();
            Connections = new List<DependencyConnection>();
        }

        private void DrawingCanvas_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            // If drawing canvas intercepts double-click, nothing exists here; create a new subsystem
            // Ask for subsystem name
            TextboxDialog dialog = new TextboxDialog("Create new subsystem", "New subsystem name");
            dialog.ShowDialog();
            if (dialog.Cancelled)
            {
                return;
            }

            // Create new element
            SubsystemElement newSubsystem = new SubsystemElement();
            newSubsystem.Name = dialog.StringValue;

            // Create new icon
            SubsystemIcon newIcon = new SubsystemIcon();
            newIcon.MouseDown += new MouseEventHandler(SubsystemClicked);
            newIcon.MouseMove += new MouseEventHandler(newIcon_MouseMove);
            newIcon.subsystem = newSubsystem;
            newIcon.Location = new Point(e.X, e.Y);
            newIcon.Parent = this;
            SubsystemIcons.Add(newIcon);
            RefreshConnections();
        }

        public void newIcon_MouseMove(object sender, MouseEventArgs e)
        {
            // Update connections and canvas if icon moved
            try
            {
                SubsystemIcon ico = (SubsystemIcon)sender;
                if (ico.IsMoving)
                {
                    RefreshConnections();
                }
            }
            catch (InvalidCastException)
            {
                // Sender wasn't a subsystem icon
            }
        }

        public void SubsystemClicked(object sender, MouseEventArgs e)
        {
            try
            {
                SubsystemIcon icon = (SubsystemIcon)sender;
                if (floater != null)
                {
                    // Connect other end
                    if (floater.IndependentIcon == null)
                    {
                        floater.IndependentIcon = icon;
                    }
                    else
                    {
                        floater.DependentIcon = icon;
                    }

                    // Add to list and reset floater reference
                    Connections.Add(floater);
                    floater = null;
                    RefreshConnections();
                    return;
                }
                if (icon.IsOverLeft)
                {
                    // Create floating dependency from left side
                    floater = new DependencyConnection();
                    floater.Parent = this;
                    floater.Location = new Point(0, 0);
                    floater.Height = Height;
                    floater.Width = Width;
                    floater.DependentIcon = icon;
                    floater.OpenPoint = e.Location;
                    floater.Refresh();
                }
                if (icon.IsOverRight)
                {
                    // Create floating (in)dependency from right side
                    floater = new DependencyConnection();
                    floater.Parent = this;
                    floater.Location = new Point(0, 0);
                    floater.Height = Height;
                    floater.Width = Width;
                    floater.IndependentIcon = icon;
                    floater.OpenPoint = e.Location;
                    floater.Refresh();
                }
            }
            catch (InvalidCastException)
            {
                // Subsystem click not sent by subsystem
                MessageBox.Show("Subsystem click not sent by subsystem");
            }
        }

        private void DrawingCanvas_MouseMove(object sender, MouseEventArgs e)
        {
            if (floater != null)
            {
                // Update location of floating connector and redraw
                floater.OpenPoint = e.Location;
                floater.Refresh();
            }
            Refresh();
        }

        private void DrawingCanvas_MouseClick(object sender, MouseEventArgs e)
        {
            // If a floating connector exists, remove it when non-subsystem clicked
            if (floater != null)
            {
                floater = null;
            }
        }

        public void RefreshConnections()
        {
            Refresh();
            foreach (DependencyConnection conn in Connections)
            {
                conn.Refresh();
            }
        }
    }
}

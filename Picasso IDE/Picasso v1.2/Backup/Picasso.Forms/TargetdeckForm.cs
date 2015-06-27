using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Hsf.Utility;
using Hsf.Elements;
using Hsf.Components;
using MementoModel;
using Picasso.Controls;
using Picasso.Utility;

namespace Picasso.Forms
{
    public partial class TargetdeckForm : PicassoSubform, IMessageRouter, IMForm
    {
        #region Variables

        public TargetdeckComponent Targetdeck;
        public List<TargetIcon> targetIcons;
        private float _aspectRatio;
        private MementoManager _mManager;
        private bool _isMoving;

        public override HsfElement Element
        {
            get { return Targetdeck; }
        }

        #endregion

        #region Constructor

        public TargetdeckForm(Form parentWindow, TreeNode node)
        {
            // Creates a new targetdeck form with a new targetdeck component
            Targetdeck = new TargetdeckComponent();
            Initialize(parentWindow, node);
        }

        public TargetdeckForm(TargetdeckComponent targetComponent, Form parentWindow, TreeNode node)
        {
            // Creates targetdeck form from pre-existing targetdeck
            Targetdeck = targetComponent;
            Initialize(parentWindow, node);
        }

        private void Initialize(Form parentWindow, TreeNode node)
        {
            Node = node;
            _aspectRatio = 2048.0f / 1024.0f;
            InitializeComponent();
            Tag = Targetdeck;
            targetIcons = new List<TargetIcon>();
            MdiParent = parentWindow;
            _mManager = new MementoManager((IMForm)MdiParent);
            Text = "Target Deck";
            RefreshTargets();
            worldMap.ZoomFactor = 1.0;
        }

        #endregion

        #region Methods

        public void RefreshTargets()
        {
            // Updates location of each target icon based on worldMap offset and size
            if (targetIcons.Count == 0)
            {
                // Load icons from targets
                foreach (TargetElement te in Targetdeck.Targets)
                {
                    if (te.Position.PositionType == PositionType.StaticLLA)
                    {
                        TargetIcon t = new TargetIcon(te);
                        double longitude = te.Position.Ics[0];
                        double latitude = te.Position.Ics[1];
                        int controlX = Conversion.Longitude2Pixel(longitude, worldMap.Width);
                        int controlY = Conversion.Latitude2Pixel(latitude, worldMap.Height);
                        int iconX = worldMap.Location.X + controlX;
                        int iconY = worldMap.Location.Y + controlY;
                        t.Location = new Point(iconX / 2, iconY / 2);
                        t.Parent = this;
                        t.BringToFront();
                        targetIcons.Add(t);
                    }
                }
            }

            foreach (TargetIcon t in targetIcons)
            {
                if (((TargetElement)t.Tag).Position.PositionType == Hsf.Utility.PositionType.StaticLLA)
                {
                    double longitude = ((TargetElement)t.Tag).Position.Ics[0];
                    double latitude = ((TargetElement)t.Tag).Position.Ics[1];
                    int controlX = Conversion.Longitude2Pixel(longitude, worldMap.Width);
                    int controlY = Conversion.Latitude2Pixel(latitude, worldMap.Height);
                    int iconX = worldMap.Location.X + controlX;
                    int iconY = worldMap.Location.Y + controlY;
                    t.Location = new Point(iconX, iconY);
                }
            }
        }

        public void AddNewTarget(double latitude, double longitude)
        {
            //TargetdeckComponent before = Targetdeck.Clone();

            // Create new component (static LLA by default, since created on map)
            TargetElement newTarget = new TargetElement();
            newTarget.Position.PositionType = Hsf.Utility.PositionType.StaticLLA;
            newTarget.Position.Ics[0] = longitude;
            newTarget.Position.Ics[1] = latitude;
            Targetdeck.Targets.Add(newTarget);
            
            // Create new icon
            double x = (longitude + 180.0) * worldMap.Size.Width / 360.0;
            double y = (-1.0 * latitude + 90.0) * worldMap.Size.Height / 180.0;
            TargetIcon newIcon = new TargetIcon(newTarget);
            newIcon.Location = new Point((int)x, (int)y);
            newIcon.Visible = true;
            targetIcons.Add(newIcon);
            Controls.Add(newIcon);
            newIcon.BringToFront();

            // Add target node
            TreeNode targetNode = new TreeNode(newTarget.TargetName);
            Node.Nodes.Add(targetNode);

            // Create new target form to attach to node
            TargetForm newForm = new TargetForm(newTarget, MdiParent, targetNode);
            targetNode.Tag = newForm;

            // Register event
            //_mManager.RegisterEvent(before, hComponent, hComponent, "Add Target");

            // Make sure target is displayed
            RefreshTargets();
        }

        #endregion

        #region Events

        private void TargetdeckForm_ResizeEnd(object sender, EventArgs e)
        {
            RefreshTargets();
        }

        private void TargetdeckForm_Resize(object sender, EventArgs e)
        {
            Width = (int)((float)Height * _aspectRatio);
            worldMap.ZoomFactor = 1.0;
        }

        private void TargetdeckForm_SizeChanged(object sender, EventArgs e)
        {
            //MessageBox.Show("Targetdeck form size has changed");
            this.Refresh();
        }

        #endregion

        #region Memento

        public void RefreshMementoMenus()
        {
            // Should update undo / redo menus in form's menu strip
            // This form does not contain menu items
        }

        public MementoManager GetMementoManager()
        {
            // Returns the MementoManager belonging to the form
            return _mManager;
        }

        public override void RefreshValues()
        {
            // Updates value visualizations that draw from clonable objects
            RefreshTargets();
        }

        #endregion

        private void zoomInButton_MouseEnter(object sender, EventArgs e)
        {
            // Change image to 'over'
            zoomInButton.Image = global::Picasso.Forms.Properties.Resources.Zoom_In_Over;
        }

        private void zoomInButton_MouseLeave(object sender, EventArgs e)
        {
            // Change image back
            zoomInButton.Image = global::Picasso.Forms.Properties.Resources.Zoom_In;
        }

        private void zoomInButton_MouseClick(object sender, MouseEventArgs e)
        {
            // Zoom in
            worldMap.ZoomFactor = worldMap.ZoomFactor * 1.61803399;
            RefreshTargets();
        }

        private void zoomOutButton_MouseEnter(object sender, EventArgs e)
        {
            // Change image to 'over'
            zoomOutButton.Image = global::Picasso.Forms.Properties.Resources.Zoom_Out_Over;
        }

        private void zoomOutButton_MouseLeave(object sender, EventArgs e)
        {
            // Change image back
            zoomOutButton.Image = global::Picasso.Forms.Properties.Resources.Zoom_Out;
        }

        private void zoomOutButton_MouseClick(object sender, MouseEventArgs e)
        {
            // Zoom out
            worldMap.ZoomFactor = worldMap.ZoomFactor * 0.618033988;
            RefreshTargets();
        }

        private void worldMap_MouseMove_1(object sender, MouseEventArgs e)
        {
            // Register movement and update target locations
            if (e.Button == MouseButtons.Left)
            {
                _isMoving = true;
                RefreshTargets();
            }

            // Don't forget to update cursor position
            int cursorX = e.X;
            int cursorY = e.Y;
            int controlX = cursorX - worldMap.Location.X;
            int controlY = cursorY - worldMap.Location.Y;
            double longitude = Conversion.Pixel2Longitude(cursorX, worldMap.Width);
            double latitude = Conversion.Pixel2Latitude(cursorY, worldMap.Height);
            string latText = latitude > 0 ? (-1 * latitude).ToString() + "° N" : latitude.ToString() + "° S";
            string longText = longitude < 0 ? (-1 * longitude).ToString() + "° W" : longitude.ToString() + "° E";
            cursorStatus.Text = "Cursor Position: (" + longText + ", " + latText + ") (" + e.X + "," + e.Y + ")";
        }

        private void worldMap_MouseUp(object sender, MouseEventArgs e)
        {
            // Create target only if map wasn't being moved
            if (!_isMoving)
            {
                double latitude = -1.0 * ((double)(e.Y)) * 180.0 / ((double)worldMap.Size.Height) + 90.0;
                double longitude = ((double)(e.X)) * 360.0 / ((double)worldMap.Size.Width) - 180.0;
                AddNewTarget(latitude, longitude);
            }
            else
            {
                _isMoving = false;
            }
        }

        #region Message Routing

        public void OnControlMove(object sender, MouseEventArgs e)
        {
            // A target control was moved; update positions
            TargetIcon icon = (TargetIcon)sender;
            int iconX = icon.Location.X;
            int iconY = icon.Location.Y;
            int controlX = iconX - worldMap.Location.X;
            int controlY = iconY - worldMap.Location.Y;
            double longitude = Conversion.Pixel2Longitude(controlX, worldMap.Width);
            double latitude = Conversion.Pixel2Latitude(controlY, worldMap.Height);

            // Change location and register event with memento
            TargetElement target = (TargetElement)icon.Tag;
            TargetElement before = (TargetElement)target.Clone();
            target.Position.Ics[0] = longitude;
            target.Position.Ics[1] = latitude;
            target.Position.Ics[2] = 0.0;
            TargetElement after = (TargetElement)target.Clone();
            _mManager.RegisterEvent(before, after, target, "Move Target");
        }

        public void OnDoubleClick(object sender, MouseEventArgs e)
        {
            // Send to main form in order to open form of target that was clicked
            ((IMessageRouter)MdiParent).OnDoubleClick(sender, e);
        }

        #endregion
    }
}

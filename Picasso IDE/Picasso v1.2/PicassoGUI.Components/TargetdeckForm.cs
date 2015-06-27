using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using MementoModel;

namespace PicassoGUI.Components
{
    public partial class TargetdeckForm : Form, IMForm
    {
        public TargetdeckComponent hComponent;
        public List<TargetIcon> targetIcons;
        private float _aspectRatio;
        private MementoManager _mManager;

        public TargetdeckForm(TargetdeckComponent targetComponent, Form parentWindow)
        {
            InitializeComponent();
            _aspectRatio = (float)Width / (float)Height;
            Tag = targetComponent;
            hComponent = targetComponent;
            targetIcons = new List<TargetIcon>();
            MdiParent = parentWindow;
            _mManager = new MementoManager((IMForm)MdiParent);
            Text = "Target Deck";
            RefreshTargets();
        }

        public void RefreshTargets()
        {
            // (Re)creates icons for each target in deck
            foreach (TargetIcon t in targetIcons)
            {
                t.Visible = false;
            }
            targetIcons.Clear();
            double x, y;
            foreach (TargetComponent target in hComponent.Targets)
            {
                TargetIcon newIcon = new TargetIcon(target);
                x = ((double)target.TargetPosition.InitialConditions[0] + 180.0) * worldMap.Size.Width / 360.0;
                y = ((double)target.TargetPosition.InitialConditions[1] + 90.0) * worldMap.Size.Height / 180.0;
                newIcon.Location = new Point((int)x, (int)y);
                newIcon.Tag = target;
                newIcon.Visible = true;
                targetIcons.Add(newIcon);
                Controls.Add(newIcon);
                newIcon.BringToFront();
            }
            return;
        }

        public void AddNewTarget(double latitude, double longitude)
        {
            TargetdeckComponent before = (TargetdeckComponent)hComponent.Clone();

            // Create new component
            TargetComponent newTarget = new TargetComponent(latitude, longitude);
            hComponent.AddTarget(newTarget);
            
            // Create new icon
            double x = (longitude + 180.0) * worldMap.Size.Width / 360.0;
            double y = (latitude + 90.0) * worldMap.Size.Height / 180.0;
            TargetIcon newIcon = new TargetIcon(newTarget);
            newIcon.Location = new Point((int)x, (int)y);
            newIcon.Visible = true;
            newIcon.Tag = newTarget;
            targetIcons.Add(newIcon);
            Controls.Add(newIcon);
            newIcon.BringToFront();

            // Register event
            _mManager.RegisterEvent(before, hComponent, hComponent, "Add Target");
        }

        private void worldMap_MouseMove(object sender, MouseEventArgs e)
        {
            // Update status strip with LLA estimate of cursor position
            double latitude = ((double)e.Y) * 180.0 / worldMap.Size.Height - 90.0;
            double longitude = ((double)e.X) * 360.0 / worldMap.Size.Width - 180.0;
            string latText = latitude < 0 ? (-1 * latitude).ToString() + "° N" : latitude.ToString() + "° S";
            string longText = longitude < 0 ? (-1 * longitude).ToString() + "° W" : longitude.ToString() + "° E";
            cursorStatus.Text = "Cursor Position: (" + latText + ", " + longText + ")";
        }

        private void worldMap_MouseClick(object sender, MouseEventArgs e)
        {
            // Create new target at this location
            double latitude = ((double)(e.Y)) * 180.0 / ((double)worldMap.Size.Height) - 90.0;
            double longitude = ((double)(e.X)) * 360.0 / ((double)worldMap.Size.Width) - 180.0;
            AddNewTarget(latitude, longitude);
        }

        private void TargetdeckForm_ResizeEnd(object sender, EventArgs e)
        {
            RefreshTargets();
        }

        private void TargetdeckForm_Resize(object sender, EventArgs e)
        {
            Width = (int)((float)Height * _aspectRatio);
        }

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

        public void RefreshValues()
        {
            // Updates value visualizations that draw from clonable objects
            RefreshTargets();
        }

        private void TargetdeckForm_SizeChanged(object sender, EventArgs e)
        {
            //MessageBox.Show("Targetdeck form size has changed");
            this.Refresh();
        }

        private void TargetdeckForm_MouseUp(object sender, MouseEventArgs e)
        {
            MessageBox.Show("Mouse up in target deck!");
        }
    }
}

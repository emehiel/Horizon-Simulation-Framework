using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using Hsf.Components;
using Hsf.Elements;
using Hsf.Utility;
using Picasso.Controls;
using Picasso.Utility;
using MementoModel;

namespace Picasso.Forms
{
    public partial class AssetForm : PicassoSubform, IMessageRouter, IMForm
    {
        private MementoManager _mManager;
        public AssetElement Asset;
        public override HsfElement Element
        {
            get
            {
                return Asset;
            }
        }

        public AssetForm(Form parentWindow, TreeNode node)
        {
            // Creates a new asset form for a new asset element
            Node = node;
            Asset = new AssetElement();
            MdiParent = parentWindow;
            Initialize();
        }

        public AssetForm(AssetElement ae, Form parentWindow, TreeNode node)
        {
            // Creates a new asset form for a given asset element
            Node = node;
            Asset = ae;
            MdiParent = parentWindow;
            Initialize();

            // Create subsystem icons for asset subsystems
            int currX = 32;
            int currY = 32;
            foreach (SubsystemElement sub in ae.Subsystems)
            {
                // Create new icon
                SubsystemIcon newIcon = new SubsystemIcon();
                newIcon.MouseDown += new MouseEventHandler(drawingCanvas1.SubsystemClicked);
                newIcon.MouseMove += new MouseEventHandler(drawingCanvas1.newIcon_MouseMove);
                newIcon.subsystem = sub;
                newIcon.Location = new Point(currX, currY);
                newIcon.Parent = drawingCanvas1;
                newIcon.BringToFront();
                int seed = (int)(DateTime.UtcNow.TimeOfDay.Ticks);
                Random random = new Random(seed);
                Thread.Sleep((int)(random.NextDouble() * 100));
                newIcon.Shade = MagicColors.RandomColor(40);
                drawingCanvas1.SubsystemIcons.Add(newIcon);

                // Update location of icons
                currX += 64;
                currY = currY == 32 ? 128 : 32;
            }
        }

        public void Initialize()
        {
            _mManager = new MementoManager((IMForm)MdiParent);
            InitializeComponent();
            Tag = Asset;
            positionTypeComboBox.DataSource = Enum.GetValues(typeof(PositionType));
            eomTypeComboBox.DataSource = Enum.GetValues(typeof(EomsType));
            Text = Asset.Name + " (Asset)";
            RefreshValues();
        }

        public override void RefreshValues()
        {
            // Refreshes control values based from element values
            positionTypeComboBox.SelectedItem = Asset.Position.PositionType;
            x1Textbox.Text = Asset.Position.Ics[0].ToString();
            y1Textbox.Text = Asset.Position.Ics[1].ToString();
            z1Textbox.Text = Asset.Position.Ics[2].ToString();
            try
            {
                x2Textbox.Text = Asset.Position.Ics[3].ToString();
                y2Textbox.Text = Asset.Position.Ics[4].ToString();
                z2Textbox.Text = Asset.Position.Ics[5].ToString();
            }
            catch (IndexOutOfRangeException)
            {
                // Current position has only three degrees of freedom
            }

            // Load EOMs, too
            eomTypeComboBox.SelectedItem = Asset.Position.Eoms.Type;
            if (Asset.Position.Eoms.Type == EomsType.None)
            {
                initFuncTextbox.Text = "";
                initFuncTextbox.Enabled = false;
                evalFuncTextbox.Text = "";
                evalFuncTextbox.Enabled = false;
            }
            else
            {
                initFuncTextbox.Text = Asset.Position.Eoms.InitialFunction;
                initFuncTextbox.Enabled = true;
                evalFuncTextbox.Text = Asset.Position.Eoms.EvaulationFunction;
                evalFuncTextbox.Enabled = true;
            }
        }

        #region Messge Routing

        public void OnDoubleClick(object sender, MouseEventArgs e)
        {
            // Pass double-click to main form
            try
            {
                IMessageRouter parent = (IMessageRouter)ParentForm;
                parent.OnDoubleClick(sender, e);
            }
            catch (InvalidCastException)
            {
                // Mdi parent doesn't implement IMessageRouter
            }
        }

        public void OnControlMove(object sender, MouseEventArgs e)
        {
            // A subsystem control has been moved; register event
            SubsystemIcon icon = (SubsystemIcon)sender;
            SubsystemIcon before = new SubsystemIcon();
            before.Location = new Point(icon.PreviousLocation.X, icon.PreviousLocation.Y);
            SubsystemIcon after = new SubsystemIcon();
            after.Location = new Point(icon.Location.X, icon.Location.Y);
            _mManager.RegisterEvent(before, after, icon, "Move Subsystem");
        }

        #endregion

        private void positionTypeComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            // Change position type of element and register event
            AssetElement before = (AssetElement)Asset.Clone();
            Asset.Position.PositionType = (PositionType)positionTypeComboBox.SelectedValue;
            AssetElement after = (AssetElement)Asset.Clone();
            if (before.ToXml() != after.ToXml()) { _mManager.RegisterEvent(before, after, Asset, "Position Type Change"); }

            // Update labels, if necessary
            switch(Asset.Position.PositionType) {
                case PositionType.DynamicECI:
                case PositionType.PredeterminedECI:
                case PositionType.StaticECI:
                    // ECI: show all textboxes, update labels, and enlarge ICs
                    x2Textbox.Visible = true; x2Label.Visible = true;
                    y2Textbox.Visible = true; y2Label.Visible = true;
                    z2Textbox.Visible = true; z2Label.Visible = true;
                    x1Label.Text = "Px";
                    y1Label.Text = "Py";
                    z1Label.Text = "Pz";
                    x2Label.Text = "Vx";
                    y2Label.Text = "Vy";
                    z2Label.Text = "Vz";
                    Asset.Position.Ics.ChangeDimensions(6, 1);
                    break;
                case PositionType.DynamicLLA:
                case PositionType.PredeterminedLLA:
                case PositionType.StaticLLA:
                    // LLA: hide extra textboxes and update labels
                    x2Textbox.Visible = false; x2Label.Visible = false;
                    y2Textbox.Visible = false; y2Label.Visible = false;
                    z2Textbox.Visible = false; z2Label.Visible = false;
                    x1Label.Text = "Long";
                    y1Label.Text = "Lat";
                    z1Label.Text = "Alt";
                    Asset.Position.Ics.ChangeDimensions(3, 1);
                    break;
                default:
                    MessageBox.Show("Unknown asset position type");
                    break;
            }
        }

        #region Memento

        public void RefreshMementoMenus()
        {
            // Should update undo / redo menus in form's menu strip according to this manager
            return;
        }

        public MementoManager GetMementoManager()
        {
            // Returns the MementoManager belonging to the form
            return _mManager;
        }

        #endregion

        private void x1Textbox_TextChanged(object sender, EventArgs e)
        {
            // Change position[0] value and register event
            try
            {
                AssetElement before = (AssetElement)Asset.Clone();
                Asset.Position.Ics[0] = Convert.ToDouble(x1Textbox.Text);
                AssetElement after = (AssetElement)Asset.Clone();
                if (before.ToXml() != after.ToXml()) { _mManager.RegisterEvent(before, after, Asset, "X1 Value Change"); }
            }
            catch (FormatException)
            {
                // Improperly formatted value; should be a decimal value
                MessageBox.Show("Improperly formatted value; should be a decimal value");
            }
        }

        private void y1Textbox_TextChanged(object sender, EventArgs e)
        {
            // Change position[1] value and register event
            try
            {
                AssetElement before = (AssetElement)Asset.Clone();
                Asset.Position.Ics[1] = Convert.ToDouble(y1Textbox.Text);
                AssetElement after = (AssetElement)Asset.Clone();
                if (before.ToXml() != after.ToXml()) { _mManager.RegisterEvent(before, after, Asset, "Y1 Value Change"); }
            }
            catch (FormatException)
            {
                // Improperly formatted value; should be a decimal value
                MessageBox.Show("Improperly formatted value; should be a decimal value");
            }
        }

        private void z1Textbox_TextChanged(object sender, EventArgs e)
        {
            // Change position[2] value and register event
            try
            {
                AssetElement before = (AssetElement)Asset.Clone();
                Asset.Position.Ics[2] = Convert.ToDouble(z1Textbox.Text);
                AssetElement after = (AssetElement)Asset.Clone();
                if (before.ToXml() != after.ToXml()) { _mManager.RegisterEvent(before, after, Asset, "Z1 Value Change"); }
            }
            catch (FormatException)
            {
                // Improperly formatted value; should be a decimal value
                MessageBox.Show("Improperly formatted value; should be a decimal value");
            }
        }

        private void x2Textbox_TextChanged(object sender, EventArgs e)
        {
            // Change position[3] value and register event
            try
            {
                AssetElement before = (AssetElement)Asset.Clone();
                Asset.Position.Ics[3] = Convert.ToDouble(x2Textbox.Text);
                AssetElement after = (AssetElement)Asset.Clone();
                if (before.ToXml() != after.ToXml()) { _mManager.RegisterEvent(before, after, Asset, "X2 Value Change"); }
            }
            catch (FormatException)
            {
                // Improperly formatted value; should be a decimal value
                MessageBox.Show("Improperly formatted value; should be a decimal value");
            }
        }

        private void y2Textbox_TextChanged(object sender, EventArgs e)
        {
            // Change position[4] value and register event
            try
            {
                AssetElement before = (AssetElement)Asset.Clone();
                Asset.Position.Ics[4] = Convert.ToDouble(y2Textbox.Text);
                AssetElement after = (AssetElement)Asset.Clone();
                if (before.ToXml() != after.ToXml()) { _mManager.RegisterEvent(before, after, Asset, "Y2 Value Change"); }
            }
            catch (FormatException)
            {
                // Improperly formatted value; should be a decimal value
                MessageBox.Show("Improperly formatted value; should be a decimal value");
            }
        }

        private void z2Textbox_TextChanged(object sender, EventArgs e)
        {
            // Change position[5] value and register event
            try
            {
                AssetElement before = (AssetElement)Asset.Clone();
                Asset.Position.Ics[5] = Convert.ToDouble(z2Textbox.Text);
                AssetElement after = (AssetElement)Asset.Clone();
                if (before.ToXml() != after.ToXml()) { _mManager.RegisterEvent(before, after, Asset, "Z2 Value Change"); }
            }
            catch (FormatException)
            {
                // Improperly formatted value; should be a decimal value
                MessageBox.Show("Improperly formatted value; should be a decimal value");
            }
        }

        private void eomTypeComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            // Change element's type of EOM and register event
            AssetElement before = (AssetElement)Asset.Clone();
            Asset.Position.Eoms.Type = (EomsType)eomTypeComboBox.SelectedValue;
            AssetElement after = (AssetElement)Asset.Clone();
            if (before.ToXml() != after.ToXml()) { _mManager.RegisterEvent(before, after, Asset, "EOMs Type Change"); }

            // Enable / disable controls based on type
            if (Asset.Position.Eoms.Type == EomsType.None)
            {
                initFuncTextbox.Enabled = false;
                evalFuncTextbox.Enabled = false;
            }
            else
            {
                initFuncTextbox.Enabled = true;
                evalFuncTextbox.Enabled = true;
            }

        }

        private void initFuncTextbox_TextChanged(object sender, EventArgs e)
        {
            // Save change to initial function and register event
            AssetElement before = (AssetElement)Asset.Clone();
            Asset.Position.Eoms.InitialFunction = initFuncTextbox.Text;
            AssetElement after = (AssetElement)Asset.Clone();
            if (before.ToXml() != after.ToXml()) { _mManager.RegisterEvent(before, after, Asset, "Initial Function Change"); }

        }

        private void evalFuncTextbox_TextChanged(object sender, EventArgs e)
        {
            // Save change to evaluation function and register event
            AssetElement before = (AssetElement)Asset.Clone();
            Asset.Position.Eoms.EvaulationFunction = evalFuncTextbox.Text;
            AssetElement after = (AssetElement)Asset.Clone();
            if (before.ToXml() != after.ToXml()) { _mManager.RegisterEvent(before, after, Asset, "Evaluation Function Change"); }
        }
    }
}

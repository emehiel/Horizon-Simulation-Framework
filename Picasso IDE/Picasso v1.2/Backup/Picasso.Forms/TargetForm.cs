using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Hsf.Elements;
using Hsf.Utility;
using MementoModel;

namespace Picasso.Forms
{
    public partial class TargetForm : PicassoSubform, IMForm
    {
        #region Variables

        private MementoManager _mManager;
        private bool _isLLA;
        public TargetElement Target;
        public override HsfElement Element
        {
            get { return Target; }
        }

        #endregion

        #region Constructors

        public TargetForm(Form parentWindow, TreeNode node)
        {
            // Create TargetForm with new target element
            _isLLA = true;
            Target = new TargetElement();
            Node = node;
            MdiParent = parentWindow;
            Initialize();
        }

        public TargetForm(TargetElement targetTarget, Form parentWindow, TreeNode node)
        {
            // Create TargetForm with known target element
            _isLLA = targetTarget.Position.PositionType == PositionType.StaticLLA;
            Target = targetTarget;
            Node = node;
            MdiParent = parentWindow;
            Initialize();
        }

        private void Initialize()
        {
            InitializeComponent();
            typeComboBox.DataSource = Enum.GetValues(typeof(PositionType));
            SetPositionType(Target.Position.PositionType);
            Tag = Target;
            RefreshValues();
            Text = Target.TargetName;
            _mManager = new MementoManager((IMForm)MdiParent);
        }

        #endregion

        #region Methods

        public override void RefreshValues()
        {
            // Update controls values from target element values
            typeComboBox.SelectedItem = Target.Position.PositionType;
            switch (Target.Position.PositionType)
            {
                case PositionType.DynamicECI:
                case PositionType.PredeterminedECI:
                case PositionType.StaticECI:
                    _isLLA = false;
                    pxBox.Text = Target.Position.Ics[0].ToString();
                    pyBox.Text = Target.Position.Ics[1].ToString();
                    pzBox.Text = Target.Position.Ics[2].ToString();
                    vxBox.Text = Target.Position.Ics[3].ToString();
                    vyBox.Text = Target.Position.Ics[4].ToString();
                    vzBox.Text = Target.Position.Ics[5].ToString();
                    break;
                case PositionType.DynamicLLA:
                case PositionType.PredeterminedLLA:
                case PositionType.StaticLLA:
                    _isLLA = true;
                    longBox.Text = Target.Position.Ics[0].ToString();
                    latBox.Text = Target.Position.Ics[1].ToString();
                    altBox.Text = Target.Position.Ics[2].ToString();
                    break;
                default:
                    MessageBox.Show("Unknown position type");
                    break;
            }
        }

        public void SetPositionType(PositionType type)
        {
            // Update target element position type from given value and adjust other controls
            Target.Position.PositionType = type;

            // ECI coordinates require six values; LLA requires three
            switch (type)
            {
                case PositionType.DynamicECI:
                case PositionType.PredeterminedECI:
                case PositionType.StaticECI:
                    if (_isLLA == true)
                    {
                        // Change flag and update position
                        _isLLA = false;
                        Target.Position.Ics = new HsfMatrix(6, 1);

                        // Hide LLA controls
                        xLabel.Visible = false;
                        yLabel.Visible = false;
                        zLabel.Visible = false;
                        longBox.Visible = false;
                        latBox.Visible = false;
                        altBox.Visible = false;

                        // Show ECI controls
                        pxLabel.Visible = true;
                        pyLabel.Visible = true;
                        pzLabel.Visible = true;
                        vxLabel.Visible = true;
                        vyLabel.Visible = true;
                        vzLabel.Visible = true;
                        pxBox.Visible = true;
                        pyBox.Visible = true;
                        pzBox.Visible = true;
                        vxBox.Visible = true;
                        vyBox.Visible = true;
                        vzBox.Visible = true;
                    }
                    break;
                case PositionType.DynamicLLA:
                case PositionType.PredeterminedLLA:
                case PositionType.StaticLLA:
                    if (_isLLA == false)
                    {
                        // Change flag and update position
                        _isLLA = true;
                        Target.Position.Ics = new HsfMatrix(3, 1);

                        // Show ECI controls
                        pxLabel.Visible = false;
                        pyLabel.Visible = false;
                        pzLabel.Visible = false;
                        vxLabel.Visible = false;
                        vyLabel.Visible = false;
                        vzLabel.Visible = false;
                        pxBox.Visible = false;
                        pyBox.Visible = false;
                        pzBox.Visible = false;
                        vxBox.Visible = false;
                        vyBox.Visible = false;
                        vzBox.Visible = false;

                        // Show LLA controls
                        xLabel.Visible = true;
                        yLabel.Visible = true;
                        zLabel.Visible = true;
                        longBox.Visible = true;
                        latBox.Visible = true;
                        altBox.Visible = true;
                    }
                    break;
                default:
                    break;
            }
        }

        #endregion

        #region Events

        private void xBox_TextChanged(object sender, EventArgs e)
        {
            if (_isLLA)
            {
                TargetElement before = (TargetElement)Target.Clone();
                Target.Position.Ics[0] = Convert.ToDouble(longBox.Text);
                TargetElement after = (TargetElement)Target.Clone();
                if (before.ToXml() != after.ToXml()) { _mManager.RegisterEvent(before, after, Target, "Longitude Change"); }
            }
        }
        
        private void yBox_TextChanged(object sender, EventArgs e)
        {
            if (_isLLA)
            {
                TargetElement before = (TargetElement)Target.Clone();
                Target.Position.Ics[1] = Convert.ToDouble(latBox.Text);
                TargetElement after = (TargetElement)Target.Clone();
                if (before.ToXml() != after.ToXml()) { _mManager.RegisterEvent(before, after, Target, "Latitude Change"); }
            }
        }

        private void zBox_TextChanged(object sender, EventArgs e)
        {
            if (_isLLA)
            {
                TargetElement before = (TargetElement)Target.Clone();
                Target.Position.Ics[2] = Convert.ToDouble(altBox.Text);
                TargetElement after = (TargetElement)Target.Clone();
                if (before.ToXml() != after.ToXml()) { _mManager.RegisterEvent(before, after, Target, "Altitude Change"); }
            }
        }

        private void vxBox_TextChanged(object sender, EventArgs e)
        {
            if (!_isLLA) {
                TargetElement before = (TargetElement)Target.Clone();
                Target.Position.Ics[3] = Convert.ToDouble(vxBox.Text);
                TargetElement after = (TargetElement)Target.Clone();
                if (before.ToXml() != after.ToXml()) { _mManager.RegisterEvent(before, after, Target, "Velocity (X) Change"); }
            }
        }

        private void vyBox_TextChanged(object sender, EventArgs e)
        {
            if (!_isLLA) {
                TargetElement before = (TargetElement)Target.Clone();
                Target.Position.Ics[4] = Convert.ToDouble(vyBox.Text);
                TargetElement after = (TargetElement)Target.Clone();
                if (before.ToXml() != after.ToXml()) { _mManager.RegisterEvent(before, after, Target, "Velocity (Y) Change"); }
            }
        }

        private void vzBox_TextChanged(object sender, EventArgs e)
        {
            if (!_isLLA) {
                TargetElement before = (TargetElement)Target.Clone();
                Target.Position.Ics[5] = Convert.ToDouble(vzBox.Text);
                TargetElement after = (TargetElement)Target.Clone();
                if (before.ToXml() != after.ToXml()) { _mManager.RegisterEvent(before, after, Target, "Velocity (Z) Change"); }
            }
        }

        private void pxBox_TextChanged(object sender, EventArgs e)
        {
            if (!_isLLA)
            {
                TargetElement before = (TargetElement)Target.Clone();
                Target.Position.Ics[0] = Convert.ToDouble(pxBox.Text);
                TargetElement after = (TargetElement)Target.Clone();
                if (before.ToXml() != after.ToXml()) { _mManager.RegisterEvent(before, after, Target, "Position (X) Change"); }
            }
        }

        private void pyBox_TextChanged(object sender, EventArgs e)
        {
            if (!_isLLA)
            {
                TargetElement before = (TargetElement)Target.Clone();
                Target.Position.Ics[1] = Convert.ToDouble(pyBox.Text);
                TargetElement after = (TargetElement)Target.Clone();
                if (before.ToXml() != after.ToXml()) { _mManager.RegisterEvent(before, after, Target, "Position (Y) Change"); }
            }
        }

        private void pzBox_TextChanged(object sender, EventArgs e)
        {
            if (!_isLLA)
            {
                TargetElement before = (TargetElement)Target.Clone();
                Target.Position.Ics[2] = Convert.ToDouble(pzBox.Text);
                TargetElement after = (TargetElement)Target.Clone();
                if (before.ToXml() != after.ToXml()) { _mManager.RegisterEvent(before, after, Target, "Position (Z) Change"); }
            }
        }

        #endregion

        private void typeComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            // Register change of position type
            TargetElement before = (TargetElement)Target.Clone();
            SetPositionType((PositionType)typeComboBox.SelectedItem);
            TargetElement after = (TargetElement)Target.Clone();
            if (before.ToXml() != after.ToXml()) { _mManager.RegisterEvent(before, after, Target, "Position Type Change"); }
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
    }
}

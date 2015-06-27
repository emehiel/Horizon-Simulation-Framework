using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace PicassoGUI.Components
{
    public partial class TargetForm : Form
    {
        public TargetComponent hComponent;

        public TargetForm(TargetComponent targetTarget)
        {
            InitializeComponent();
            typeComboBox.DataSource = Enum.GetValues(typeof(PositionType));
            SetPositionType(targetTarget.TargetPosition.Type);
            hComponent = targetTarget;
            Tag = targetTarget;
            RefreshValues();
            Text = hComponent.TargetName + " (Target)";
        }

        public void RefreshValues()
        {
            xBox.Text = hComponent.TargetPosition.InitialConditions[0].ToString();
            yBox.Text = hComponent.TargetPosition.InitialConditions[1].ToString();
            zBox.Text = hComponent.TargetPosition.InitialConditions[2].ToString();
        }

        public void SetPositionType(PositionType type)
        {
            // Changing target type requires an update of labels for values
            switch (type)
            {
                case PositionType.DynamicECI:
                case PositionType.PredeterminedECI:
                case PositionType.StaticECI:
                    xLabel.Text = "Equinox";
                    yLabel.Text = "East";
                    zLabel.Text = "Spin Up";
                    break;
                case PositionType.DynamicLLA:
                case PositionType.PredeterminedLLA:
                case PositionType.StaticLLA:
                    xLabel.Text = "Longitude";
                    yLabel.Text = "Latitude";
                    zLabel.Text = "Altitude";
                    break;
                default:
                    break;
            }
        }

        private void typeComboBox_SelectedValueChanged(object sender, EventArgs e)
        {
            foreach (PositionType testPosition in Enum.GetValues(typeof(PositionType)))
            {
                if (testPosition == (PositionType)typeComboBox.SelectedItem)
                {
                    SetPositionType(testPosition);
                }
            }
        }

        private void xBox_TextChanged(object sender, EventArgs e)
        {
            hComponent.TargetPosition.InitialConditions[0] = Convert.ToDouble(xBox.Text);
        }
        
        private void yBox_TextChanged(object sender, EventArgs e)
        {
            hComponent.TargetPosition.InitialConditions[1] = Convert.ToDouble(yBox.Text);
        }

        private void zBox_TextChanged(object sender, EventArgs e)
        {
            hComponent.TargetPosition.InitialConditions[2] = Convert.ToDouble(zBox.Text);
        }
    }
}

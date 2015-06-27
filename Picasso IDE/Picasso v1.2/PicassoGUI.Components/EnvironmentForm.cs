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
    public partial class EnvironmentForm : Form
    {
        EnvironmentComponent hComponent;

        public EnvironmentForm(EnvironmentComponent targetEnvironment)
        {
            InitializeComponent();
            hComponent = targetEnvironment;
            Tag = targetEnvironment;
            RefreshValues();
            Text = "Environment Settings";
        }

        public void RefreshValues()
        {
            constSunCheck.Checked = hComponent.Sun.IsSunVecConstant;
        }
    }
}

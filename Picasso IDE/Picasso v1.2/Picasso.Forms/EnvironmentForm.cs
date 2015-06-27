using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using MementoModel;
using Hsf.Components;
using Hsf.Elements;

namespace Picasso.Forms
{
    public partial class EnvironmentForm : PicassoSubform
    {
        public EnvironmentComponent Environment;
        public override HsfElement Element
        {
            get
            {
                return Environment;
            }
        }

        public EnvironmentForm(Form parentWindow, TreeNode node)
        {
            // Creates a new form for a new environment
            Node = node;
            Environment = new EnvironmentComponent();
            InitializeComponent();
            Tag = Environment;
            RefreshValues();
            Text = "Environment Settings";
            MdiParent = parentWindow;
        }

        public EnvironmentForm(EnvironmentComponent targetEnvironment, Form parentWindow, TreeNode node)
        {
            // Creates a new form for a pre-existing environment
            Node = node;
            Environment = targetEnvironment;
            InitializeComponent();
            Tag = targetEnvironment;
            RefreshValues();
            Text = "Environment Settings";
            MdiParent = parentWindow;
        }

        public override void RefreshValues()
        {
            constSunCheck.Checked = Environment.IsSunVecConstant;
        }
    }
}

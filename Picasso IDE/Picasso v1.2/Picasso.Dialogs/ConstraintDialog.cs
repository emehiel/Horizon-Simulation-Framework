using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Hsf.Utility;

namespace Picasso.Dialogs
{
    public partial class ConstraintDialog : Form
    {
        public string ConstraintValue
        {
            get { return valueLabel.Text; }
        }

        public ConstraintType ConstraintType
        {
            get { return (ConstraintType)typeCombo.SelectedItem; }
        }

        public string VariableName
        {
            get { return nameTextbox.Text; }
        }

        public HsfType VariableDatatype
        {
            get { return (HsfType)datatypeCombo.SelectedItem; }
        }

        public ConstraintDialog()
        {
            InitializeComponent();
            typeCombo.DataSource = Enum.GetValues(typeof(ConstraintType));
            datatypeCombo.DataSource = Enum.GetValues(typeof(HsfType));
        }

        private void okButton_Click(object sender, EventArgs e)
        {
            // Hide this form for constraint data retrival
            Close();
        }
    }
}

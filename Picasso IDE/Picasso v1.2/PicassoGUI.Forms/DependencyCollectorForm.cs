using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using PicassoGUI.Controls;

namespace PicassoGUI
{
    public partial class DependencyCollectorForm : Form
    {
        public DependencyCollector DependencyCollector { get; set; }

        public DependencyCollectorForm(DependencyCollector depColl)
        {
            InitializeComponent();
            this.DependencyCollector = depColl;
            this.depComTextBox.Text = this.DependencyCollector.DependencyCombination;
            this.depFcnCheckBox.Checked = false;
            this.depFcnRichTextBox.Enabled = false;
        }

        private void depComTextBox_TextChanged(object sender, EventArgs e)
        {
            this.DependencyCollector.DependencyCombination = this.depComTextBox.Text;
        }
    }
}
